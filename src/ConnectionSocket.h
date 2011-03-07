/*
 * ConnectionSocket.h
 *
 *  Created on: 07.10.2010
 *      Author: jonathan
 */

#ifndef CONNECTIONSOCKET_H_
#define CONNECTIONSOCKET_H_

#include <QGraphicsItem>

class ConnectionSocket: public QGraphicsItem {
public:
	ConnectionSocket();
	ConnectionSocket(QGraphicsItem *parent,QPointF center);
	QRectF boundingRect() const;
	QPointF getCenter();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	virtual ~ConnectionSocket();
private:
	QPointF center;
};

#endif /* CONNECTIONSOCKET_H_ */
