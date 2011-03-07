/*
 * ConnectionLine.cpp
 *
 *  Created on: 04.10.2010
 *      Author: jonathan
 */
#include <QPainter>
#include <QPainterPath>
#include <QBrush>
#include <QRect>
#include <iostream>
using namespace std;
#include "ConnectionSocket.h"
#include "NodeProperty.h"
#include "ConnectionLine.h"


ConnectionLine::ConnectionLine() : QGraphicsItem()
{

}
ConnectionLine::ConnectionLine(QGraphicsScene* scene) : QGraphicsItem(0,scene)
{
	this->setZValue(-1);
	this->startProp = 0;
	this->endProp = 0;
}

QRectF ConnectionLine::boundingRect()  const
{
	return QRectF(startPoint,endPoint);
}

void ConnectionLine::setEndPoint(int x, int y)
{
	this->endPoint.setX(x);
	this->endPoint.setY(y);
}

void ConnectionLine::setStartPoint(int x, int y)
{
	this->startPoint.setX(x);
	this->startPoint.setY(y);
}

void ConnectionLine::setStartEndProp(NodeProperty *start,NodeProperty *end)
{
	this->startProp = start;
	this->endProp = end;
}

void ConnectionLine::setStartPoint(QPointF pos)
{
	this->startPoint = pos;
}

void ConnectionLine::setEndPoint(QPointF pos)
{
	this->endPoint = pos;
}

void ConnectionLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->setBrush(Qt::black);
	QPainterPath path;
	path.moveTo(startPoint);
	QPointF c0((startPoint.x()+50),startPoint.y());
	QPointF c1(endPoint.x()-50,endPoint.y());
	path.cubicTo(c0,c1,endPoint);
	painter->setBrush(Qt::NoBrush);
	/*for(int i=0;i<5;i++)
	{
		painter->setPen(QPen(Qt::black,11-i*2));
		painter->setOpacity(0.1);
		painter->drawPath(path);
	}*/
	QPen p(Qt::black,2);
	if(this->startProp == 0 || this->endProp == 0){p.setBrush(Qt::gray); p.setWidth(2); p.setStyle(Qt::DashLine);}
	painter->setPen(p);
	painter->setOpacity(1);
	painter->drawPath(path);
}

ConnectionLine::~ConnectionLine() {
	// TODO Auto-generated destructor stub
}

NodeProperty *ConnectionLine::getEndProp() const
{
    return endProp;
}

NodeProperty *ConnectionLine::getStartProp() const
{
    return startProp;
}

void ConnectionLine::setEndProp(NodeProperty *endProp)
{
    this->endProp = endProp;
}

void ConnectionLine::setStartProp(NodeProperty *startProp)
{
    this->startProp = startProp;
}

void ConnectionLine::moveStartPoint(qreal dx,qreal dy)
{
	this->startPoint += QPointF(dx,dy);
}

void ConnectionLine::moveEndPoint(qreal dx,qreal dy)
{
	this->endPoint += QPointF(dx,dy);
}

