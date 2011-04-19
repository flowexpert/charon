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
#include "ConnectionSocket.h"
#include "NodeProperty.h"
#include "ConnectionLine.h"
#include "NodeHandler.h"
#include "NodeTreeView.h"
#include "TypeHandler.h"
#include "GraphModel.h"
#include "MetaData.h"
#include "FileManager.h"

#include <QList>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QGraphicsView>
#include <QStandardItem>

NodeHandler::NodeHandler(QObject* pp, QString classesFile) :
		QGraphicsScene(pp),_model(0) {
	_addLine = false;
	_cline = NULL;
	_scaleFactor = 1;

	if (classesFile.isEmpty())
		classesFile = FileManager::instance().classesFile();
	_model	= new GraphModel("", this, classesFile);
}

void NodeHandler::_deselectAllNodes() {
	for(int i=0; i < items().size();i++) {
		Node* n = qgraphicsitem_cast<Node*>(items().at(i));
		if(n != 0) {
			n->setSelectedNode(false);
		}
	}
	_selectedNode = 0;
}

void NodeHandler::wheelEvent(QGraphicsSceneWheelEvent* ev) {
	int f = ev->delta();
	qreal factor = std::max(0.7,1.0 + ((double)f * 0.001));
	QList<QGraphicsView*> vs (views());
	_scaleFactor *= factor;
	if (vs.size() > 0) {
		vs.at(0)->scale(factor,factor);
	}
	QGraphicsScene::wheelEvent(ev);
}

void NodeHandler::mousePressEvent(QGraphicsSceneMouseEvent* ev) {
	_deselectAllNodes();
	QGraphicsItem *itm = itemAt(ev->scenePos());
	Node* np = qgraphicsitem_cast<Node*>(itm);
	if (np != 0) {
		_deselectAllNodes();
		_selectedNode = np;
		_model->setPrefix(np->getName());
		np->setSelectedNode(true);
	} else {
		for (int i=0; i < items().size(); i++)
			items().at(i)->setSelected(false); // Deselect all
	}
	ConnectionSocket* cs = qgraphicsitem_cast<ConnectionSocket*>(itm);
	if (cs != 0) {
		NodeProperty* prop = qgraphicsitem_cast<NodeProperty*>(
				cs->parentItem());
		if (prop->canNewConnect()) {
			if (prop->hasConnection()) {
				prop->removeAllConnections(_model);
			} else {
				_startProp = prop;
				_cline = new ConnectionLine(this);
				_cline->setStartPoint(
						ev->scenePos().x(),ev->scenePos().y());
				_cline->setEndPoint(
						ev->scenePos().x(),ev->scenePos().y());
				_addLine = true;
			}
		}
	} else {
		QGraphicsScene::mousePressEvent(ev);
	}
	update();
}

void NodeHandler::mouseMoveEvent(QGraphicsSceneMouseEvent* ev) {
	if (_addLine) {
		if(_startProp->getPropType()->iotype == PropType::OUT) {
			_cline->setEndPoint(ev->scenePos().x(),ev->scenePos().y());
		} else {
			_cline->setStartPoint(ev->scenePos().x(),ev->scenePos().y());
		}
	}
	else {
		QGraphicsScene::mouseMoveEvent(ev);
	}
	update();
}

GraphModel* NodeHandler::model() {
	return _model;
}

void NodeHandler::setModel(GraphModel* m) {
	_model = m;
}

bool NodeHandler::load(QString fname) {
	bool loaded = _model->load(fname);
	loadFromModel();
	return loaded;
}

void NodeHandler::loadFromModel() {
	const MetaData* mi = _model->metaInfo();
	QVector<QString> nodes = _model->nodes().toVector();
	QVector<QString> nodesout,nodesin;
	QVector<QString> slotout,slotin;
	_model->setPrefix("");

	for (int ii=0;ii<nodes.size();ii++) {
		QString name = nodes[ii];
		std::string cname = _model->getClass(nodes[ii].toStdString());

		Node* node = new Node(name,10*ii,10*ii,this);
		_model->setPrefix("");
		_model->setOnlyParams(false);
		for(int j=0;j<_model->rowCount();j++){
			if (_model->data(_model->index(j,0)).toString()
					== name+".editorinfo") {
				QString pdata = _model->data(_model->index(j,1)).toString();
				float x = pdata.split(" ").at(0).toFloat();
				float y = pdata.split(" ").at(1).toFloat();
				node->setPos(x,y);
			}
			if(_model->data(_model->index(j,0)).toString() == name+".type"){
				QString modname = _model->data(_model->index(j,1)).toString();
				node->setModulName(modname);
			}
		}
		_model->setOnlyParams(true);

		std::vector<std::string> ins = mi->getInputs(cname);
		std::vector<std::string> outs = mi->getOutputs(cname);
		std::vector<std::string> params = mi->getParameters(cname);

		for (size_t jj=0; jj < ins.size(); jj++) {
			node->addProperty(
					QString::fromStdString(ins[jj]),
					QString::fromStdString(mi->getType(ins[jj],cname)),
					PropType::IN);
		}
		for (size_t jj=0; jj < outs.size(); jj++) {
			node->addProperty(
					QString::fromStdString(outs[jj]),
					QString::fromStdString(mi->getType(outs[jj],cname)),
					PropType::OUT);
		}
		for (size_t jj=0; jj < params.size(); jj++) {
			node->addProperty(
					QString::fromStdString(params[jj]),
					QString::fromStdString(mi->getType(params[jj],cname)),
					PropType::NONE);
		}

		for (size_t jj=0; jj < outs.size(); jj++) {
			std::string curO = name.toStdString()+"."+outs[jj];
			if (_model->parameterFile().isSet(curO)) {
				std::vector<std::string> conns =
						_model->parameterFile().getList<std::string>(curO);
				for (size_t kk=0; kk < conns.size(); kk++) {
					slotout.push_back(QString::fromStdString(outs[jj]));
					slotin.push_back(QString::fromStdString(conns[kk]).remove(
						QRegExp("*.",Qt::CaseSensitive,QRegExp::Wildcard)));
					nodesin.push_back(QString::fromStdString(conns[kk]).remove(
						QRegExp(".*",Qt::CaseSensitive,QRegExp::Wildcard)));
					nodesout.push_back(name);
				}
			}
		}
		node->moveBy(0,0);
	}

	update();
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
	_deselectAllNodes();
}

