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
/** @file   ConnectionSocket.cpp
 *  @brief  Implementation of class ConnectionSocket
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#include "NodeProperty.h"
#include "ConnectionSocket.h"
#include <QPainter>

ConnectionSocket::ConnectionSocket() {
}

void ConnectionSocket::paint(
		QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
	NodeProperty *np = dynamic_cast<NodeProperty*>(this->parentItem());
	if (np != 0) {
		painter->setBrush(np->getPropType()->ptype->getColor());
		painter->drawEllipse(center,6,6);
	}
}

QPointF ConnectionSocket::getCenter() {
	return this->center;
}

ConnectionSocket::ConnectionSocket(QGraphicsItem* p,QPointF ct) :
		QGraphicsItem(p) {
	this->center = ct;
}

QRectF ConnectionSocket::boundingRect() const {
	return QRectF(this->center.x()-5,this->center.y()-5,10,10);
}

ConnectionSocket::~ConnectionSocket() {
}
