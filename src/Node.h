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
/** \file   Node.h
 *  \brief  Declaration of class Node
 *  \date   15.03.2011
 *  \author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#ifndef NODE_H_
#define NODE_H_

#include <QGraphicsItem>
#include <QRect>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>
#include <QVector>
#include <QMouseEvent>
#include "NodeProperty.h"

/// QGraphicsItem that contains nodeproperties and draws the node
class Node : public QGraphicsItem {
public:
	/// default constructor
	/** \param title    name of the node
	 *  \param xpos     x coordinate of upper left corner of the node
	 *  \param ypos     y coordinate of upper left corner of the node
	 *  \param parent   parent graphics scene
	 *  \param pFile    link to parameter file
	 */
	Node(const ParameterFileModel* pFile, QString title,
			int xpos, int ypos, QGraphicsScene* parent);

	/// area where node is clickable
	QRectF boundingRect() const;

	/// handles mouse move events
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

	/// adds a property to the node
	/// @param name     name of parameter
	/// @param input    input or output type of property
	void addProperty(QString name, bool input);

	/// adds a property to the node
	/// @param name     name of parameter
	/// @param displayname     display name of parameter
	/// @param input    input or output type of property
	void addProperty(QString name,QString displayname, bool input);

	/// sets the id of the node
	/// @param id      ID to set
	void setId(unsigned int id);

    /// sets the active parameter active or inactive
    void setActive(bool);

	/// resets the name of the node
	/// @param name    name to set
	void setName(QString name);

	/// paints the node
	void paint(
			QPainter* painter, const QStyleOptionGraphicsItem* option,
			QWidget* widget = 0);

	/// node width
	int getWidth() const;

	/// node height
	int getHeight() const;

	/// get node property
	NodeProperty* getProperty(QString propName) const;

	/// get node id
	unsigned int getId() const;

	/// get node name
	QString getInstanceName();

	/// set node class name
	/// @param modname    name of the module
	void setClassName(QString modname);

	/// get class name
	QString getClassName() const;

	/// sets the node selected or not
	/// @param s    select state
	void setSelectedNode(bool s);

	/// state of node selections
	/// @return state of node selection
	bool isSelectedNode();

	/// change color of connected line
	void changeConnectionLineColor(QColor lineColor);

private:
		/// checks the node width in reference to the node name
	void _checkWidth(int nChars=0);

	/// list of nodeproperties
	QMap<QString,NodeProperty*> _properties;

	/// name of the node
	QString _instanceName;

	/// name of the module the node is representing
	QString _className;

	/// width of the node
	int _width;

	/// height of the node
	int _height;

	/// number of properties in node
	int _nProps;

	/// state of node selection
	bool _selectedNode;

    /// state of activeness
    bool _active;

	///  id of the node
	unsigned int _id;

	/// static id counter for id creation
	static unsigned int _idCount;

	/// link to parameter file
    const ParameterFileModel* _pFile;
};

#endif /* NODE_H_ */
