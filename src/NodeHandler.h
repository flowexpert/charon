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
/** @file   NodeHandler.h
 *  @brief  Declaration of class NodeHandler
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */
#ifndef NODEHANDLER_H_
#define NODEHANDLER_H_

#include <QGraphicsScene>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QKeyEvent>

class NodeProperty;
class ConnectionLine;
class Node;
class GraphModel;

class NodeHandler: public QGraphicsScene {
	Q_OBJECT

public:
	NodeHandler(QObject *parent,QString classesFile);
	void wheelEvent(QGraphicsSceneWheelEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void keyReleaseEvent(QKeyEvent * keyEvent);
	void addNode(QString name, QPointF pos);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void connectNodes(QString node0,QString prop0,QString node1,QString prop1);
	void toXMLFile(QString filename);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void deleteNode(Node *node);
	void setModel(GraphModel *model);
	GraphModel *model();
	void loadFromModel(); // fehlt: Verbindungen laden
	void load(QString fname);
	void dragEnterEvent(QGraphicsSceneDragDropEvent * event);
	void dropEvent(QGraphicsSceneDragDropEvent * event);
	void dragMoveEvent(QGraphicsSceneDragDropEvent * event);

	virtual ~NodeHandler();
private:
	void deselectAllNodes();
	ConnectionLine *cline;
	NodeProperty *startProp;
	Node* selectedNode;
	GraphModel *_model;
	qreal scaleFactor;
	bool addLine;
};

#endif /* NODEHANDLER_H_ */
