/*  Copyright (C) 2011 Jonathan Wuest

	This file is part of Tuchulcha.

	Tuchulcha is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Tuchulcha is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file   NodeHandler.cpp
 *  \brief  Implementation of class NodeHandler
 *  \date   15.03.2011
 *  \author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#include "Node.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsView>
#include <QStandardItemModel>
#include <QKeyEvent>
#include <QPrinter>
#include <QMenu>

#include "ConnectionLine.h"
#include "NodeHandler.h"
#include "GraphModel.h"
#include "MetaData.h"
#include "FileManager.h"
#include "QParameterFile.h"
#include <stdexcept>

NodeHandler::NodeHandler(GraphModel* model, QObject* pp) :
		QGraphicsScene(pp),
		_cline(0),
		_model(model),
		_addLine(false)
{
	Q_ASSERT(_model);
	connect(_model, SIGNAL(graphChanged()), this, SLOT(loadFromModel()));
	connect(_model, SIGNAL(prefixChanged(QString)),
			this, SLOT(selectNode(QString)));
	loadFromModel();
}

NodeHandler::~NodeHandler()
{
	delete _cline ;
}

void NodeHandler::_deselectAllNodes() {
	QMapIterator<QString,Node*> iter(_nodeMap);
	while(iter.hasNext()) {
		Node* n = iter.next().value();
		n->setSelectedNode(false);
	}
	_selectedNode = 0;
}

void NodeHandler::selectNode(QString name) {
	if(_selectedNode) {
		if (_selectedNode->getInstanceName() == name) {
			return;
		}
		_selectedNode->setSelectedNode(false);
	}
	_selectedNode = _nodeMap[name];
	if (_selectedNode) {
		_selectedNode->setSelectedNode(true);
	}
	update();
}


void NodeHandler::mousePressEvent(QGraphicsSceneMouseEvent* ev) {
	QGraphicsScene::mousePressEvent(ev);

	if (ev->button() == Qt::LeftButton) {
		QGraphicsItem* itm = itemAt(ev->scenePos());
		if (!itm) {
			return;
		}
		Node* np = dynamic_cast<Node*>(itm);
		if (!np) {
			np = dynamic_cast<Node*>(itm->parentItem());
		}
		if (np) {
			_model->setPrefix(np->getInstanceName());
		}
		NodeProperty* prop = dynamic_cast<NodeProperty*>(itm);
		if (prop != 0) {
			_startProp = prop;
			delete _cline ; _cline = 0 ;
			_cline = new ConnectionLine(this);
			QPointF sckPos(prop->pos()+prop->getSocketCenter());
			QPointF curPos(ev->scenePos().x(),ev->scenePos().y());
			const bool isIn = prop->isInput();
			_cline->setStartPoint(isIn ? curPos : sckPos);
			_cline->setEndPoint  (isIn ? sckPos : curPos);
			_addLine = true;
			update(_cline->boundingRect());
		}
	}
}

void NodeHandler::mouseMoveEvent(QGraphicsSceneMouseEvent* ev) {
	if (_addLine) {
		if(_startProp->isInput()) {
			_cline->setStartPoint(ev->scenePos().x(),ev->scenePos().y());
		} else {
			_cline->setEndPoint(ev->scenePos().x(),ev->scenePos().y());
		}
	}
	else {
		QGraphicsScene::mouseMoveEvent(ev);
	}
	update();
}

void NodeHandler::mouseReleaseEvent(QGraphicsSceneMouseEvent* ev) {
	QGraphicsScene::mouseReleaseEvent(ev);
	NodeProperty* prop = dynamic_cast<NodeProperty*>(itemAt(ev->scenePos()));

	delete _cline;
	_cline = 0;

	if (_addLine && prop && (prop != _startProp)) {
		try {
			_model->connectSlot(
					_startProp->getFullName(),
					prop->getFullName(),true);
		} catch (const std::runtime_error& err) {
			emit statusMessage(err.what());
		}
	}
	_addLine = false;
}


GraphModel* NodeHandler::model() {
	return _model;
}

bool NodeHandler::load(QString fname) {
	return _model->load(fname);
}

void NodeHandler::loadFromModel() {
	clear();
	_nodeMap.clear();
	_deselectAllNodes();
	QStringList nodes = _model->nodes();
	QStringList nodesout,nodesin;
	QStringList slotout,slotin;

	for (int ii=0;ii<nodes.size();ii++) {
		QString name = nodes[ii];
		QString cname = _model->getClass(nodes[ii], true);

		Node* node = new Node(_model,name,10*ii,10*ii,this);
		node->setClassName(cname);
		_nodeMap.insert(name,node);

		QStringList pdata = _model->getValue(name+".editorinfo").split(" ");
		if (pdata.size() >= 2) {
			float x = pdata.at(0).toFloat();
			float y = pdata.at(1).toFloat();
			node->setPos(x,y);
		}

		// set tooltip
		QString str = _model->getValue(name+".editorcomment");
		if (str.isEmpty()) {
			node->setToolTip("");
		} else {
			node->setToolTip("<b>" + tr("Comment:") + "</b><br/>" + str);
		}

		QStringList ins = _model->getInputs(name);
		QStringList outs = _model->getOutputs(name);
		for (int jj=0; jj < ins.size(); jj++) {
			node->addProperty(ins[jj], true);
		}
		for (int jj=0; jj < outs.size(); jj++) {
			node->addProperty(outs[jj], false);
		}
		for (int jj=0; jj < outs.size(); jj++) {
			QString curO = name+"."+outs[jj];
			QStringList conns =
				_model->getValue(curO).split(";",QString::SkipEmptyParts);
			foreach (const QString& con, conns) {
				nodesout << name;
				slotout << outs[jj];
				slotin << con.section(".",-1,-1);
				nodesin << con.section(".",0,0);
			}
		}
		node->moveBy(0,0);
	}
	for (int ii=0; ii<slotout.size(); ii++) {
		try {
			connectNodes(nodesout[ii],slotout[ii],nodesin[ii],slotin[ii]);
		}
		catch(const std::exception& e) {
			if (QMessageBox::question(
					0, "connection error",
					QString(
							"Failed to connect <em>%1.%2</em> "
							"to <em>%3.%4</em>.<br>"
							"This may happen after slot renaming<br>"
							"and/or using old module descriptions.<br><br>"
							"Exception message:<br>%5<br><br>"
							"Remove this invalid connection from workflow?")
						.arg(nodesout[ii]).arg(slotout[ii])
						.arg(nodesin[ii]).arg(slotin[ii])
						.arg(e.what()), QMessageBox::Yes, QMessageBox::No)
					== QMessageBox::Yes) {
				_model->disconnectSlot(
					nodesout[ii]+"."+slotout[ii],nodesin[ii]+"."+slotin[ii]);
			}
		}
	}
	update();
	selectNode(_model->prefix());
}

void NodeHandler::connectNodes(
		QString node0, QString prop0, QString node1, QString prop1) {
	Node* out = _nodeMap[node0];
	Node* in = _nodeMap[node1];
	Q_ASSERT(out && in) ;
	NodeProperty* outp = out->getProperty(prop0.toLower());
	NodeProperty* inp = in->getProperty(prop1.toLower());

	if(!out) {
		throw std::runtime_error(QString(
				"Property <em>%1.%2</em> does not exist.")
					.arg(node0).arg(prop0).toStdString());
	}
	if (!inp) {
		throw std::runtime_error(QString(
				"Property <em>%1.%2</em> does not exist.")
					.arg(node1).arg(prop1).toStdString());
	}

	Q_ASSERT(outp && inp) ;

	ConnectionLine* l = new ConnectionLine(this);
	l->setStartEndProp(outp,inp);
	outp->addConnection(l);
	inp->addConnection(l);
	outp->moveBy(0,0);
	inp->moveBy(0,0);
}

void NodeHandler::saveFlowchart() {
	QString fname = QFileDialog::getSaveFileName(
		0, tr("Select File to Write"),
		"",tr("Images (*.png *.jpg *.bmp *.gif);;"
			"PDF/Postscript (*.pdf *.ps)"));
	if (fname.contains(QRegExp("\\.(pdf|ps)\\s*$",Qt::CaseInsensitive))) {
		QPrinter printer(QPrinter::HighResolution);
		printer.setPaperSize(sceneRect().size(),QPrinter::Point);
		printer.setOutputFileName(fname);
		printer.setFullPage(true);
		QPainter painter;
		painter.begin(&printer);
		render(&painter);
		painter.end();
	}
	else if (!fname.isEmpty()) {
		QPixmap pixmap(sceneRect().size().toSize());
		pixmap.fill(Qt::white);
		QPainter painter(&pixmap);
		painter.setRenderHint(QPainter::Antialiasing);
		render(&painter);
		if(!pixmap.save(fname)) {
			QMessageBox::warning(
				0,tr("error writing file"),
				tr("failed to save the workflow visualization to<br>"
					"<tt>%1</tt>"
					"Perhaps the image file format is not supported.")
					.arg(fname));
		}
	}
}

void NodeHandler::keyPressEvent(QKeyEvent* keyEvent) {
	switch(keyEvent->key()) {
	case Qt::Key_Right:
		_model->selectNext(false);
		break;

	case Qt::Key_Left:
		_model->selectNext(true);
		break;

	case Qt::Key_F2:
		if (_model && !_model->prefix().isEmpty() && _model->prefixValid()) {
			_model->renameNode(_model->prefix());
		}
		break;

	case Qt::Key_Delete:
		if (_model && !_model->prefix().isEmpty() && _model->prefixValid()) {
			_model->deleteNode(_model->prefix());
		}
		break;

	case Qt::Key_Escape:
		if (_model) {
			_model->setPrefix(QString());
		}

	default:
		break;
	}

	QGraphicsScene::keyPressEvent(keyEvent);
}

void NodeHandler::dragEnterEvent(QGraphicsSceneDragDropEvent* ev) {
	if (ev->mimeData()->hasFormat("application/x-qstandarditemmodeldatalist"))
		ev->accept();
	else
		QGraphicsScene::dragEnterEvent(ev);
}

void NodeHandler::dragMoveEvent(QGraphicsSceneDragDropEvent* ev) {
	if (ev->mimeData()->hasFormat("application/x-qstandarditemmodeldatalist"))
		ev->accept();
	else
		QGraphicsScene::dragMoveEvent(ev);
}

void NodeHandler::dropEvent(QGraphicsSceneDragDropEvent* ev) {
	QStandardItemModel m;
	bool res = m.dropMimeData(ev->mimeData(),Qt::CopyAction,0,0,QModelIndex());
	if(res && m.rowCount() == 1 && m.columnCount() == 1) {
		QString className = m.item(0)->text();
		_selectedNode = 0;
		QString instName = _model->addNode(className,false);

		// if plugin instanziation was successfull,
		// set its coordinates to the drop position
		if(!instName.isEmpty()) {
			_model->setPrefix(instName);
			// classic DropEvent->pos is always zero for this case
			QPointF pos = ev->scenePos();
			QString posString = QString("%0 %1").arg(pos.x()).arg(pos.y());
			// insert editor info
			_model->setValue(instName+".editorinfo", posString);
			loadFromModel();
			selectNode(_model->prefix());
			ev->accept();
			return ;
		}
	}

	QGraphicsScene::dropEvent(ev);
}

void NodeHandler::contextMenuEvent(QGraphicsSceneContextMenuEvent* ev) {
	QString nodeName, propName;

	// try to determine node and/or slot name
	QGraphicsItem* item = itemAt(ev->scenePos());
	if (item) {
		Node* node = dynamic_cast<Node*>(item);
		NodeProperty* prop = dynamic_cast<NodeProperty*>(item);

		if (prop) {
			node = dynamic_cast<Node*>(prop->parentItem());
			propName = prop->getFullName();
		}
		if (node) {
			nodeName = node->getInstanceName();
		}
	}

	if (nodeName.isEmpty()) {
		QGraphicsScene::contextMenuEvent(ev);
	}
	else {
		// node and prop may not be used after this point because model
		// changes let them disappear (disconnect/rename/etc.)
		ev->accept();

		// node context menu
		QMenu menu;
		QIcon disIco(":/icons/disconnect.png");
		QIcon renIco(":/icons/rename.png");
		QIcon delIco(":/icons/delete.png");

		// if context menu was called on a slot, provide option
		// concerning this slot at a prominent position (top)
		if (!propName.isEmpty()) {
			QAction* act = menu.addAction(
				disIco, tr("disconnect %1").arg(propName.section(".",1)));
			act->setData(propName);
			menu.addSeparator();
		}

		// common options
		QAction* delAct = menu.addAction(delIco, tr("delete"));
		QAction* renAct = menu.addAction(renIco, tr("rename"));
		QAction* disAct = menu.addAction(disIco, tr("disconnect all slots"));

		// submenu for slot disconnection (all slots are provided)
		QStringList slotNames;
		slotNames << _model->getInputs(nodeName)
				<< _model->getOutputs(nodeName);

		QMenu* dmenu = menu.addMenu(disIco, tr("disconnect slot"));
		foreach (const QString& cur, slotNames) {
			QAction* curAct =
				dmenu->addAction(disIco, cur);
			curAct->setData(QString("%1.%2").arg(nodeName).arg(cur));
		}

		// handle user selection
		QAction* selAct = menu.exec(ev->screenPos());
		if (selAct) {
			QString data = selAct->data().toString();
			if (!data.isEmpty()) {
				Q_ASSERT(data.contains(QRegExp("^[\\w_-]+\\.[\\w_-]+$")));
				_model->disconnectSlot(data);
			}
			else if (selAct == delAct) {
				_model->deleteNode(nodeName);
			}
			else if (selAct == renAct) {
				_model->renameNode(nodeName);
			}
			else if (selAct == disAct) {
				_model->disconnectAllSlots(nodeName);
			}
		}
	}
}

void NodeHandler::updateTooltip (QString comment) {
	// to prevent exceptions, ensure that _selectedNode is not empty
	if (_selectedNode && !_nodeMap.isEmpty()) {
		if (comment.isEmpty()) {
			_selectedNode->setToolTip("");
		} else {
			_selectedNode->setToolTip(
				"<b>" + tr("Comment:") + "</b><br/>" + comment);
		}
	}
}

#include "NodeHandler.moc"
