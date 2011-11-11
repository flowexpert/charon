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
#include "ConnectionLine.h"
#include "GraphModel.h"
#include "NodeHandler.h"
#include "Node.h"
#include "ParameterFileModel.h"

NodeProperty::NodeProperty(
		Node* parentNode, QString name, int propNr,
		bool input, const ParameterFileModel* pF) :
			QGraphicsItem(parentNode),
			_propNr(propNr),
			_name(name),
			_isInput(input),
			_node(parentNode),
			_pFile(pF)
{
	Q_ASSERT(_node);
	setAcceptHoverEvents(true);
}

void NodeProperty::moveBy(qreal, qreal) {
	for (int i = 0; i < _connectionList.size(); i++) {
		ConnectionLine *cline = _connectionList.at(i);
		if (cline->getStartProp() == this) {
			cline->setStartPoint(scenePos()+_getSocketRect().center());
		} else {
			cline->setEndPoint(scenePos()+_getSocketRect().center());
		}
	}
}

void NodeProperty::addConnection(ConnectionLine *nl) {
	_connectionList.push_back(nl);
}

QString NodeProperty::getType() const {
	QString tt;
	if (_pFile) {
		tt = _pFile->getType(_node->getInstanceName()+"."+_name);
		tt.replace("<","&lt;").replace(">","&gt;");
	}
	return tt;
}

QRectF NodeProperty::boundingRect() const {
	int yy = 28 + _propNr * 25;
	return QRectF(-7, yy-1, _node->getWidth()+13, 21);
}

QRectF NodeProperty::_getSocketRect() const {
	QRectF ret = boundingRect();
	ret.adjust(1,5,-1,-5);
	if (isInput()) {
		ret.setRight(ret.left()+11);
	}
	else {
		ret.setLeft(ret.right()-11);
	}
	return ret;
}

QPointF NodeProperty::getSocketCenter() const {
	return _node->pos() + _getSocketRect().center();
}

void NodeProperty::paint(
		QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
	QRectF inner = boundingRect();
	inner.adjust(12,0,-12,0);
	painter->setPen(QPen(Qt::black,0.5f));
	painter->setOpacity(1);
	painter->setBrush(isUnderMouse() ? Qt::green : Qt::lightGray);
	painter->drawRoundRect(inner, 10, 100);
	painter->setBrush(Qt::black);
	inner.adjust(10,0,-10,0);
	QFont f = painter->font();
	f.setPixelSize(14);
	painter->setFont(f);
	painter->drawText(inner, _name);
	painter->setBrush(_getSocketColor(getType()));
	painter->drawEllipse(_getSocketRect());
}

QString NodeProperty::getFullName() const {
	return _node->getInstanceName() + "." + _name;
}

bool NodeProperty::isInput() const {
	return _isInput;
}

void NodeProperty::hoverEnterEvent(QGraphicsSceneHoverEvent* ev) {
	setToolTip(QString(
			"<p style='white-space:pre'><b>Slot: <i>%1</i><br>"
			"Type:</b><br>%2</p>").arg(_name).arg(getType()));

	changeConnectionLineColor(Qt::green);
	QGraphicsItem::hoverEnterEvent(ev);
}

void NodeProperty::hoverLeaveEvent(QGraphicsSceneHoverEvent* event){
	changeConnectionLineColor(Qt::black, false);
	QGraphicsItem::hoverLeaveEvent(event);
}

void NodeProperty::changeConnectionLineColor(QColor lineColor, bool isHover){
	QListIterator<ConnectionLine *> cl(_connectionList);
	ConnectionLine* connectedLine;
	while (cl.hasNext()) {
		connectedLine = cl.next();
		// reset color to blue if not hovering and one of the parents is
		// selected
		if(!isHover && (
				connectedLine->getStartProp()->_node->isSelectedNode() ||
				connectedLine->getEndProp()->_node->isSelectedNode())) {
			connectedLine->setLineColor(Qt::blue);
		}
		// else use given color (i.e. black/green)
		else{
			connectedLine->setLineColor(lineColor);
		}
	}
}

QColor NodeProperty::_getSocketColor(QString tName) {
	if (tName.contains("cimg",Qt::CaseInsensitive)) {
		return Qt::yellow;
	}
	else if (tName.contains("vigra",Qt::CaseInsensitive)) {
		return Qt::darkYellow;
	}
	else if(tName.contains("roi",Qt::CaseInsensitive)) {
		return Qt::green;
	}
	else if(tName.contains("interpolator",Qt::CaseInsensitive)) {
		return Qt::blue;
	}

	// add more color presets here
	return Qt::gray;
}
