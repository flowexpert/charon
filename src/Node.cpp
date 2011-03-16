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
/** \file   Node.cpp
 *  \brief  Implementation of class Node
 *  \date   15.03.2011
 *  \author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#include "Node.h"
#include "ParameterFile.h"
#include "NodeHandler.h"
#include "GraphModel.h"
#include <QFile>
#include <QTextStream>
#include <QPainter>
#include <QPainterPath>
#include <QMessageBox>
#include <QBrush>
#include <string>
#include <cmath>

unsigned int Node::idCount = 0;

Node::Node() {
	setFlag(QGraphicsItem::ItemIsMovable,false);
}

Node::Node(QString title, int xpos, int ypos,QGraphicsScene* p) :
		QGraphicsItem(0,p) {
	name = title;
	setPos(xpos,ypos);
	width = 100;
	height = 30;
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	nProps = 0;
	id = idCount;
	idCount++;
}

void Node::setId(unsigned int nId){
	if(nId >= Node::idCount)
		Node::idCount = nId+1;
	id = nId;
}

void Node::setName(QString nName){
	name = nName;
	checkWidth();
}

QString Node::getName()
{
	return this->name;
}

void Node::addProperty(
		QString nn,QString typeName,PropType::NodePropertyIOType iotype) {
	NodeProperty *prop = new NodeProperty(this,nn,nProps,typeName,iotype);
	properties.push_back(prop);
	nProps++;
	if(iotype != PropType::NONE) height += 25;
}

QRectF Node::boundingRect() const
{
	 return QRectF(0,0,width,height);
}
void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mousePressEvent(event);
	update();
}

void Node::setSelectedNode(bool s){
	this->selectedNode = s;
}
bool Node::isSelectedNode(){
	return this->selectedNode;
}


void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
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
	QString posstring = QString("%0 %1").arg(
			this->pos().x()).arg(this->pos().y());
	model->setOnlyParams(false);
	bool found = false;
	// search for entry
	for(int i=0;i<model->rowCount();i++){
		if(model->data(model->index(i,0)).toString() == "editorinfo"){
		model->setData(model->index(i,1),posstring);
		found = true;
		break;
		}
	}
	// insert editor info
	if(!found){
		model->insertRow(model->rowCount());
		model->setData(model->index(model->rowCount()-1,0),"editorinfo");
		model->setData(model->index(model->rowCount()-1,1),posstring);
	}

	model->setOnlyParams(true);

}

void Node::remove(){
	for(int i=0;i<properties.size();i++){
		properties.at(i)->removeAllConnections();
	}
	properties.clear();
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	if(this->selectedNode){
		QRadialGradient gradient(
				width/2, height/2, width*0.7, width/2, height/2);
		gradient.setColorAt(0, QColor::fromRgbF(0, 0.2, 1, 1));
		gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
		QBrush brush(gradient);
		painter->setBrush(brush);
		painter->setPen(Qt::NoPen);
		painter->drawRect(-40,-40,width+80,height+80);
	}
	painter->setPen(Qt::black);
	painter->setOpacity(1);
	painter->setBrush(Qt::gray);
	painter->drawRoundRect(0,0,width,height,15,15);
	if(this->selectedNode)	painter->setBrush(Qt::blue);
	else painter->setBrush(Qt::lightGray);
	painter->drawRoundRect(0,0,width,18,15,65);
	if(this->selectedNode) painter->setPen(Qt::white);
	else painter->setPen(Qt::black);
	painter->drawText(5,13,name);
}

unsigned int Node::getId()
{
	return this->id;
}

int Node::getWidth()
{
	return this->width;
}

int Node::getHeight()
{
	return this->height;
}

void Node::checkWidth(){
	if(this->name.size() > 14){
		this->width = 100 + (this->name.size() - 14)*5;
	}
}

QString Node::getConfigFileName(){
	return this->configFileName;
}

QVector<NodeProperty*> Node::getProperties(){
	return this->properties;
}


void Node::loadFromParameterFile(QString fname){
	this->configFileName = fname;
	ParameterFile pfile(fname.toStdString());
	std::string modname = pfile.getEveryParameter("")[0];
	modname = modname.substr(0,modname.find("."));
	this->name = QString::fromStdString(modname);
	checkWidth();
	std::string params = pfile.get<std::string>(modname + ".inputs");
	std::vector<std::string> ins;
	size_t cPos = 0;
	while(cPos != params.npos){
		cPos = params.find(";");
		ins.push_back(params.substr(0,cPos));
		params = params.substr(cPos+1);
	}
	for(unsigned int i=0;i<ins.size();i++){
		if(ins[i]!= ""){
			QString tname = QString::fromStdString(
						pfile.get<std::string>(modname+"."+ins[i]+".type"));
			this->addProperty(
						QString::fromStdString(ins[i]),tname,PropType::IN);
		}

	}
	params = pfile.get<std::string>(modname + ".outputs");
	std::vector<std::string> outs;
	cPos = 0;
	while(cPos != params.npos){
		cPos = params.find(";");
		outs.push_back(params.substr(0,cPos));
		params = params.substr(cPos+1);
	}
	for(unsigned int i=0;i<outs.size();i++){
		if(outs[i]!= ""){
			QString tname = QString::fromStdString(
						pfile.get<std::string>(modname+"."+outs[i]+".type"));
			this->addProperty(
						QString::fromStdString(outs[i]),tname,PropType::OUT);
		}
	}
	params = pfile.get<std::string>(modname + ".parameters");
	std::vector<std::string> pars;
	cPos = 0;
	while(cPos != params.npos){
		cPos = params.find(";");
		pars.push_back(params.substr(0,cPos));
		params = params.substr(cPos+1);
	}
	for(unsigned int i=0;i<pars.size();i++){
		if(pars[i]!= ""){
			QString tname = QString::fromStdString(
						pfile.get<std::string>(modname+"."+pars[i]+".type"));
			this->addProperty(
						QString::fromStdString(pars[i]),tname,PropType::NONE);
		}
	}
}

Node::~Node() {
}
