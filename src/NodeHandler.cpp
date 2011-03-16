/*	Copyright (C) 2011 Jonathan Wuest

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
/** @file   NodeHandler.cpp
 *  @brief  Implementation of class NodeHandler
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
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

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include<QList>
#include <QFileDialog>
#include<QFile>
#include<QTextStream>
#include<QGraphicsView>
#include<QStandardItem>
#include<iostream>
#include<math.h>

#include "NodeHandler.moc"

using namespace std;
NodeHandler::NodeHandler(QObject *parent,QString classesFile) : QGraphicsScene(parent),_model(0) {
	addLine = false;
	this->cline = NULL;
	scaleFactor = 1;
	TypeHandler::inizialize();
	if (classesFile.isEmpty())
		classesFile = FileManager::instance().classesFile();
	_model	= new GraphModel("", this, classesFile);
}

void NodeHandler::deselectAllNodes(){
	for(int i=0;i<this->items().size();i++)
		{
			Node *n = dynamic_cast<Node*>(this->items().at(i));
			if(n != 0)
			{
				n->setSelectedNode(false);
			}
		}
	this->selectedNode = 0;
}

void NodeHandler::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	int f = event->delta();
	qreal factor = max(0.7,1.0 + ((double)f * 0.001));
	QList<QGraphicsView*> views(this->views());
	this->scaleFactor*=factor;
	if(views.size()>0)
	{
		views.at(0)->scale(factor,factor);
	}
	QGraphicsScene::wheelEvent(event);
}

void NodeHandler::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	this->deselectAllNodes();
	QGraphicsItem *itm = this->itemAt(event->scenePos());
	Node *np = dynamic_cast<Node*>(itm);
	if(np != 0)
	{
		this->deselectAllNodes();
		this->selectedNode = np;
		_model->setPrefix(np->getName());
		np->setSelectedNode(true);
	}else{
		for(int i=0;i<this->items().size();i++) this->items().at(i)->setSelected(false);  //Deselect all
	}
	ConnectionSocket *cs = dynamic_cast<ConnectionSocket*>(itm);
	if(cs != 0)
	{
		NodeProperty *prop = dynamic_cast<NodeProperty*>(cs->parentItem());
		if(prop->canNewConnect()){
			if(prop->hasConnection()){
				prop->removeAllConnections(_model);
			}else{
				this->startProp = prop;
				this->cline = new ConnectionLine(this);
				this->cline->setStartPoint(event->scenePos().x(),event->scenePos().y());
				this->cline->setEndPoint(event->scenePos().x(),event->scenePos().y());
				this->addLine = true;
			}
		}
	}else{
		QGraphicsScene::mousePressEvent(event);
	}
	update();
}

void NodeHandler::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	/*QGraphicsItem *itm = this->itemAt(event->scenePos());				//TODO move scene
	Node *np = dynamic_cast<Node*>(itm);
	if(np == 0 and event->button() == Qt::LeftButton){
		QRectF rect = this->sceneRect();
		QPointF mv = event->scenePos()-event->lastScenePos();
		cout<<mv.x()<<","<<mv.y()<<endl;
		rect.moveTo(mv);
		this->setSceneRect(rect);
	}*/
	if(addLine)
	{
		if(this->startProp->getPropType()->iotype == PropType::OUT)
		{
			this->cline->setEndPoint(event->scenePos().x(),event->scenePos().y());
		}else{
			this->cline->setStartPoint(event->scenePos().x(),event->scenePos().y());
		}
	}
	else{
		QGraphicsScene::mouseMoveEvent(event);
	}
	update();
}

GraphModel *NodeHandler::model(){
	return this->_model;
}

void NodeHandler::setModel(GraphModel *model){
	this->_model = model;
}


void NodeHandler::load(QString fname){
	_model->load(fname);
	this->loadFromModel();
}

