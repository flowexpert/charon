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

class ConnectionLine;
class Node;
class ConnectionSocket;
class GraphModel;

/// Property(/Parameter) of a node
class NodeProperty : public QGraphicsItem {
public:
	/// default constructor
	/** \param parentNode      the node which the property belongs to
	 *  \param name            name of the property
	 *  \param propNr          number of the property
	 *                         (needed for correct positioning)
	 *  \param propType        property type string
	 *  \param input           input output type of the property
	 */
	NodeProperty(
			QGraphicsItem* parentNode, QString name,
			int propNr, QString propType,
			bool input);

	/// area for hover event
	QRectF boundingRect() const;

	/// moves the property and if connected all connectionlines
	/// @param dx     x coordinate of movement
	/// @param dy     y coordinate of movement
	void moveBy(qreal dx,qreal dy);

	/// add a connectionline to the property
	/// @param cl     ConnectionLine to add
	void addConnection(ConnectionLine *cl);

	/// returns the PropType of the property
	/// @returns the PropType of the property
	bool isInput() const;

	/// returns the name of the property
	/// @returns the name of the property
	QString getName() const;

	/// get property type
	QString getType() const;

	/// returns the number of the property
	/// @return the number of the property
	unsigned int getNr();

	/// checks if connection is possible
	/// @param prop     property to connect to
	/// @return  true if connection is possible
	bool canConnect(NodeProperty* prop);

	/// checks if property can accept more connections
	/// @returns true if property can accept more connections
	bool canNewConnect();

	/// checks wheather property is connected
	/// @returns true if connected
	bool hasConnection();

	/// removes all connections from node
	/// @param model    model where connection will be removed too
	void removeAllConnections(GraphModel* model=0);

	/// node which property is child of
	/// @return node which property is child of
	Node* getNode();

	/// removes given connectionline
	/// @param line     line to delete
	void removeConnection(ConnectionLine *line);

	/// returns all connectionLines of the property
	/// @returns all connectionLines of the property
	QList<ConnectionLine*> getConnections();

	/// draws the property
	void paint(
			QPainter* painter,
			const QStyleOptionGraphicsItem* option,
			QWidget* widget = 0);

	/// default destructor
	virtual ~NodeProperty();
private:
	/// width of the property
	int _width;

	/// state of connection
	bool _isConnected;

	/// true on mouse over
	bool _drawType;

	/// can handle more than one connection
	bool _multiConnect;

	/// number of property
	unsigned int _propNr;

	/// name of the property
	QString _name;

	/// list of all connectionlines
	QList<ConnectionLine *> _connectionList;

	/// pointer to PropType of the property
	bool _isInput;

	/// Property type
	QString _propType;

	/// pointer to parent node
	Node *_node;

	/// the properties socket
	ConnectionSocket *_socket;
};

#endif /* NODEPROPERTY_H_ */
