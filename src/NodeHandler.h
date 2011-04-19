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

/// QGraphicsScene that handles all drag&drop,
/// node move and connect events
class NodeHandler: public QGraphicsScene {
	Q_OBJECT

public:
	/// default constructor
	/// @param parent         parent QObject
	/// @param classesFile    file for module lookup
	NodeHandler(QObject *parent,QString classesFile);

	/// zooms in and out on mousewheel event
	void wheelEvent(QGraphicsSceneWheelEvent *event);

	/// handles mouse press events
	void mousePressEvent(QGraphicsSceneMouseEvent *event);

	/// handles key release events
	/// del -  deletes the selected node
	/// F12 -  renders a workflow to a given png or jpg file
	void keyReleaseEvent(QKeyEvent * keyEvent);

	/// adds a node
	/// @param name    name of the node
	/// @param pos     point where node is added
	void addNode(QString name, QPointF pos);

	/// handles all mousebutton release events
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

	/// connects two node with its slots: node0.prop0 to node1.prop1
	/// @param node0     name of the first node
	/// @param node1     name of the second node
	/// @param prop0     name of the first nodes property
	/// @param prop1     name of the second nodes property
	void connectNodes(QString node0,QString prop0,QString node1,QString prop1);

	/// handles mouse movement
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

	/// deletes a node and all connections to it
	/// @param node     node to delete
	void deleteNode(Node *node);

	/// sets the model for the scene
	/// @param model     model to set
	void setModel(GraphModel *model);

	/// returns the current set GraphModel
	/// @return the current set GraphModel
	GraphModel *model();

	/// loads the scene from the set GraphModel
	void loadFromModel();

	/// loads the GraphModel from the given filename
	/// and calls loadFromModel to fill the scene
	/// @param fname     name of workflow parameter file
	/// @returns    true if file has successfully been loaded
	bool load(QString fname);

	/// handles drop events
	void dropEvent(QGraphicsSceneDragDropEvent * event);

private:
	/// deselects all nodes
	void _deselectAllNodes();

	/// buffered connection line for slot connection drawing
	ConnectionLine *_cline;

	/// buffer that stores the slot connection start property
	NodeProperty *_startProp;

	/// pointer to currently selected node (zero if none selected)
	Node* _selectedNode;

	/// current set GraphModel
	GraphModel *_model;

	/// current scaleFactor
	qreal _scaleFactor;

	/// state of slot connection mode
	bool _addLine;
};

#endif /* NODEHANDLER_H_ */
