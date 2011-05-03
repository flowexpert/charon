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
/** @file   ConnectionSocket.h
 *  @brief  Declaration of class ConnectionSocket
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#ifndef CONNECTIONSOCKET_H_
#define CONNECTIONSOCKET_H_

#include <QGraphicsItem>

/// simple class to display the connectable in and output slots
/// for each node and its properties
class ConnectionSocket: public QGraphicsItem {

public:
	/// default constructor
	ConnectionSocket();

	/// default constructor
	ConnectionSocket(QGraphicsItem *parent,QPointF center);

	/// returns the clickable area for each socket
	/// @return returns the clickable area for each socket
	QRectF boundingRect() const;

	/// center of the socket, used to draw end and start of connectionline
	/// @return point at center of socket
	QPointF getCenter();

	/// paints the socket
	void paint(
			QPainter* painter, const QStyleOptionGraphicsItem* option,
			QWidget* widget = 0);

	/// default destructor
	virtual ~ConnectionSocket();
private:
	/// get color from type
	static QColor _getColor(QString typeName);

	/// point at center of the socket
	QPointF _center;
};

#endif /* CONNECTIONSOCKET_H_ */
