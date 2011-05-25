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
class Node;

/// Line that connects two slots of two different nodes
class ConnectionLine : public QGraphicsItem {
	/// create color property for read and write color
	Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor)

public:
	/// Default constructor
	/// @param scene parent scene to use
	ConnectionLine(QGraphicsScene *scene);

	/// bounding Rect, always 0 to be not selectable
	QRectF boundingRect() const;

	/// sets start point (for drawing)
	/// @param x     x coordinate of start point
	/// @param y     y coordinate of start point
	void setStartPoint(int x,int y);

	/// sets start point
	/// @param pos   start point
	void setStartPoint(QPointF pos);

	/// sets end point (for drawing)
	/// @param x     x coordinate of end point
	/// @param y     y coordinate of end point
	void setEndPoint(int x,int y);

	/// sets end point
	/// @param pos     end point
	void setEndPoint(QPointF pos);

	/// sets start and end property
	/// @param start  start property
	/// @param end    end property
	void setStartEndProp(NodeProperty *start,NodeProperty *end);

	/// paints the line
	void paint(
                        QPainter *painter, const QStyleOptionGraphicsItem *option,
                        QWidget *widget = 0);

	/// get property at end of line
	/// @return   property at end of line
	NodeProperty *getEndProp() const;

	/// get property at start of line
	/// @return   property at start of line
	NodeProperty *getStartProp() const;

	/// sets end property
	/// @param endProp   property at end of line
	void setEndProp(NodeProperty *endProp);

	/// sets start property
	/// @param startProp   property at start of line
	void setStartProp(NodeProperty *startProp);

	/// moves start point
	/// @param dx    x coordinate of movement vector
	/// @param dy    y coordinate of movement vector
	void moveStartPoint(qreal dx,qreal dy);

	/// moves end point
	/// @param dx    x coordinate of movement vector
	/// @param dy    y coordinate of movement vector
	void moveEndPoint(qreal dx,qreal dy);

    /// set color connected line
	void setLineColor(const QColor &newColor);

	/// return line color
	QColor lineColor() const;

private:
	/// point where line starts
	QPointF _startPoint;

	/// point where line ends
	QPointF _endPoint;

	/// property where line starts
	NodeProperty *_startProp;

	/// property where line ends
	NodeProperty *_endProp;

	///store color
	QColor _lColor;

};

#endif /* CONNECTIONLINE_H_ */
