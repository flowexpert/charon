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
class ParameterFileModel;

/// Property(/Parameter) of a node
class NodeProperty : public QGraphicsItem {
public:
	/// default constructor
	/** \param parentNode      the node which the property belongs to
	 *  \param name            name of the property
	 *  \param propNr          number of the property
	 *                         (needed for correct positioning)
	 *  \param input           input output type of the property
	 *  \param pFile           link to given parameter file
	 */
	NodeProperty(
			Node* parentNode, QString name,
			int propNr, bool input,
			const ParameterFileModel* pFile);

	/// area for hover event
	virtual QRectF boundingRect() const;

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

	/// property name including node instance name
	QString getFullName() const;

	/// get property type
	QString getType() const;

	/// checks if connection is possible
	/// @param prop     property to connect to
	/// @return  true if connection is possible
	bool canConnect(NodeProperty* prop);

	/// socket center including node position
	QPointF getSocketCenter() const;

	/// draws the property
	void paint(
			QPainter* painter,
			const QStyleOptionGraphicsItem* option,
			QWidget* widget = 0);

	/// return the list of connectionlines (my implementation)
	QList<ConnectionLine*> getConnectionLine();

	/// change color all connected line of hovering property 
	/// isHover set to true when the property was hovered
	void changeColorHoverProperty(QColor lineColor, bool isHover=true);

protected:
	/// update tool tip
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);

	/// clear color of connected line when leave hoverevent
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

private:
	/// number of property
	unsigned int _propNr;

	/// name of the property
	QString _name;

	/// list of all connectionlines
	QList<ConnectionLine *> _connectionList;

	/// pointer to PropType of the property
	bool _isInput;

	/// pointer to parent node
	Node* _node;

	/// link to parameter file
	const ParameterFileModel* _pFile;

	/// center of the socket, used to draw end and start of connectionline
	QRectF _getSocketRect() const;

	/// get color from type
	static QColor _getSocketColor(QString typeName);
};

#endif /* NODEPROPERTY_H_ */
