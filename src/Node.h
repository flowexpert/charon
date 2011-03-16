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
/** @file   Node.h
 *  @brief  Declaration of class Node
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#ifndef NODE_H_
#define NODE_H_

#include<QGraphicsItem>
#include<QRect>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>
#include <QVector>
#include "NodeProperty.h"

class Node : public QGraphicsItem {

public:
	Node();
	Node(QString title,int xpos,int ypos,QGraphicsScene *parent);
	QRectF boundingRect() const;
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void addProperty(
		QString name,
		QString typeName,
		PropType::NodePropertyIOType iotype=PropType::NONE);
	void setId(unsigned int id);
	void setName(QString name);
	void loadFromParameterFile(QString fname);
	void paint(
		QPainter *painter,
		const QStyleOptionGraphicsItem *option,
		QWidget *widget = 0);
	void remove();
	int getWidth();
	int getHeight();
	QString getConfigFileName();
	QVector<NodeProperty*> getProperties();
	unsigned int getId();
	QString getName();
	void setSelectedNode(bool s);
	bool isSelectedNode();
	virtual ~Node();
	void checkWidth();
private:
	QVector<NodeProperty*> properties;
	QString name;
	QString configFileName;
	int width,height;
	int nProps;
	bool selectedNode;
	unsigned int id;
	static unsigned int idCount;

};

#endif /* NODE_H_ */
