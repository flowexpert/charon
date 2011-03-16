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
#include "TreeViewItem.h"
#include "GraphModel.h"
#include "MetaData.h"
#include "FileManager.h"

#include <QList>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QGraphicsView>
#include <QStandardItem>
#include <cmath>

NodeHandler::NodeHandler(
		QObject* p,QString classesFile) : QGraphicsScene(p),_model(0) {
	addLine = false;
	cline = NULL;
	scaleFactor = 1;
	TypeHandler::inizialize();
	if (classesFile.isEmpty())
		classesFile = FileManager::instance().classesFile();
	_model	= new GraphModel("", this, classesFile);
}

void NodeHandler::deselectAllNodes(){
	for(int i=0;i<this->items().size();i++) {
		Node *n = dynamic_cast<Node*>(this->items().at(i));
		if(n != 0) {
			n->setSelectedNode(false);
		}
	}
	this->selectedNode = 0;
}

void NodeHandler::wheelEvent(QGraphicsSceneWheelEvent* ev) {
	int f = ev->delta();
	qreal factor = std::max(0.7,1.0 + ((double)f * 0.001));
	QList<QGraphicsView*> vs(views());
	this->scaleFactor*=factor;
	if(vs.size() > 0) {
		vs.at(0)->scale(factor,factor);
	}
	QGraphicsScene::wheelEvent(ev);
}

void NodeHandler::mousePressEvent(QGraphicsSceneMouseEvent* ev) {
	deselectAllNodes();
	QGraphicsItem *itm = this->itemAt(ev->scenePos());
	Node *np = dynamic_cast<Node*>(itm);
	if(np != 0) {
		deselectAllNodes();
		selectedNode = np;
		_model->setPrefix(np->getName());
		np->setSelectedNode(true);
	} else {
		for(int i=0;i<this->items().size();i++)
			items().at(i)->setSelected(false);  // Deselect all
	}
	ConnectionSocket *cs = dynamic_cast<ConnectionSocket*>(itm);
	if(cs != 0)
	{
		NodeProperty *prop = dynamic_cast<NodeProperty*>(cs->parentItem());
		if(prop->canNewConnect()){
			if (prop->hasConnection()) {
				prop->removeAllConnections(_model);
			} else {
				startProp = prop;
				cline = new ConnectionLine(this);
				cline->setStartPoint(
							ev->scenePos().x(),ev->scenePos().y());
				cline->setEndPoint(
							ev->scenePos().x(),ev->scenePos().y());
				addLine = true;
			}
		}
	}else{
		QGraphicsScene::mousePressEvent(ev);
	}
	update();
}

void NodeHandler::mouseMoveEvent(QGraphicsSceneMouseEvent* ev)
{
/*
	QGraphicsItem *itm = this->itemAt(event->scenePos());
	// ODO move scene
	Node *np = dynamic_cast<Node*>(itm);
	if(np == 0 and event->button() == Qt::LeftButton){
		QRectF rect = this->sceneRect();
		QPointF mv = event->scenePos()-event->lastScenePos();
		cout<<mv.x()<<","<<mv.y()<<endl;
		rect.moveTo(mv);
		this->setSceneRect(rect);
	}
*/
	if(addLine) {
		if(this->startProp->getPropType()->iotype == PropType::OUT) {
			this->cline->setEndPoint(ev->scenePos().x(),ev->scenePos().y());
		} else {
			this->cline->setStartPoint(ev->scenePos().x(),ev->scenePos().y());
		}
	}
	else {
		QGraphicsScene::mouseMoveEvent(ev);
	}
	update();
}

GraphModel *NodeHandler::model(){
	return _model;
}

void NodeHandler::setModel(GraphModel* m){
	_model = m;
}


bool NodeHandler::load(QString fname){
	return _model->load(fname) && loadFromModel();
}

