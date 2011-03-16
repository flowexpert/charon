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
/** @file   ConnectionLine.cpp
 *  @brief  Implementation of class ConnectionLine.
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */
#include "ConnectionLine.h"
#include <QPainter>
#include <QPainterPath>
#include <QBrush>
#include <QRect>
#include <iostream>
using namespace std;
#include "ConnectionSocket.h"
#include "NodeProperty.h"

ConnectionLine::ConnectionLine() : QGraphicsItem() {
}

ConnectionLine::ConnectionLine(QGraphicsScene* sc) :
		QGraphicsItem(0,sc) {
	setZValue(-1);
	startProp = 0;
	endProp = 0;
	setZValue(-1);
}

QRectF ConnectionLine::boundingRect()  const {
	return QRectF(startPoint,endPoint);
}

void ConnectionLine::setEndPoint(int xx, int yy) {
	endPoint.setX(xx);
	endPoint.setY(yy);
}

void ConnectionLine::setStartPoint(int xx, int yy) {
	startPoint.setX(xx);
	startPoint.setY(yy);
}

void ConnectionLine::setStartEndProp(NodeProperty* start, NodeProperty* end) {
	startProp = start;
	endProp = end;
}

void ConnectionLine::setStartPoint(QPointF p) {
	startPoint = p;
}

void ConnectionLine::setEndPoint(QPointF p) {
	endPoint = p;
}

void ConnectionLine::paint(
		QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
	painter->setBrush(Qt::black);
	QPainterPath path;
	path.moveTo(startPoint);
	QPointF c0((startPoint.x()+50),startPoint.y());
	QPointF c1(endPoint.x()-50,endPoint.y());
	path.cubicTo(c0,c1,endPoint);
	painter->setBrush(Qt::NoBrush);
	QPen p(Qt::black,2);
	if (startProp == 0 || endProp == 0) {
		p.setBrush(Qt::gray);
		p.setWidth(2);
		p.setStyle(Qt::DashLine);
	}
	painter->setPen(p);
	painter->setOpacity(1);
	painter->drawPath(path);
}

ConnectionLine::~ConnectionLine() {
}

NodeProperty *ConnectionLine::getEndProp() const {
	return endProp;
}

NodeProperty *ConnectionLine::getStartProp() const {
	return startProp;
}

void ConnectionLine::setEndProp(NodeProperty* ep) {
	endProp = ep;
}

void ConnectionLine::setStartProp(NodeProperty* sp) {
	startProp = sp;
}

void ConnectionLine::moveStartPoint(qreal dx,qreal dy) {
	startPoint += QPointF(dx,dy);
}

void ConnectionLine::moveEndPoint(qreal dx,qreal dy) {
	endPoint += QPointF(dx,dy);
}