void NodeHandler::connectNodes(
		QString node0, QString prop0, QString node1, QString prop1) {
	Node *out, *in;
	for (int i=0; i < items().size(); i++) {
		Node *n = dynamic_cast<Node*>(items().at(i));
		if (n) {
			if(n->getName() == node0) out = n;
			if(n->getName() == node1) in = n;
		}
	}
	if (!(out && in)) {
		QMessageBox::warning(
				0, "connection error",
				QString("failed to connect: %1.%2 to %3.%4")
					.arg(node0,prop0,node1,prop1));
		return;
	}

	NodeProperty *inp,*outp;
	for (int i=0;i<in->getProperties().size();i++) {
		if (in->getProperties().at(i)->getName() == prop1) {
			inp = in->getProperties().at(i);
			break;
		}
	}
	for(int i=0;i<out->getProperties().size();i++) {
		if (out->getProperties().at(i)->getName() == prop0) {
			outp = out->getProperties().at(i);
			break;
		}
	}

	ConnectionLine *l = new ConnectionLine(this);
	l->setStartEndProp(outp,inp);
	outp->addConnection(l);
	inp->addConnection(l);
	outp->moveBy(0,0);
	inp->moveBy(0,0);
}


void NodeHandler::keyReleaseEvent(QKeyEvent* keyEvent) {
	if(keyEvent->key() == Qt::Key_F12) {
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

	if (keyEvent->key() == Qt::Key_Delete && _selectedNode != 0) {
		deleteNode(_selectedNode);
	}
}

void NodeHandler::deleteNode(Node* node) {
	if(_model->deleteNode(node->getName())) {
		clear();
		loadFromModel();
	}
}

void NodeHandler::mouseReleaseEvent(QGraphicsSceneMouseEvent* ev) {
	QGraphicsItem* itm = itemAt(ev->scenePos());
	ConnectionSocket *cs = qgraphicsitem_cast<ConnectionSocket*>(itm);
	NodeProperty* prop = 0;
	if (_addLine && cs != 0) {
		prop = qgraphicsitem_cast<NodeProperty*>(cs->parentItem());
		if (prop != 0 && prop != _startProp) {
			if (_startProp->getIOType()==PropType::IN) { //swap buffers
				NodeProperty *b = _startProp;
				_startProp = prop;
				prop = b;
			}
			try {
				_model->connected(
					_startProp->getNode()->getName()+"."+_startProp->getName(),
					prop->getNode()->getName()+"."+prop->getName());
				_cline->setStartEndProp(_startProp,prop);
				_startProp->addConnection(_cline);
				prop->addConnection(_cline);
				_startProp->moveBy(0,0);
				prop->moveBy(0,0);
				_cline = NULL;
				_model->connectSlot(
					_startProp->getNode()->getName()+"."+_startProp->getName(),
					prop->getNode()->getName()+"."+prop->getName());
			} catch (std::runtime_error) {
				removeItem(_cline);
				_cline = NULL;
			}
			_addLine = false;
		} else {
			removeItem(_cline);
			_cline = NULL;
			_addLine = false;
		}
	} else if(_addLine) {
		removeItem(_cline);
		_cline = NULL;
		_addLine = false;
	}

	QGraphicsScene::mouseReleaseEvent(ev);
	update();
}

void NodeHandler::addNode(QString name, QPointF pos) {
	new Node(name,pos.x(),pos.y(),this);
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
		QString instName = _model->addNode(className);
		if(!instName.isEmpty()) {
			_model->setPrefix(instName);
			loadFromModel();
			ev->accept();
			return;
		}
	}
	QGraphicsScene::dropEvent(ev);
}

#include "NodeHandler.moc"
