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
/** @file   ConnectionLine.h
 *  @brief  Declaration of class ConnectionLine.
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#ifndef CONNECTIONLINE_H_
#define CONNECTIONLINE_H_

#include <QGraphicsItem>
#include <QPointF>

class NodeProperty;

class ConnectionLine : public QGraphicsItem {

public:
	ConnectionLine();
	ConnectionLine(QGraphicsScene *scene);
	QRectF boundingRect() const;
	void setStartPoint(int x,int y);
	void setStartPoint(QPointF pos);
	void setEndPoint(int x,int y);
	void setEndPoint(QPointF pos);
	void setStartEndProp(NodeProperty *start,NodeProperty *end);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	virtual ~ConnectionLine();
	NodeProperty *getEndProp() const;
	NodeProperty *getStartProp() const;
	void setEndProp(NodeProperty *endProp);
	void setStartProp(NodeProperty *startProp);
	void moveStartPoint(qreal dx,qreal dy);
	void moveEndPoint(qreal dx,qreal dy);
private:
	QPointF startPoint;
	QPointF endPoint;
	NodeProperty *startProp;
	NodeProperty *endProp;
};

#endif /* CONNECTIONLINE_H_ */
