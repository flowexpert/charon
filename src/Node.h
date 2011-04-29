/*	Copyright (C) 2011 Jonathan Wuest
 *  @file   Node.h
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


/// QGraphicsItem that contains nodeproperties and draws the node
class Node : public QGraphicsItem {
public:
	/// default constructor
	/// @param title    name of the node
	/// @param xpos     x coordinate of upper left corner of the node
	/// @param ypos     y coordinate of upper left corner of the node
	/// @param parent   parent graphics scene
	Node(QString title = "", int xpos=0, int ypos=0,QGraphicsScene* parent=0);

	/// area where node is clickable
	QRectF boundingRect() const;

	/// handles mouse press events
	void mousePressEvent(QGraphicsSceneMouseEvent* event);

	/// handles mouse move events
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

	/// adds a property to the node
	/// @param name     name of parameter
	/// @param typeName type name of parameter
	/// @param iotype   input or output type of property (IN,OUT,NONE)
	void addProperty(
			QString name, QString typeName,
			PropType::NodePropertyIOType iotype=PropType::NONE);

	/// sets the id of the node
	/// @param id      ID to set
	void setId(unsigned int id);

	/// resets the name of the node
	/// @param name    name to set
	void setName(QString name);

	/// paints the node
	void paint(
			QPainter* painter, const QStyleOptionGraphicsItem* option,
			QWidget *widget = 0);

	/// removes all properties correctly
	/// also removes connectionlines of each property
	void remove();

	/// width of the node
	/// @return width of the node
	int getWidth();

	/// height of the node
	/// @return height of the node
	int getHeight();

	/// returns all nodeproperties of the node
	/// @return nodeproperties
	QVector<NodeProperty*> getProperties();

	/// returns the node id
	/// @return node id
	unsigned int getId();

	/// returns the node name
	/// @return the node name
	QString getName();

	/// set the _modulname
	/// @param modname    name of the module
	void setModulName(QString modname);

	/// get _modulname
	/// @returns modulname
	QString getModulName();

	/// sets the node selected or not
	/// @param s    select state
	void setSelectedNode(bool s);

	/// state of node selections
	/// @return state of node selection
	bool isSelectedNode();

	/// default destructor
	virtual ~Node();

private:
	/// cecks the node width in reference to the node name
	void checkWidth();

	/// list of nodeproperties
	QVector<NodeProperty*> _properties;

	/// name of the node
	QString _name;

	/// name of the module the node is representing
	QString _modulname;

	/// width of the node
	int _width;

	/// height of the node
	int _height;

	/// number of properties in node
	int _nProps;

	/// state of node selection
	bool _selectedNode;

	///  id of the node
	unsigned int _id;

	/// static id counter for id creation
	static unsigned int _idCount;
};

#endif /* NODE_H_ */
