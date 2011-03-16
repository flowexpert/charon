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
/** @file   NodeProperty.h
 *  @brief  Declaration of class NodeProperty
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#ifndef NODEPROPERTY_H_
#define NODEPROPERTY_H_

#include <QGraphicsItem>
#include <QList>
#include "ParameterType.h"


struct PropType {
	enum NodePropertyIOType {
		IN,
		OUT,
		NONE // Parameter for Node - no input or output
	} iotype;
	ParameterType* ptype;
};

class ConnectionLine;
class Node;
class ConnectionSocket;
class GraphModel;

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
	void removeAllConnections(GraphModel* model=0);
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