bool NodeHandler::loadFromModel(){
	const MetaData* mi = _model->metaInfo();
	QVector<QString> nodes = _model->nodes().toVector();

	QVector<QString> nodesOut,nodesIn;
	QVector<QString> slotOut,slotIn;

	for(int ii=0;ii<nodes.size();ii++){
		QString name = nodes[ii];
		std::string cname = _model->getClass(nodes[ii].toStdString());

		Node *node = new Node(name,10*ii,10*ii,this);
		_model->setPrefix("");
		_model->setOnlyParams(false);
		for(int jj=0; jj<_model->rowCount(); jj++){
			if (_model->data(
					_model->index(jj,0)).toString() == name+".editorinfo") {
			QString pdata = _model->data(_model->index(jj,1)).toString();
			float x = pdata.split(" ").at(0).toFloat();
			float y = pdata.split(" ").at(1).toFloat();
			node->setPos(x,y);
			}
		}
		_model->setOnlyParams(true);

		std::vector<std::string> ins = mi->getInputs(cname);
		for(unsigned int jj=0; jj<ins.size(); jj++){
			node->addProperty(
					QString::fromStdString(ins[jj]),
					QString::fromStdString(mi->getType(ins[jj],cname)),
					PropType::IN);
		}
		std::vector<std::string> outs = mi->getOutputs(cname);
		for(unsigned int jj=0; jj<outs.size(); jj++){
			node->addProperty(
					QString::fromStdString(outs[jj]),
					QString::fromStdString(mi->getType(outs[jj],cname)),
					PropType::OUT);
			std::vector<std::string> conns =
					_model->parameterFile().getList<std::string>(
						name.toStdString()+"."+outs[jj]);
			for(unsigned int j=0;j<conns.size();j++){
				slotOut.push_back(QString::fromStdString(outs[jj]));
				slotIn.push_back(
					QString::fromStdString(conns[j]).remove(
						QRegExp("*.",Qt::CaseSensitive,QRegExp::Wildcard)));
				nodesIn.push_back(
					QString::fromStdString(conns[j]).remove(
						QRegExp(".*",Qt::CaseSensitive,QRegExp::Wildcard)));
				nodesOut.push_back(name);
			}
		 }
		node->moveBy(0,0);
	}
	update();

	bool ok = true;
	for (int i=0;i<slotOut.size();i++) {
		ok = this->connectNodes(nodesOut[i],slotOut[i],nodesIn[i],slotIn[i])
				&& ok;
	}
	update();

	deselectAllNodes();
	return ok;
}

bool NodeHandler::connectNodes(
		QString node0, QString prop0, QString node1, QString prop1) {
	Node *out,*in;
	for(int i=0; i < items().size(); i++){
		Node* n = dynamic_cast<Node*>(items().at(i));
		if(n){
			if(n->getName() == node0) out = n;
			if(n->getName() == node1) in = n;
		}
	}
	if(!(out && in)){
		QMessageBox::warning(
			0, "connection error",
			QString("failed to connect: %1.%2 to %3.%4")
					.arg(node0).arg(prop0).arg(node1).arg(prop1));
		return false;
	}
	NodeProperty *inp,*outp;
	for(int i=0;i<in->getProperties().size();i++) {
		if(in->getProperties().at(i)->getName() == prop1) {
			inp = in->getProperties().at(i);
			break;
		}
	}
	for(int i=0;i<out->getProperties().size();i++){
		if(out->getProperties().at(i)->getName() == prop0) {
			outp = out->getProperties().at(i);
			break;
		}
	}
	if (inp == 0 || outp == 0) {
		QMessageBox::warning(
			0, "connection error",
			QString("failed to connect: %1.%2 to %3.%4")
					.arg(node0).arg(prop0).arg(node1).arg(prop1));
		return false;
	}

	ConnectionLine *l = new ConnectionLine(this);
	l->setStartEndProp(outp,inp);
	outp->addConnection(l);
	inp->addConnection(l);
	outp->moveBy(0,0);
	inp->moveBy(0,0);
	return true;
}

