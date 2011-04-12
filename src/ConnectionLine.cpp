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




ConnectionLine::ConnectionLine() : QGraphicsItem()
{

}
ConnectionLine::ConnectionLine(QGraphicsScene* scene) : QGraphicsItem(0,scene)
{
	this->setZValue(-1);
	this->_startProp = 0;
	this->_endProp = 0;
	this->setZValue(-1);
}

QRectF ConnectionLine::boundingRect()  const
{
	return QRectF(_startPoint,_endPoint);
}

void ConnectionLine::setEndPoint(int x, int y)
{
	this->_endPoint.setX(x);
	this->_endPoint.setY(y);
}

void ConnectionLine::setStartPoint(int x, int y)
{
	this->_startPoint.setX(x);
	this->_startPoint.setY(y);
}

void ConnectionLine::setStartEndProp(NodeProperty *start,NodeProperty *end)
{
	this->_startProp = start;
	this->_endProp = end;
}

void ConnectionLine::setStartPoint(QPointF pos)
{
	this->_startPoint = pos;
}

void ConnectionLine::setEndPoint(QPointF pos)
{
	this->_endPoint = pos;
}

void ConnectionLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->setBrush(Qt::black);
	QPainterPath path;
	path.moveTo(_startPoint);
	QPointF c0((_startPoint.x()+50),_startPoint.y());
	QPointF c1(_endPoint.x()-50,_endPoint.y());
	path.cubicTo(c0,c1,_endPoint);
	painter->setBrush(Qt::NoBrush);
	/*for(int i=0;i<5;i++)
	{
		painter->setPen(QPen(Qt::black,11-i*2));
		painter->setOpacity(0.1);
		painter->drawPath(path);
	}*/
	QPen p(Qt::black,2);
	if(this->_startProp == 0 || this->_endProp == 0){p.setBrush(Qt::gray); p.setWidth(2); p.setStyle(Qt::DashLine);}
	painter->setPen(p);
	painter->setOpacity(1);
	painter->drawPath(path);
}

ConnectionLine::~ConnectionLine() {
	// TODO Auto-generated destructor stub
}

NodeProperty *ConnectionLine::getEndProp() const
{
	return _endProp;
}

NodeProperty *ConnectionLine::getStartProp() const
{
	return _startProp;
}

void ConnectionLine::setEndProp(NodeProperty *endProp)
{
	this->_endProp = endProp;
}

void ConnectionLine::setStartProp(NodeProperty *startProp)
{
	this->_startProp = startProp;
}

void ConnectionLine::moveStartPoint(qreal dx,qreal dy)
{
	this->_startPoint += QPointF(dx,dy);
}

void ConnectionLine::moveEndPoint(qreal dx,qreal dy)
{
	this->_endPoint += QPointF(dx,dy);
}
