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
/** \file   NodeProperty.cpp
 *  \brief  Implementationof class NodeProperty
 *  \date   15.03.2011
 *  \author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#include "NodeProperty.h"
#include <QPainter>
#include "ConnectionSocket.h"
#include "ConnectionLine.h"
#include "GraphModel.h"
#include "TypeHandler.h"
#include "NodeHandler.h"
#include "Node.h"

NodeProperty::NodeProperty(
		QGraphicsItem* parentNode, QString name, int propNr,
		QString ptypeName, PropType::NodePropertyIOType p_iotype) :
			QGraphicsItem(parentNode),
			_isConnected(false),
			_drawType(false),
			_name(name)
{
	setAcceptHoverEvents(true);
	_ptype = new PropType();

	if (TypeHandler::hasType(ptypeName)) _ptype->ptype =
			TypeHandler::getType(ptypeName);
	else {
		ParameterType* paramtype = new ParameterType(ptypeName);
		TypeHandler::addType(paramtype);
		_ptype->ptype = paramtype;
	}

	_ptype->iotype = p_iotype;
	_propNr = propNr;

	int yy = 28 + _propNr * 25;

	Node *n = dynamic_cast<Node*> (parentItem());
	if (n != 0) {
		_node = n;
		_width = _node->getWidth() - 10;
	}

	switch (getIOType()) {
	case PropType::IN:
		_socket = new ConnectionSocket(this, QPointF(0, yy + 10));
		_multiConnect = false;
		break;
	case PropType::OUT:
		_socket = new ConnectionSocket(this, QPointF(_width + 10, yy + 10));
		_multiConnect = true;
		break;
	default:
		break;
	}
}

NodeProperty::~NodeProperty() {
}

void NodeProperty::moveBy(qreal, qreal) {
	for (int i = 0; i < _connectionList.size(); i++) {
		ConnectionLine *cline = _connectionList.at(i);
		if (cline->getStartProp() == this) {
			cline->setStartPoint(
					scenePos()+(dynamic_cast<ConnectionSocket*>(
							children().at(0)))->getCenter()); //ugly as hell!!
		} else {
			cline->setEndPoint(
					scenePos()+(dynamic_cast<ConnectionSocket*>(
							children().at(0)))->getCenter()); //ugly as hell!!
		}
	}

}

unsigned int NodeProperty::getNr() {
	return _propNr;
}

QList<ConnectionLine*> NodeProperty::getConnections() {
	return _connectionList;
}

QString NodeProperty::getName() {
	return _name;
}

void NodeProperty::addConnection(ConnectionLine *nl) {
	_isConnected = true;
	_connectionList.push_back(nl);
}

PropType *NodeProperty::getPropType() {
	return _ptype;
}


QRectF NodeProperty::boundingRect() const {
	int yy = 45 + _propNr * 25;
	return QRectF(5, yy, _width, 20);
}

void NodeProperty::paint(
		QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
	_width = _node->getWidth() - 10;
	int yy = 28 + _propNr * 25;
	painter->setOpacity(1);
	painter->setBrush(Qt::lightGray);
	painter->drawRoundRect(5, yy, _width, 20, 10, 100);
	painter->setBrush(Qt::black);
	painter->drawText(10, yy + 13, _name);
	if (_drawType) {
		switch (_ptype->iotype) {
		case PropType::IN: {
			painter->setBrush(Qt::white);
			painter->setOpacity(0.8);
			QPointF p = _socket->getCenter() - QPointF(
					_ptype->ptype->getTypeNameUnTemplated().size()*7 + 10, 10);
			int length = _ptype->ptype->getTypeNameUnTemplated().size()*7;
			painter->drawRoundRect(p.x(), p.y(), length, 20, 60, 70);
			painter->setBrush(Qt::black);
			painter->setOpacity(1);
			painter->drawText(
					p.x() + 5, p.y() + 13,
					_ptype->ptype->getTypeNameUnTemplated());
			break;
		}
		case PropType::OUT: {
			painter->setBrush(Qt::white);
			painter->setOpacity(0.8);
			QPointF p = _socket->getCenter() + QPointF(10, -10);
			int length = _ptype->ptype->getTypeNameUnTemplated().size()*7;
			painter->drawRoundRect(p.x(), p.y(), length, 20, 60, 70);
			painter->setBrush(Qt::black);
			painter->setOpacity(1);
			painter->drawText(
					p.x() + 5, p.y() + 13,
					_ptype->ptype->getTypeNameUnTemplated());
			break;
		}
		default:
			break;
		}
	}
}

PropType::NodePropertyIOType NodeProperty::iotypeFromString(QString str) {
	if (str == "in")
		return PropType::IN;
	else if (str == "out")
		return PropType::OUT;
	qWarning("Invalid node property type: %s", str.toAscii().constData());
	return PropType::OUT;
}

QString NodeProperty::iotypeToString(PropType::NodePropertyIOType type) {
	switch (type) {
	case PropType::IN:
		return "in";
	case PropType::OUT:
		return "out";
	}
	return QString();
}

bool NodeProperty::canConnect(NodeProperty *prop) {
	bool sameTemp = false;
	if (_ptype->ptype->isTemplated()) {
		sameTemp = (_ptype->ptype->getTempName() ==
					prop->getPropType()->ptype->getTempName());
	} else {
		sameTemp = true;
	}

	if (canNewConnect() && prop->canNewConnect() &&
			prop->getPropType()->iotype != _ptype->iotype &&
			prop->getPropType()->ptype->getTypeNameUnTemplated() ==
			_ptype->ptype->getTypeNameUnTemplated() &&
			sameTemp)
		return true;
	return false;
}

PropType::NodePropertyIOType NodeProperty::getIOType() {
	return _ptype->iotype;
}

void NodeProperty::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
	_drawType = true;
}

void NodeProperty::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
	_drawType = false;
}

bool NodeProperty::canNewConnect() {
	return (!_isConnected || (_isConnected && _multiConnect));
}

bool NodeProperty::hasConnection() {
	return _isConnected;
}

void NodeProperty::removeAllConnections(GraphModel *model) {
	if(!model) model = ((NodeHandler*)scene())->model();
	//cout<<"removing all connections"<<endl;
	for (int i = 0; i<_connectionList.size(); i++) {
		ConnectionLine *l = _connectionList[i];
		model->disconnectSlot(
				l->getStartProp()->getNode()->getName()+"."
					+l->getStartProp()->getName(),
				l->getEndProp()->getNode()->getName()+"."
					+l->getEndProp()->getName());
		switch (_ptype->iotype) {
		case PropType::IN:
			l->getStartProp()->removeConnection(l);
			break;
		case PropType::OUT:
			l->getEndProp()->removeConnection(l);
			break;
		}
		delete l;
	}
	_connectionList.clear();
	_isConnected = false;
}

void NodeProperty::removeConnection(ConnectionLine *line) {
	_connectionList.removeAll(line);
	if (_connectionList.size() == 0) _isConnected = false;
}

Node *NodeProperty::getNode() {
	return _node;
}