void NodeHandler::loadFromModel(){
	const MetaData *mi = _model->metaInfo();
	QVector<QString> nodes = _model->nodes().toVector();

	QVector<QString> nodesout,nodesin;
	QVector<QString> slotout,slotin;

	for(int i=0;i<nodes.size();i++){
		QString name = nodes[i];
		string cname = _model->getClass(nodes[i].toStdString());

		Node *node = new Node(name,10*i,10*i,this);
		_model->setPrefix("");
		_model->setOnlyParams(false);
		for(int i=0;i<_model->rowCount();i++){
		    if(_model->data(_model->index(i,0)).toString() == name+".editorinfo"){
			QString pdata = _model->data(_model->index(i,1)).toString();
			float x = pdata.split(" ").at(0).toFloat();
			float y = pdata.split(" ").at(1).toFloat();
			node->setPos(x,y);
		    }
		}
		_model->setOnlyParams(true);

		vector<string> ins = mi->getInputs(cname);
		for(unsigned int i=0;i<ins.size();i++){
			node->addProperty(QString::fromStdString(ins[i]),
							  QString::fromStdString(mi->getType(ins[i],cname)),
							  PropType::IN);
		}
		vector<string> outs = mi->getOutputs(cname);
		for(unsigned int i=0;i<outs.size();i++){
			node->addProperty(QString::fromStdString(outs[i]),
							  QString::fromStdString(mi->getType(outs[i],cname)),
							  PropType::OUT);
			vector<string> conns = _model->parameterFile().getList<string>(name.toStdString()+"."+outs[i]);
			for(unsigned int j=0;j<conns.size();j++){
			    slotout.push_back(QString::fromStdString(outs[i]));
			    slotin.push_back(QString::fromStdString(conns[j]).remove(QRegExp("*.",Qt::CaseSensitive,QRegExp::Wildcard)));
			    nodesin.push_back(QString::fromStdString(conns[j]).remove(QRegExp(".*",Qt::CaseSensitive,QRegExp::Wildcard)));
			    nodesout.push_back(name);
			}
		 }
		node->moveBy(0,0);
	}
	update();
	for(int i=0;i<slotout.size();i++){
	    this->connectNodes(nodesout[i],slotout[i],nodesin[i],slotin[i]);

	}
	update();
	deselectAllNodes();
}

void NodeHandler::connectNodes(QString node0, QString prop0, QString node1, QString prop1){
    Node *out,*in;
    for(int i=0;i<this->items().size();i++){
	Node *n = dynamic_cast<Node*>(items().at(i));
	if(n){
	    if(n->getName() == node0) out = n;
	    if(n->getName() == node1) in = n;
	}
    }
    if(!(out && in)){
	cout<<"failed to connect: "<<node0.toStdString()<<"."<<prop0.toStdString();
	cout<<" to "<<node1.toStdString()<<"."<<prop1.toStdString()<<endl;
	return;
    }
    NodeProperty *inp,*outp;
    for(int i=0;i<in->getProperties().size();i++){
	if(in->getProperties().at(i)->getName() == prop1){
	    inp = in->getProperties().at(i);
	    break;
	}
    }
    for(int i=0;i<out->getProperties().size();i++){
	if(out->getProperties().at(i)->getName() == prop0){
	    outp = out->getProperties().at(i);
	    break;
	}
    }
    if(inp == 0 || outp == 0){
	cout<<"failed to connect: "<<node0.toStdString()<<"."<<prop0.toStdString();
	cout<<" to "<<node1.toStdString()<<"."<<prop1.toStdString()<<endl;
	return;
    }

    ConnectionLine *l = new ConnectionLine(this);
    l->setStartEndProp(outp,inp);
    outp->addConnection(l);
    inp->addConnection(l);
    outp->moveBy(0,0);
    inp->moveBy(0,0);
}