void NodeHandler::keyReleaseEvent (QKeyEvent* keyEvent)
{
	if(keyEvent->key() == Qt::Key_F12){
		QString fname = QFileDialog::getOpenFileName();
		if(!fname.isEmpty()){
		QPixmap pixmap(this->width()+10,this->height()+10);
		QPainter painter(&pixmap);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.fillRect(0, 0, this->width()+10,this->height()+10, Qt::white);
		this->render(&painter);

		if(fname.endsWith(".png")){
			pixmap.save(fname,"PNG");
		}else if(fname.endsWith(".jpg")){
			pixmap.save(fname,"JPG",90);
		}
		}
	}

	if(keyEvent->key() == Qt::Key_Delete && this->selectedNode != 0){
		this->deleteNode(this->selectedNode);
	}
}

void NodeHandler::deleteNode(Node *node) {
	if(_model->deleteNode(node->getName())) {
		clear();
		loadFromModel();
	}
}

void NodeHandler::mouseReleaseEvent(QGraphicsSceneMouseEvent* ev) {
	QGraphicsItem *itm = this->itemAt(ev->scenePos());
	ConnectionSocket *cs = dynamic_cast<ConnectionSocket*>(itm);
	NodeProperty *prop = 0;
	if(addLine && cs != 0) {
		prop = dynamic_cast<NodeProperty*>(cs->parentItem());
		if(prop != 0 && prop != startProp) {
			if(startProp->getIOType()==PropType::IN) {
				//swap buffers
				NodeProperty *b = startProp;
				startProp = prop;
				prop = b;
			}
			try {
				_model->connected(
					startProp->getNode()->getName()+"."+startProp->getName(),
					prop->getNode()->getName()+"."+prop->getName());
				cline->setStartEndProp(startProp,prop);
				startProp->addConnection(cline);
				prop->addConnection(cline);
				startProp->moveBy(0,0);
				prop->moveBy(0,0);
				cline = NULL;
				_model->connectSlot(
					startProp->getNode()->getName()+"."+startProp->getName(),
					prop->getNode()->getName()+"."+prop->getName());
			} catch (std::runtime_error){
				this->removeItem(cline);
				cline = NULL;
			}
			addLine = false;
		} else {
			this->removeItem(cline);
			cline = NULL;
			addLine = false;
		}
	} else if(addLine) {
		this->removeItem(cline);
		cline = NULL;
		addLine = false;
	}

	QGraphicsScene::mouseReleaseEvent(ev);
	update();
}

void NodeHandler::addNode(QString name, QPointF pos)
{
	new Node(name,pos.x(),pos.y(),this);
}



void NodeHandler::dragEnterEvent(QGraphicsSceneDragDropEvent*){
}

void NodeHandler::dropEvent(QGraphicsSceneDragDropEvent* ev){

	NodeTreeView *ntv = (NodeTreeView*)(ev->source());
	if(ntv != 0){
		Node *node = new Node(
			ntv->getSelectedItem()->getNode()->getName(),
			ev->scenePos().x(),ev->scenePos().y(),this);

		QVector<NodeProperty*> props =
			ntv->getSelectedItem()->getNode()->getProperties();
		for(int i=0;i<props.size();i++){
			node->addProperty(
				props[i]->getName(),
				props[i]->getPropType()->ptype->getTypeName(),
				props[i]->getIOType());
		}

		this->deselectAllNodes();
		node->setSelectedNode(true);
		this->selectedNode = node;

		QString newname = _model->addNode(node->getName());
		if(!(newname == "")) node->setName(newname);
		else this->removeItem(node);
		_model->setPrefix(node->getName());
	}
}

void NodeHandler::dragMoveEvent(QGraphicsSceneDragDropEvent*){
}

NodeHandler::~NodeHandler() {
}

#include "NodeHandler.moc"
