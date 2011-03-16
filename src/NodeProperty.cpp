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
/** @file   NodeProperty.cpp
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

NodeProperty::NodeProperty() {
}

NodeProperty::NodeProperty(
		QGraphicsItem* parentNode, QString nn, int pNr,
		QString ptypeName, PropType::NodePropertyIOType p_iotype) :
		QGraphicsItem(parentNode) {
	isConnected = false;
	drawType = false;
	name = nn;
	setAcceptHoverEvents(true);
	ptype = new PropType();
	if (TypeHandler::hasType(ptypeName))
		ptype->ptype = TypeHandler::getType(ptypeName);
	else {
		ParameterType *paramtype = new ParameterType(ptypeName);
		TypeHandler::addType(paramtype);
		ptype->ptype = paramtype;
	}
	ptype->iotype = p_iotype;
	propNr = pNr;

	int yy = 25 + propNr * 25;

	Node *n = dynamic_cast<Node*> (this->parentItem());
	if (n != 0) {
		node = n;
		width = node->getWidth() - 10;
	}

	switch (this->getIOType()) {
		case PropType::IN:
		{
			socket = new ConnectionSocket(
						this, QPointF(0, yy + 10));
			multiConnect = false;
			break;
		}
		case PropType::OUT:
		{
			socket = new ConnectionSocket(
						this, QPointF(this->width + 10, yy + 10));
			multiConnect = true;
			break;
		}
		case PropType::NONE:
		{
			break;
		}
	}
}

void NodeProperty::moveBy(qreal, qreal) {
	for (int i = 0; i < connectionList.size(); i++) {
		ConnectionLine *cline = connectionList.at(i);
		if (cline->getStartProp() == this) {
			cline->setStartPoint(
				this->scenePos()+(dynamic_cast<ConnectionSocket*> (
					this->children().at(0)))->getCenter()); // ugly as hell!!
		} else {
			cline->setEndPoint(
				this->scenePos()+(dynamic_cast<ConnectionSocket*> (
					this->children().at(0)))->getCenter()); // ugly as hell!!
		}
	}

}

unsigned int NodeProperty::getNr() {
	return this->propNr;
}

QList<ConnectionLine*> NodeProperty::getConnections() {
	return this->connectionList;
}

QString NodeProperty::getName() {
	return this->name;
}

void NodeProperty::addConnection(ConnectionLine *nl) {
	this->isConnected = true;
	this->connectionList.push_back(nl);
}

PropType *NodeProperty::getPropType() {
	return this->ptype;
}

void NodeProperty::mousePressEvent(QGraphicsSceneMouseEvent*) {
	// TODO open property edit dialog
}

QRectF NodeProperty::boundingRect() const {
	if(this->ptype->iotype == PropType::NONE) QRectF(0, 0, 0, 0);
	int yy = 25 + this->propNr * 25;
	return QRectF(5, yy, this->width, 20);
}

void NodeProperty::paint(
		QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) {
	if(this->ptype->iotype == PropType::NONE) return;
	this->width = node->getWidth() - 10;
	int yy = 25 + this->propNr * 25;
	painter->setOpacity(1);
	painter->setBrush(Qt::lightGray);
	painter->drawRoundRect(5, yy, this->width, 20, 10, 100);
	painter->setBrush(Qt::black);
	painter->drawText(10, yy + 13, name);
	if (this->drawType) {
		switch (ptype->iotype) {
			case PropType::IN:
			{
				painter->setBrush(Qt::white);
				painter->setOpacity(0.8);
				QPointF p = socket->getCenter() - QPointF(ptype->ptype->getTypeNameUnTemplated().size()*7 + 10, 10);
				int length = ptype->ptype->getTypeNameUnTemplated().size()*7;
				painter->drawRoundRect(p.x(), p.y(), length, 20, 60, 70);
				painter->setBrush(Qt::black);
				painter->setOpacity(1);
				painter->drawText(p.x() + 5, p.y() + 13, ptype->ptype->getTypeNameUnTemplated());
				break;
			}
			case PropType::OUT:
			{
				painter->setBrush(Qt::white);
				painter->setOpacity(0.8);
				QPointF p = socket->getCenter() + QPointF(10, -10);
				int length = ptype->ptype->getTypeNameUnTemplated().size()*7;
				painter->drawRoundRect(p.x(), p.y(), length, 20, 60, 70);
				painter->setBrush(Qt::black);
				painter->setOpacity(1);
				painter->drawText(p.x() + 5, p.y() + 13, ptype->ptype->getTypeNameUnTemplated());
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
    if(this->ptype->ptype->isTemplated()){
	sameTemp = (ptype->ptype->getTempName() == prop->getPropType()->ptype->getTempName());
    }else{
	sameTemp = true;
    }

	if (this->canNewConnect() && prop->canNewConnect() &&
			prop->getPropType()->iotype != this->ptype->iotype &&
			prop->getPropType()->ptype->getTypeNameUnTemplated() ==
			this->ptype->ptype->getTypeNameUnTemplated() &&
			sameTemp) return true;
	return false;
}

PropType::NodePropertyIOType NodeProperty::getIOType() {
	return this->ptype->iotype;
}

void NodeProperty::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
	this->drawType = true;
}

void NodeProperty::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
	this->drawType = false;
}

bool NodeProperty::canNewConnect() {
	return (!this->isConnected || (this->isConnected && this->multiConnect));
}

bool NodeProperty::hasConnection() {
	return this->isConnected;
}

void NodeProperty::removeAllConnections(GraphModel *model) {
	if(!model) model = ((NodeHandler*)this->scene())->model();
	//cout<<"removing all connections"<<endl;
	for (int i = 0; i<this->connectionList.size(); i++) {
		ConnectionLine *l = connectionList[i];
		model->disconnectSlot(l->getStartProp()->getNode()->getName()+"."+l->getStartProp()->getName(),
				      l->getEndProp()->getNode()->getName()+"."+l->getEndProp()->getName());
		switch (ptype->iotype) {
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
	connectionList.clear();
	this->isConnected = false;
}

void NodeProperty::removeConnection(ConnectionLine *line) {
	connectionList.removeAll(line);
	if (this->connectionList.size() == 0) this->isConnected = false;
}

Node *NodeProperty::getNode() {
	return this->node;
}

NodeProperty::~NodeProperty() {

}
