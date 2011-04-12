/*	Copyright (C) 2011 Jonathan Wuest
 * @file   NodeHandler.cpp
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

#include<QList>
#include <QFileDialog>
#include<QFile>
#include<QTextStream>
#include<QErrorMessage>
#include<QGraphicsView>
#include<QStandardItem>
#include<iostream>
#include<math.h>

#include "NodeHandler.moc"

using namespace std;
NodeHandler::NodeHandler(QObject *parent,QString classesFile) : QGraphicsScene(parent),_model(0) {
	_addLine = false;
	this->_cline = NULL;
	_scaleFactor = 1;

	TypeHandler::inizialize();
	if (classesFile.isEmpty())
		classesFile = FileManager::instance().classesFile();
	_model	= new GraphModel("", this, classesFile);
}

void NodeHandler::_deselectAllNodes(){
	for(int i=0;i<this->items().size();i++)
		{
			Node *n = dynamic_cast<Node*>(this->items().at(i));
			if(n != 0)
			{
				n->setSelectedNode(false);
			}
		}
	this->_selectedNode = 0;
}

void NodeHandler::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	int f = event->delta();
	qreal factor = max(0.7,1.0 + ((double)f * 0.001));
	QList<QGraphicsView*> views(this->views());
	this->_scaleFactor*=factor;
	if(views.size()>0)
	{
		views.at(0)->scale(factor,factor);
	}
	QGraphicsScene::wheelEvent(event);
}



void NodeHandler::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	this->_deselectAllNodes();
	QGraphicsItem *itm = this->itemAt(event->scenePos());
	Node *np = dynamic_cast<Node*>(itm);
	if(np != 0)
	{
		this->_deselectAllNodes();
		this->_selectedNode = np;
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
				this->_startProp = prop;
				this->_cline = new ConnectionLine(this);
				this->_cline->setStartPoint(event->scenePos().x(),event->scenePos().y());
				this->_cline->setEndPoint(event->scenePos().x(),event->scenePos().y());
				this->_addLine = true;
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
	if(_addLine)
	{
		if(this->_startProp->getPropType()->iotype == PropType::OUT)
		{
			this->_cline->setEndPoint(event->scenePos().x(),event->scenePos().y());
		}else{
			this->_cline->setStartPoint(event->scenePos().x(),event->scenePos().y());
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


bool NodeHandler::load(QString fname){
	bool loaded = _model->load(fname);
	cout<<"model loaded: "<<loaded<<endl;
	this->loadFromModel();
	return loaded;
}

void NodeHandler::loadFromModel(){
	const MetaData *mi = _model->metaInfo();
	QVector<QString> nodes = _model->nodes().toVector();

	QVector<QString> nodesout,nodesin;
	QVector<QString> slotout,slotin;
	//cout<<"loading nodes"<<endl;
	_model->setPrefix("");



	for(int i=0;i<nodes.size();i++){
		//cout<<"loading node"<<endl;
		QString name = nodes[i];
		string cname = _model->getClass(nodes[i].toStdString());

		Node *node = new Node(name,10*i,10*i,this);
		_model->setPrefix("");
		_model->setOnlyParams(false);
		for(int j=0;j<_model->rowCount();j++){
			if(_model->data(_model->index(j,0)).toString() == name+".editorinfo"){
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

		//cout<<"adding parameters"<<endl;

		vector<string> ins = mi->getInputs(cname);
		vector<string> outs = mi->getOutputs(cname);
		vector<string> params = mi->getParameters(cname);

		for(unsigned int j=0;j<ins.size();j++){
			node->addProperty(QString::fromStdString(ins[j]),QString::fromStdString(mi->getType(ins[j],cname)),PropType::IN);
		}
		for(unsigned int j=0;j<outs.size();j++){
			node->addProperty(QString::fromStdString(outs[j]),QString::fromStdString(mi->getType(outs[j],cname)),PropType::OUT);
		}
		for(unsigned int j=0;j<params.size();j++){
			node->addProperty(QString::fromStdString(params[j]),QString::fromStdString(mi->getType(params[j],cname)),PropType::NONE);
		}

		for(unsigned int i=0;i<outs.size();i++){
			if(_model->parameterFile().isSet(name.toStdString()+"."+outs[i])){
				vector<string> conns = _model->parameterFile().getList<string>(name.toStdString()+"."+outs[i]);
				for(unsigned int j=0;j<conns.size();j++){
					slotout.push_back(QString::fromStdString(outs[i]));
					slotin.push_back(QString::fromStdString(conns[j]).remove(QRegExp("*.",Qt::CaseSensitive,QRegExp::Wildcard)));
					nodesin.push_back(QString::fromStdString(conns[j]).remove(QRegExp(".*",Qt::CaseSensitive,QRegExp::Wildcard)));
					nodesout.push_back(name);
				}
			}
		 }
		node->moveBy(0,0);
	}
	//cout<<"connecting nodes!"<<endl;
	update();
	for(int i=0;i<slotout.size();i++){
		try{
		this->connectNodes(nodesout[i],slotout[i],nodesin[i],slotin[i]);
		}catch(...){
		QErrorMessage errorMessage;
		errorMessage.showMessage("Failed to connect "+nodesout[i]+"."+slotout[i]+
					 " to "+nodesin[i]+"."+slotin[i]+". "+
					 "Disconnected nodes in graphmodel.");
		errorMessage.exec();
		_model->disconnectSlot(nodesout[i]+"."+slotout[i],nodesin[i]+"."+slotin[i]);
		}

	}
	update();
	_deselectAllNodes();
	//cout<<"all done"<<endl;
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

	if(keyEvent->key() == Qt::Key_Delete && this->_selectedNode != 0){
		this->deleteNode(this->_selectedNode);
	}
}

void NodeHandler::deleteNode(Node *node){
	if(_model->deleteNode(node->getName())){
	this->clear();
	this->loadFromModel();
	}
}

void NodeHandler::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem *itm = this->itemAt(event->scenePos());
	ConnectionSocket *cs = dynamic_cast<ConnectionSocket*>(itm);
	NodeProperty *prop = 0;
	if(_addLine && cs != 0)
	{
		prop = dynamic_cast<NodeProperty*>(cs->parentItem());
		if(prop != 0 && prop != _startProp)
		{
			if(_startProp->getIOType()==PropType::IN) //swap buffers
			{
				NodeProperty *b = _startProp;
				_startProp = prop;
				prop = b;
			}
			try{
				_model->connected(_startProp->getNode()->getName()+"."+_startProp->getName(),
									prop->getNode()->getName()+"."+prop->getName());
				//this->cline->setEndPoint(event->scenePos().x(),event->scenePos().y());
				_cline->setStartEndProp(_startProp,prop);
				_startProp->addConnection(_cline);
				prop->addConnection(_cline);
				_startProp->moveBy(0,0);
				prop->moveBy(0,0);
				_cline = NULL;
				_model->connectSlot(
						_startProp->getNode()->getName()+"."+_startProp->getName(),
						prop->getNode()->getName()+"."+prop->getName());
			}catch(std::runtime_error){
				this->removeItem(_cline);
				_cline = NULL;
			}
			_addLine = false;
		}else{
			this->removeItem(_cline);
			_cline = NULL;
			_addLine = false;
		}
	}else if(_addLine){
		this->removeItem(_cline);
		_cline = NULL;
		_addLine = false;
	}

	QGraphicsScene::mouseReleaseEvent(event);
	update();
}

void NodeHandler::addNode(QString name, QPointF pos)
{
	new Node(name,pos.x(),pos.y(),this);
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

		this->_deselectAllNodes();
		node->setSelectedNode(true);
		this->_selectedNode = node;

		QString newname = _model->addNode(node->getName());
		node->setModulName(node->getName());
		if(!(newname == "")) node->setName(newname);
		else this->removeItem(node);
		_model->setPrefix(node->getName());
	}
}

void NodeHandler::dragEnterEvent(QGraphicsSceneDragDropEvent *){}
void NodeHandler::dragMoveEvent(QGraphicsSceneDragDropEvent *){}


NodeHandler::~NodeHandler() {
	// TODO Auto-generated destructor stub
}
