/*
 * NodeProperty.h
 *
 *  Created on: 06.10.2010
 *      Author: jonathan
 */

#ifndef NODEPROPERTY_H_
#define NODEPROPERTY_H_

#include <QGraphicsItem>
#include <QList>
#include "ParameterType.h"


struct PropType{
	enum NodePropertyIOType
	{
		IN,
		OUT,
		NONE //Parameter for Node - no input or output
	} iotype;
	ParameterType *ptype;
};

class ConnectionLine;
class Node;
class ConnectionSocket;

class NodeProperty : public QGraphicsItem {
public:
	NodeProperty();
	NodeProperty(QGraphicsItem *parentNode,QString name,int propNr,QString ptype,PropType::NodePropertyIOType p_iotype=PropType::NONE);
	QRectF boundingRect() const;
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void moveBy(qreal dx,qreal dy);
	void addConnection(ConnectionLine *cl);
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	PropType *getPropType();
	QString getName();
	unsigned int getNr();
	PropType::NodePropertyIOType getIOType();
	static QString iotypeToString(PropType::NodePropertyIOType type);
	static PropType::NodePropertyIOType iotypeFromString(QString str);
	bool canConnect(NodeProperty* prop);
	bool canNewConnect();
	bool hasConnection();
	void removeAllConnections();
	Node *getNode();
	void removeConnection(ConnectionLine *line);
	QList<ConnectionLine*> getConnections();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	virtual ~NodeProperty();
private:
	int width;
	bool isConnected;
	bool drawType;
	bool multiConnect;
	unsigned int propNr;
	QString name;
	QList<ConnectionLine *> connectionList;
	PropType *ptype;
	Node *node;
	ConnectionSocket *socket;
};

#endif /* NODEPROPERTY_H_ */
