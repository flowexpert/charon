/*
 * ConnectionLine.h
 *
 *  Created on: 04.10.2010
 *      Author: jonathan
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
