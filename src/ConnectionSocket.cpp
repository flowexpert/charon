/*
 * ConnectionSocket.cpp
 *
 *  Created on: 07.10.2010
 *      Author: jonathan
 */

#include "NodeProperty.h"
#include "ConnectionSocket.h"
#include <QPainter>

ConnectionSocket::ConnectionSocket() {
}

void ConnectionSocket::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	NodeProperty *np = dynamic_cast<NodeProperty*>(this->parentItem());
	if(np != 0)
	{
		painter->setBrush(np->getPropType()->ptype->getColor());
		painter->drawEllipse(center,6,6);
	}
}

QPointF ConnectionSocket::getCenter()
{
	return this->center;
}

ConnectionSocket::ConnectionSocket(QGraphicsItem *parent,QPointF center) : QGraphicsItem(parent)
{
	this->center = center;
}

QRectF ConnectionSocket::boundingRect() const
{
	return QRectF(this->center.x()-5,this->center.y()-5,10,10);
}

ConnectionSocket::~ConnectionSocket() {
	// TODO Auto-generated destructor stub
}
