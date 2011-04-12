/*	Copyright (C) 2011 Jonathan Wuest
 * @file   NodeProperty.cpp
 *  @brief  Implementationof class NodeProperty
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#include "NodeProperty.h"
#include <QPainter>
#include <iostream>
#include "ConnectionSocket.h"
#include "ConnectionLine.h"
#include "GraphModel.h"
#include "TypeHandler.h"
#include "NodeHandler.h"
#include "Node.h"



using namespace std;

NodeProperty::NodeProperty() {

}

NodeProperty::NodeProperty(QGraphicsItem *parentNode, QString name, int propNr, QString ptypeName, PropType::NodePropertyIOType p_iotype) : QGraphicsItem(parentNode) {
	this->_isConnected = false;
	this->_drawType = false;
	this->_name = name;
	this->setAcceptHoverEvents(true);
	this->_ptype = new PropType();
	if (TypeHandler::hasType(ptypeName)) _ptype->ptype = TypeHandler::getType(ptypeName);
	else {
		ParameterType *paramtype = new ParameterType(ptypeName);
		TypeHandler::addType(paramtype);
		_ptype->ptype = paramtype;
	}
	_ptype->iotype = p_iotype;
	this->_propNr = propNr;

	int y = 45 + this->_propNr * 25;

	Node *n = dynamic_cast<Node*> (this->parentItem());
	if (n != 0) {
		this->_node = n;
		this->_width = _node->getWidth() - 10;
	}

	switch (this->getIOType()) {
		case PropType::IN:
		{
			this->_socket = new ConnectionSocket(this, QPointF(0, y + 10));
			this->_multiConnect = false;
			break;
		}
		case PropType::OUT:
		{
			this->_socket = new ConnectionSocket(this, QPointF(this->_width + 10, y + 10));
			this->_multiConnect = true;
			break;
		}
		case PropType::NONE:
		{
			break;
		}
	}
}

void NodeProperty::moveBy(qreal, qreal) {
	for (int i = 0; i < _connectionList.size(); i++) {
		ConnectionLine *cline = _connectionList.at(i);
		if (cline->getStartProp() == this) {
			cline->setStartPoint(this->scenePos()+(dynamic_cast<ConnectionSocket*> (this->children().at(0)))->getCenter()); //ugly as hell!!
		} else {
			cline->setEndPoint(this->scenePos()+(dynamic_cast<ConnectionSocket*> (this->children().at(0)))->getCenter()); //ugly as hell!!
		}
	}

}

unsigned int NodeProperty::getNr() {
	return this->_propNr;
}

QList<ConnectionLine*> NodeProperty::getConnections() {
	return this->_connectionList;
}

QString NodeProperty::getName() {
	return this->_name;
}

void NodeProperty::addConnection(ConnectionLine *nl) {
	this->_isConnected = true;
	this->_connectionList.push_back(nl);
}

PropType *NodeProperty::getPropType() {
	return this->_ptype;
}


QRectF NodeProperty::boundingRect() const {
	if(this->_ptype->iotype == PropType::NONE) QRectF(0, 0, 0, 0);
	int y = 45 + this->_propNr * 25;
	return QRectF(5, y, this->_width, 20);
}

void NodeProperty::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
	if(this->_ptype->iotype == PropType::NONE) return;
	this->_width = _node->getWidth() - 10;
	int y = 45 + this->_propNr * 25;
	painter->setOpacity(1);
	painter->setBrush(Qt::lightGray);
	painter->drawRoundRect(5, y, this->_width, 20, 10, 100);
	painter->setBrush(Qt::black);
	painter->drawText(10, y + 13, _name);
	if (this->_drawType) {
		switch (_ptype->iotype) {
			case PropType::IN:
			{
				painter->setBrush(Qt::white);
				painter->setOpacity(0.8);
				QPointF p = _socket->getCenter() - QPointF(_ptype->ptype->getTypeNameUnTemplated().size()*7 + 10, 10);
				int length = _ptype->ptype->getTypeNameUnTemplated().size()*7;
				painter->drawRoundRect(p.x(), p.y(), length, 20, 60, 70);
				painter->setBrush(Qt::black);
				painter->setOpacity(1);
				painter->drawText(p.x() + 5, p.y() + 13, _ptype->ptype->getTypeNameUnTemplated());
				break;
			}
			case PropType::OUT:
			{
				painter->setBrush(Qt::white);
				painter->setOpacity(0.8);
				QPointF p = _socket->getCenter() + QPointF(10, -10);
				int length = _ptype->ptype->getTypeNameUnTemplated().size()*7;
				painter->drawRoundRect(p.x(), p.y(), length, 20, 60, 70);
				painter->setBrush(Qt::black);
				painter->setOpacity(1);
				painter->drawText(p.x() + 5, p.y() + 13, _ptype->ptype->getTypeNameUnTemplated());
				break;
			}
			case PropType::NONE:
			{
				break;
			}
		}
	}
}

PropType::NodePropertyIOType NodeProperty::iotypeFromString(QString str) {
	if (str == "in") return PropType::IN;
	else if (str == "out") return PropType::OUT;
	else if (str == "none") return PropType::NONE;
	return PropType::NONE;
}

QString NodeProperty::iotypeToString(PropType::NodePropertyIOType type) {
	switch (type) {
		case PropType::IN:
		{
			return "in";
		}
		case PropType::OUT:
		{
			return "out";
		}
		case PropType::NONE:
		{
			return "none";
		}
	}
	return "none";
}

bool NodeProperty::canConnect(NodeProperty *prop) {
	bool sameTemp = false;
	if(this->_ptype->ptype->isTemplated()){
	sameTemp = (_ptype->ptype->getTempName() == prop->getPropType()->ptype->getTempName());
	}else{
	sameTemp = true;
	}

	if (this->canNewConnect() && prop->canNewConnect() &&
			prop->getPropType()->iotype != this->_ptype->iotype &&
			prop->getPropType()->ptype->getTypeNameUnTemplated() ==
			this->_ptype->ptype->getTypeNameUnTemplated() &&
			sameTemp) return true;
	return false;
}

PropType::NodePropertyIOType NodeProperty::getIOType() {
	return this->_ptype->iotype;
}

void NodeProperty::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
	this->_drawType = true;
}

void NodeProperty::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
	this->_drawType = false;
}

bool NodeProperty::canNewConnect() {
	return (!this->_isConnected || (this->_isConnected && this->_multiConnect));
}

bool NodeProperty::hasConnection() {
	return this->_isConnected;
}

void NodeProperty::removeAllConnections(GraphModel *model) {
	if(!model) model = ((NodeHandler*)this->scene())->model();
	//cout<<"removing all connections"<<endl;
	for (int i = 0; i<this->_connectionList.size(); i++) {
		ConnectionLine *l = _connectionList[i];
		model->disconnectSlot(l->getStartProp()->getNode()->getName()+"."+l->getStartProp()->getName(),
					  l->getEndProp()->getNode()->getName()+"."+l->getEndProp()->getName());
		switch (_ptype->iotype) {
			case PropType::IN:
			{
				l->getStartProp()->removeConnection(l);
				break;
			}
			case PropType::OUT:
			{
				l->getEndProp()->removeConnection(l);
				break;
			}
			case PropType::NONE:
			{
				break;
			}
		}
		delete l;
	}
	_connectionList.clear();
	this->_isConnected = false;
}

void NodeProperty::removeConnection(ConnectionLine *line) {
	_connectionList.removeAll(line);
	if (this->_connectionList.size() == 0) this->_isConnected = false;
}

Node *NodeProperty::getNode() {
	return this->_node;
}

NodeProperty::~NodeProperty() {

}
