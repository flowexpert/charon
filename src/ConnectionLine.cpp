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
#include "NodeProperty.h"
#include "Node.h"

ConnectionLine::ConnectionLine(QGraphicsScene* sc) :
		QGraphicsItem(0,sc),
		_startProp(0),
		_endProp(0)
{
	setZValue(-1);
	setZValue(-1);
	_lColor = Qt::black;
}

QRectF ConnectionLine::boundingRect() const {
	return QRectF(_startPoint,_endPoint);
}

void ConnectionLine::setEndPoint(int xx, int yy) {
	_endPoint.setX(xx);
	_endPoint.setY(yy);
}

void ConnectionLine::setStartPoint(int xx, int yy) {
	_startPoint.setX(xx);
	_startPoint.setY(yy);
}

void ConnectionLine::setStartEndProp(NodeProperty* start,NodeProperty* end) {
	_startProp = start;
	_endProp = end;
}

void ConnectionLine::setStartPoint(QPointF p) {
	_startPoint = p;
}

void ConnectionLine::setEndPoint(QPointF p) {
	_endPoint = p;
}

QColor ConnectionLine::lineColor() const{
	 return _lColor;
}

void ConnectionLine::setLineColor(const QColor &newColor){
	 _lColor = newColor;
}

void ConnectionLine::paint(
		QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {


    painter->setBrush(Qt::black);
	QPainterPath path;
    path.moveTo(_startPoint);
	QPointF c0((_startPoint.x()+50),_startPoint.y());
	QPointF c1(_endPoint.x()-50,_endPoint.y());
	path.cubicTo(c0,c1,_endPoint);
	painter->setBrush(Qt::NoBrush);
     
    QPen p(_lColor,2);

	if (_startProp == 0 || _endProp == 0) {
			p.setBrush(Qt::gray);
			p.setWidth(2);
			p.setStyle(Qt::DashLine);
	}
	painter->setPen(p);
	painter->setOpacity(1);
	painter->drawPath(path);
}

NodeProperty *ConnectionLine::getEndProp() const{
	return _endProp;
}

NodeProperty *ConnectionLine::getStartProp() const {
	return _startProp;
}

void ConnectionLine::setEndProp(NodeProperty *endProp) {
	_endProp = endProp;
}

void ConnectionLine::setStartProp(NodeProperty *startProp) {
	_startProp = startProp;
}

void ConnectionLine::moveStartPoint(qreal dx,qreal dy) {
	_startPoint += QPointF(dx,dy);
}

void ConnectionLine::moveEndPoint(qreal dx,qreal dy) {
	_endPoint += QPointF(dx,dy);
}
