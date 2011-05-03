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
#include <QHelpEvent>
#include "ConnectionSocket.h"
#include "ConnectionLine.h"
#include "GraphModel.h"
#include "NodeHandler.h"
#include "Node.h"
#include "QParameterFile.h"

NodeProperty::NodeProperty(
		Node* parentNode, QString name, int propNr,
		QString pType, bool input, const QParameterFile* pF) :
			QGraphicsItem(parentNode),
			_isConnected(false),
			_multiConnect(!input),
			_propNr(propNr),
			_name(name),
			_isInput(input),
			_propType(pType),
			_node(parentNode),
			_pFile(pF)
{
	Q_ASSERT(_node);

	int yy = 28 + _propNr * 25;
	_width = _node->getWidth() - 10;

	_socket = new ConnectionSocket(
			this, _isInput ? QPointF(0, yy+10) : QPointF(_width+10, yy+10));

	setAcceptHoverEvents(true);
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

QString NodeProperty::getName() const {
	return _name;
}

void NodeProperty::addConnection(ConnectionLine *nl) {
	_isConnected = true;
	_connectionList.push_back(nl);
}

QString NodeProperty::getType() const {
	QString type;
	type = _propType;

	// handle templated types
	NodeHandler* h = qobject_cast<NodeHandler*>(scene()); Q_ASSERT(h);
	if (_pFile) {
		QString ttype = _pFile->get(
				_node->getInstanceName()+".templatetype");
		if (!ttype.isEmpty()) {
			type.replace(
					QRegExp("<\\s*T\\s*>",Qt::CaseInsensitive),
					QString("&lt;%1&gt;").arg(ttype));
		}
	}
	return type;
}


QRectF NodeProperty::boundingRect() const {
	int yy = 28 + _propNr * 25;
	return QRectF(5, yy, _width, 20);
}

void NodeProperty::paint(
		QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
	_width = _node->getWidth() - 10;
	int yy = 28 + _propNr * 25;
	painter->setOpacity(1);
	painter->setBrush(isUnderMouse() ? Qt::yellow : Qt::lightGray);
	painter->drawRoundRect(5, yy, _width, 20, 10, 100);
	painter->setBrush(Qt::black);
	painter->drawText(10, yy + 13, _name);
}

bool NodeProperty::canConnect(NodeProperty* prop) {
	if (canNewConnect() && prop->canNewConnect() &&
			_isInput != prop->_isInput &&
			getType() == prop->getType())
		return true;
	return false;
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
				l->getStartProp()->getNode()->getInstanceName()+"."
					+l->getStartProp()->getName(),
				l->getEndProp()->getNode()->getInstanceName()+"."
					+l->getEndProp()->getName());
		if (_isInput) {
			l->getStartProp()->removeConnection(l);
		}
		else {
			l->getEndProp()->removeConnection(l);
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

Node* NodeProperty::getNode() {
	return _node;
}

bool NodeProperty::isInput() const {
	return _isInput;
}

void NodeProperty::hoverEnterEvent(QGraphicsSceneHoverEvent* ev) {
	setToolTip(QString("<b>Slot: <i>%1</i><br>Type:</b><br>%2")
				.arg(_name).arg(getType()));
	QGraphicsItem::hoverEnterEvent(ev);
}