void NodeHandler::toXMLFile(QString filename)	//unused
{
	QDomDocument doc("Workflow");
	QDomElement root = doc.createElement("Nodes");
	QDomElement lroot = doc.createElement("Edges");
	int nodeID=0;
	for(int i=0;i<this->items().size();i++)
	{
		Node *n = dynamic_cast<Node*>(this->items().at(i));
		if(n != 0)
		{
			QDomElement nodeElement = doc.createElement("Node");
						nodeElement.setAttribute("name",n->getName());
			nodeElement.setAttribute("config_file",n->getConfigFileName());
			nodeElement.setAttribute("position_x",n->scenePos().x());
			nodeElement.setAttribute("position_y",n->scenePos().y());
			nodeElement.setAttribute("id",n->getId());
			nodeID++;
			root.appendChild(nodeElement);
		}else{
			ConnectionLine *l = dynamic_cast<ConnectionLine*>(this->items().at(i));
			if(l != 0){
				QDomElement line = doc.createElement("Edge");
				line.setAttribute("node0",l->getStartProp()->getNode()->getId());
				line.setAttribute("prop0",l->getStartProp()->getNr());
				line.setAttribute("node1",l->getEndProp()->getNode()->getId());
				line.setAttribute("prop1",l->getEndProp()->getNr());
				lroot.appendChild(line);
			}
		}
	}
	doc.appendChild(root);
	doc.appendChild(lroot);
	QDomElement editProps = doc.createElement("Editor");
	if(this->views().size()>0)
	{
		editProps.setAttribute("center_x",this->views().at(0)->sceneRect().center().x());
		editProps.setAttribute("center_y",this->views().at(0)->sceneRect().center().y());
		editProps.setAttribute("zoom",this->scaleFactor);
	}

	doc.appendChild(editProps);
	QFile file(filename);
	if( !file.open( QIODevice::WriteOnly ) )
	return ;

	QTextStream ts( &file );
	ts << doc.toString();

	file.close();
	cout<<"   done"<<endl;
}

void NodeHandler::keyReleaseEvent (QKeyEvent * keyEvent )
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

void NodeHandler::deleteNode(Node *node){
	for(int i=0;i<this->items().size();i++)
		{
			Node *n = dynamic_cast<Node*>(this->items().at(i));
			if(n != 0)
			{
				if(node->getId() == n->getId()){
					n->remove();
					cout<<"deleting:"<<n->getName().toStdString()<<endl;
					this->removeItem(this->items().at(i));
					update();
					break;
				}
			}
		}
	_model->deleteNode(node->getName());
}

void NodeHandler::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem *itm = this->itemAt(event->scenePos());
	ConnectionSocket *cs = dynamic_cast<ConnectionSocket*>(itm);
	NodeProperty *prop = 0;
	if(addLine && cs != 0)
	{
		prop = dynamic_cast<NodeProperty*>(cs->parentItem());
		if(prop != 0 && prop != startProp)
		{
			if(startProp->getIOType()==PropType::IN) //swap buffers
			{
				NodeProperty *b = startProp;
				startProp = prop;
				prop = b;
			}
			try{
				_model->connected(startProp->getNode()->getName()+"."+startProp->getName(),
									prop->getNode()->getName()+"."+prop->getName());
				//this->cline->setEndPoint(event->scenePos().x(),event->scenePos().y());
				cline->setStartEndProp(startProp,prop);
				startProp->addConnection(cline);
				prop->addConnection(cline);
				startProp->moveBy(0,0);
				prop->moveBy(0,0);
				cline = NULL;
				_model->connectSlot(
						startProp->getNode()->getName()+"."+startProp->getName(),
						prop->getNode()->getName()+"."+prop->getName());
			}catch(std::runtime_error){
				this->removeItem(cline);
				cline = NULL;
			}
			addLine = false;
		}else{
			this->removeItem(cline);
			cline = NULL;
			addLine = false;
		}
	}else if(addLine){
		this->removeItem(cline);
		cline = NULL;
		addLine = false;
	}

	QGraphicsScene::mouseReleaseEvent(event);
	update();
}

void NodeHandler::addNode(QString name, QPointF pos)
{
	new Node(name,pos.x(),pos.y(),this);
}



void NodeHandler::dragEnterEvent(QGraphicsSceneDragDropEvent*){
}

void NodeHandler::dropEvent(QGraphicsSceneDragDropEvent* event){

	NodeTreeView *ntv = (NodeTreeView*)(event->source());
	if(ntv != 0){
		Node *node = new Node(ntv->getSelectedItem()->getNode()->getName(), event->scenePos().x(),event->scenePos().y(),this);

		QVector<NodeProperty*> props = ntv->getSelectedItem()->getNode()->getProperties();
		for(int i=0;i<props.size();i++){
			node->addProperty(props[i]->getName(),
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

void NodeHandler::dragMoveEvent(QGraphicsSceneDragDropEvent*){}

NodeHandler::~NodeHandler() {
	// TODO Auto-generated destructor stub
}
