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
	selectNode(_model->prefix());
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
		QString cname = _model->getClass(nodes[ii]);

		Node* node = new Node(_model,name,10*ii,10*ii,this);
		node->setClassName(cname);
		_nodeMap.insert(name,node);

		if (_model->parameterFile().isSet(name+".editorinfo")) {
			QString pdata = _model->parameterFile().get(name+".editorinfo");
			float x = pdata.split(" ")[0].toFloat();
			float y = pdata.split(" ")[1].toFloat();
			node->setPos(x,y);
		}

		// set tooltip
		if (_model->parameterFile().isSet(name+".editorcomment")) {
			QString str = _model->parameterFile().get(name+".editorcomment");
			if (str.isEmpty()) {
				node->setToolTip("");
			} else {
				node->setToolTip("<b>" + tr("Comment:") + "</b><br/>" + str);
			}
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
			if (_model->parameterFile().isSet(curO)) {
				QStringList conns = _model->parameterFile().getList(curO);
				for (int kk=0; kk < conns.size(); kk++) {
					nodesout << name;
					slotout << outs[jj];
					slotin << conns[kk].section(".",-1,-1);
					nodesin << conns[kk].section(".",0,0);
				}
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

void NodeHandler::keyReleaseEvent(QKeyEvent* keyEvent) {
	switch(keyEvent->key()) {
	case Qt::Key_Right:
		_model->selectNext(false);
		break;

	case Qt::Key_Left:
		_model->selectNext(true);
		break;
	default:
		break;
	}
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

		//if plugin instanziation was successfull, set its coordinates to the drop position
		if(!instName.isEmpty()) {
			_model->setPrefix(instName);
			QPointF pos = ev->scenePos() ; //classic DropEvent->pos is always zero for this case
			QString posString = QString("%0 %1").arg(pos.x()).arg(pos.y());
			_model->setOnlyParams(false);
			// insert editor info
			_model->insertRow(_model->rowCount());
			_model->setData(_model->index(_model->rowCount()-1,0),"editorinfo");
			_model->setData(_model->index(_model->rowCount()-1,1),posString);
			_model->setOnlyParams(true) ;
			loadFromModel();
			selectNode(_model->prefix());
			ev->accept();
			return ;
		}
	}

	QGraphicsScene::dropEvent(ev);
}

void NodeHandler::updateTooltip(QString comment)
{
	// to prevent exceptions, ensure that _selectedNode is not empty
	if (_selectedNode && !_nodeMap.isEmpty()) {
		if (comment.isEmpty()) {
			_selectedNode->setToolTip("");
		} else {
			_selectedNode->setToolTip("<b>" + tr("Comment:") + "</b><br/>" + comment);
		}
	}
}

#include "NodeHandler.moc"
