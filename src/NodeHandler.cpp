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

#include "ConnectionSocket.h"
#include "ConnectionLine.h"
#include "NodeHandler.h"
#include "GraphModel.h"
#include "MetaData.h"
#include "FileManager.h"
#include "QParameterFile.h"

NodeHandler::NodeHandler(QObject* pp) :
		QGraphicsScene(pp),
		_cline(0),
		_addLine(false)
{
	_model = new GraphModel("", this, FileManager::instance().classesFile());
	connect(_model, SIGNAL(graphChanged()), this, SLOT(loadFromModel()));
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
		_deselectAllNodes();
		_selectedNode = np;
		_model->setPrefix(np->getInstanceName());
		np->setSelectedNode(true);
	} else {
		_deselectAllNodes();
	}
	ConnectionSocket* cs = dynamic_cast<ConnectionSocket*>(itm);
	if (cs != 0) {
		NodeProperty* prop = dynamic_cast<NodeProperty*>(cs->parentItem());
		Q_ASSERT(prop);
		
		_startProp = prop;
		delete _cline ; _cline = 0 ;
		_cline = new ConnectionLine(this);
		_cline->setStartPoint(
				ev->scenePos().x(),ev->scenePos().y());
		_cline->setEndPoint(
				ev->scenePos().x(),ev->scenePos().y());
		_addLine = true;
		update();
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
	QGraphicsItem* itm = itemAt(ev->scenePos());
	ConnectionSocket *cs = dynamic_cast<ConnectionSocket*>(itm);
	NodeProperty* prop = 0;

	delete _cline ;
	_cline = 0 ;

	if (_addLine && cs != 0) {
		prop = dynamic_cast<NodeProperty*>(cs->parentItem());
		if (prop != 0 && prop != _startProp) {
			try {
				_model->connectSlot(
					_startProp->getNode()->getInstanceName()+"."
						+_startProp->getName(),
					prop->getNode()->getInstanceName()+"."
						+prop->getName(),true);
			} catch (const std::runtime_error& err) {
				err.what() ;
			}
		}
	}
	_addLine = false;
			
	//update();
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
	const MetaData* mi = _model->metaInfo();
	QStringList nodes = _model->nodes();
	QStringList nodesout,nodesin;
	QStringList slotout,slotin;

	for (int ii=0;ii<nodes.size();ii++) {
		QString name = nodes[ii];
		QString cname = _model->getClass(nodes[ii]);

		Node* node = new Node(
				&_model->parameterFile(),name,10*ii,10*ii,this);
		node->setClassName(cname);
		_nodeMap.insert(name,node);

		if (_model->parameterFile().isSet(name+".editorinfo")) {
			QString pdata = _model->parameterFile().get(name+".editorinfo");
			float x = pdata.split(" ")[0].toFloat();
			float y = pdata.split(" ")[1].toFloat();
			node->setPos(x,y);
		}

		QStringList ins = mi->getInputs(cname);
		QStringList outs = mi->getOutputs(cname);
		for (int jj=0; jj < ins.size(); jj++) {
			node->addProperty(
					ins[jj], mi->getType(ins[jj],cname),
					true);
		}
		for (int jj=0; jj < outs.size(); jj++) {
			node->addProperty(
					outs[jj], mi->getType(outs[jj],cname),
					false);
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
		catch(...) {
			QMessageBox::warning(
					0, "connection error",
					"Failed to connect "+nodesout[ii]+"."+slotout[ii]+
					" to "+nodesin[ii]+"."+slotin[ii]+". "+
					 "Disconnected nodes in graphmodel.");
			_model->disconnectSlot(
					nodesout[ii]+"."+slotout[ii],nodesin[ii]+"."+slotin[ii]);
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

	Q_ASSERT(outp && inp) ;

	ConnectionLine* l = new ConnectionLine(this);
	l->setStartEndProp(outp,inp);
	outp->addConnection(l);
	inp->addConnection(l);
	outp->moveBy(0,0);
	inp->moveBy(0,0);
}

void NodeHandler::saveFlowchart() {
	QString fname = QFileDialog::getOpenFileName();
	if (!fname.isEmpty()) {
		QPixmap pixmap(width()+10,height()+10);
		QPainter painter(&pixmap);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.fillRect(0, 0, width()+10,height()+10, Qt::white);
		render(&painter);

		if (fname.endsWith(".png")) {
			pixmap.save(fname,"PNG");
		} else if(fname.endsWith(".jpg")) {
			pixmap.save(fname,"JPG",90);
		}
	}
}

void NodeHandler::keyReleaseEvent(QKeyEvent* keyEvent) {
	if(keyEvent->key() == Qt::Key_F12) {
		saveFlowchart();
	}

	if (keyEvent->key() == Qt::Key_Delete && _selectedNode != 0) {
		_model->deleteNode(_selectedNode->getInstanceName());
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
			loadFromModel() ;
			ev->accept();
			return ;
		}
	}

	QGraphicsScene::dropEvent(ev);
}

#include "NodeHandler.moc"
