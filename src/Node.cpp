/*	Copyright (C) 2011 Jonathan Wuest
 *  @file   Node.cpp
 *  @brief  Implementation of class Node
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#include "Node.h"
#include "NodeHandler.h"
#include "GraphModel.h"
#include <QFile>
#include <QTextStream>
#include <QPainter>
#include <QPainterPath>
#include <QBrush>

unsigned int Node::_idCount = 0;

Node::Node(const ParameterFileModel* pFile, QString title,
			int xpos, int ypos,QGraphicsScene *parent) :
		QGraphicsItem(0,parent),
		_instanceName(title),
		_width(100),
		_height(50),
		_nProps(0),
		_selectedNode(false),
		_id(_idCount++),
		_pFile(pFile)
{
	setPos(xpos,ypos);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
}

void Node::setId(unsigned int id) {
	if (id >= Node::_idCount)
		Node::_idCount = id+1;
	_id = id;
}

void Node::setName(QString name) {
	_instanceName = name;
	_checkWidth();
}

QString Node::getInstanceName() {
	return _instanceName;
}

void Node::addProperty(QString name, bool input) {
		NodeProperty* prop = new NodeProperty(
						this,name,_nProps,input,_pFile);
		_height += 25;
		_properties.insert(name.toLower(),prop);
		_nProps++;
		_checkWidth(name.size());
}

QRectF Node::boundingRect() const {
	QRectF bb(0,0,_width,_height);
	bb.adjust(-1,-1,+1,+1);
	return bb;
}

void Node::setSelectedNode(bool s) {
	_selectedNode = s;
	if(_selectedNode){
		changeConnectionLineColor(Qt::blue);
	}
	else{
		changeConnectionLineColor(Qt::black);
	}
}

bool Node::isSelectedNode() {
	return _selectedNode;
}

void Node::changeConnectionLineColor(QColor lineColor){
	QMapIterator<QString,NodeProperty*> i(_properties);
	NodeProperty* curProp;
	while(i.hasNext()){
		curProp = i.next().value();
		curProp->changeConnectionLineColor(lineColor,false);
	}
}

void Node::mouseMoveEvent(QGraphicsSceneMouseEvent* ev) {
	QGraphicsItem::mouseMoveEvent(ev);

	qreal dx = (ev->scenePos()-ev->lastScenePos()).x();
	qreal dy = (ev->scenePos()-ev->lastScenePos()).y();
	for (int i=0; i<children().size(); i++) {
		NodeProperty *np = dynamic_cast<NodeProperty*>(children()[i]);
		if(np != 0) {
			np->moveBy(dx,dy);
		}
	}

	// update model
	NodeHandler* handler = qobject_cast<NodeHandler*>(scene());
	Q_ASSERT(handler);
	GraphModel* model = handler->model();
	QString posString = QString("%1 %2").arg(pos().x()).arg(pos().y());
	model->setValue(_instanceName+".editorinfo",posString);
}

void Node::paint(
		QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
	painter->setPen(QPen(Qt::black,1.f));
	painter->setOpacity(1);
	painter->setBrush(_selectedNode ? Qt::blue : Qt::gray);
	painter->drawRoundedRect(0,0,_width,_height,10,10);
	painter->setBrush(_selectedNode ? QColor("#AAF") : QColor("#BBC"));
	painter->drawRoundedRect(0,0,_width,22,10,10);
	painter->drawRoundedRect(0,_height-22,_width,22,10,10);
	painter->setPen(Qt::black);

	QFont f = painter->font();
	f.setPixelSize(15);
	f.setBold(true);
	f.setCapitalization(QFont::SmallCaps);
	painter->setFont(f);
	painter->drawText(0,0,_width,22,Qt::AlignCenter,_className);
	f.setCapitalization(QFont::MixedCase);
	painter->setFont(f);
	painter->drawText(0,_height-22,_width,22,Qt::AlignCenter,_instanceName);
}

unsigned int Node::getId() const {
	return _id;
}

int Node::getWidth() const {
	return _width;
}

int Node::getHeight() const {
	return _height;
}

void Node::_checkWidth(int s) {
	s = qMax(s,qMax(_instanceName.size(), _className.size()));
	_width = qMax(_width, 50 + s * 7);
}

NodeProperty* Node::getProperty(QString propName) const {
	return _properties[propName];
}

void Node::setClassName(QString modname) {
	_className = modname;
	_checkWidth();
}

QString Node::getClassName() const {
	return _className;
}


