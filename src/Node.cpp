/*	Copyright (C) 2011 Jonathan Wuest
 *  @file   Node.cpp
 *  @brief  Implementation of class Node
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#include "Node.h"
#include "ParameterFile.h"
#include "NodeHandler.h"
#include "GraphModel.h"
#include <QFile>
#include <QTextStream>
#include <QPainter>
#include <QPainterPath>
#include <QBrush>
#include <string>
#include<iostream>
#include<math.h>



using namespace std;

unsigned int Node::_idCount = 0;

Node::Node() {
	this->setFlag(QGraphicsItem::ItemIsMovable,false);
}

Node::Node(QString title, int xpos, int ypos,QGraphicsScene *parent) : QGraphicsItem(0,parent)
{
	this->_name = title;
	this->setPos(xpos,ypos);
	this->_width = 100;
	this->_height = 50;
	this->setFlag(QGraphicsItem::ItemIsMovable);
	this->setFlag(QGraphicsItem::ItemIsSelectable);
	this->_nProps = 0;
	this->_id = _idCount;
	_idCount++;
}

void Node::setId(unsigned int id){
	if(id >= Node::_idCount) Node::_idCount = id+1;
	this->_id = id;
}

void Node::setName(QString name){
	this->_name = name;
	checkWidth();
}

QString Node::getName()
{
	return this->_name;
}

void Node::addProperty(QString name,QString typeName,PropType::NodePropertyIOType iotype)
{
	NodeProperty *prop = new NodeProperty(this,name,_nProps,typeName,iotype);
	_properties.push_back(prop);
	_nProps++;
	if(iotype != PropType::NONE) _height += 25;
}

QRectF Node::boundingRect() const
{
	 return QRectF(0,0,_width,_height);
}
void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mousePressEvent(event);
	update();
}

void Node::setSelectedNode(bool s){
	this->_selectedNode = s;
}
bool Node::isSelectedNode(){
	return this->_selectedNode;
}


void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

	QGraphicsItem::mouseMoveEvent(event);
	qreal dx = (event->scenePos()-event->lastScenePos()).x();
	qreal dy = (event->scenePos()-event->lastScenePos()).y();
	for(int i=0;i<this->children().size();i++)
	{
		NodeProperty *np = dynamic_cast<NodeProperty*>(this->children().at(i));
		if(np != 0)
		{
			np->moveBy(dx,dy);
		}
	}

	// update model
	GraphModel *model = ((NodeHandler*)this->scene())->model();
	QString posstring = QString("%0 %1").arg(this->pos().x()).arg(this->pos().y());
	model->setOnlyParams(false);
	bool found = false;
	//search for entry
	for(int i=0;i<model->rowCount();i++){
		if(model->data(model->index(i,0)).toString() == "editorinfo"){
		model->setData(model->index(i,1),posstring);
		found = true;
		break;
		}
	}
	//insert editor info
	if(!found){
		model->insertRow(model->rowCount());
		model->setData(model->index(model->rowCount()-1,0),"editorinfo");
		model->setData(model->index(model->rowCount()-1,1),posstring);
	}

	model->setOnlyParams(true);

}

void Node::remove(){
	for(int i=0;i<_properties.size();i++){
		_properties.at(i)->removeAllConnections();
	}
	_properties.clear();
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	if(this->_selectedNode){
		QRadialGradient gradient(_width/2, _height/2, _width*0.7, _width/2, _height/2);
		gradient.setColorAt(0, QColor::fromRgbF(0, 0.2, 1, 1));
		gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
		QBrush brush(gradient);
		painter->setBrush(brush);
		painter->setPen(Qt::NoPen);
		painter->drawRect(-40,-40,_width+80,_height+80);
	}
	painter->setPen(Qt::black);
	painter->setOpacity(1);
	painter->setBrush(Qt::gray);
	painter->drawRoundRect(0,0,_width,_height,15,15);
	if(this->_selectedNode)	painter->setBrush(Qt::blue);
	else painter->setBrush(Qt::lightGray);
	painter->drawRoundRect(0,0,_width,36,15,65);
	if(this->_selectedNode) painter->setPen(Qt::white);
	else painter->setPen(Qt::black);
	painter->drawText(5,13,_name);
	painter->setPen(Qt::gray);
	painter->drawText(5,24,"["+_modulname+"]");
}

unsigned int Node::getId()
{
	return this->_id;
}

int Node::getWidth()
{
	return this->_width;
}

int Node::getHeight()
{
	return this->_height;
}

void Node::checkWidth(){
	if(this->_name.size() > 14){
		this->_width = 100 + (this->_name.size() - 14)*5;
	}else{
		if(_modulname.size() > 14) _width = 100 + (_modulname.size()-14)*5;
	}

}


QString Node::getConfigFileName(){
	return this->_configFileName;
}

QVector<NodeProperty*> Node::getProperties(){
	return this->_properties;
}


void Node::loadFromParameterFile(QString fname){
	this->_configFileName = fname;
	ParameterFile pfile(fname.toStdString());
	string modname = pfile.getEveryParameter("")[0];
	modname = modname.substr(0,modname.find("."));
	this->_name = QString::fromStdString(modname);
	checkWidth();
	string params = pfile.get<string>(modname + ".inputs");
	vector<string> ins;
	size_t pos = 0;
	while(pos != params.npos){
		pos = params.find(";");
		ins.push_back(params.substr(0,pos));
		params = params.substr(pos+1);
	}
	for(unsigned int i=0;i<ins.size();i++){
		if(ins[i]!= ""){
			QString tname = QString::fromStdString(pfile.get<string>(modname+"."+ins[i]+".type"));
			this->addProperty(QString::fromStdString(ins[i]),tname,PropType::IN);
		}

	}
	params = pfile.get<string>(modname + ".outputs");
	vector<string> outs;
	pos = 0;
	while(pos != params.npos){
		pos = params.find(";");
		outs.push_back(params.substr(0,pos));
		params = params.substr(pos+1);
	}
	for(unsigned int i=0;i<outs.size();i++){
		if(outs[i]!= ""){
			QString tname = QString::fromStdString(pfile.get<string>(modname+"."+outs[i]+".type"));
			this->addProperty(QString::fromStdString(outs[i]),tname,PropType::OUT);
		}
	}
	params = pfile.get<string>(modname + ".parameters");
	vector<string> pars;
	pos = 0;
	while(pos != params.npos){
		pos = params.find(";");
		pars.push_back(params.substr(0,pos));
		params = params.substr(pos+1);
	}
	for(unsigned int i=0;i<pars.size();i++){
		if(pars[i]!= ""){
			QString tname = QString::fromStdString(pfile.get<string>(modname+"."+pars[i]+".type"));
			this->addProperty(QString::fromStdString(pars[i]),tname,PropType::NONE);
		}
	}
}

void Node::setModulName(QString modname){
	_modulname = modname;
	checkWidth();
}

QString Node::getModulName(){
	return _modulname;
}

Node::~Node() {
}
