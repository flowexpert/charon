/*
 * Node.h
 *
 *  Created on: 29.09.2010
 *      Author: jonathan
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
	void setGScene(QGraphicsScene *parent);
	QRectF boundingRect() const;
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void addProperty(QString name,QString typeName,PropType::NodePropertyIOType iotype=PropType::NONE);
	void loadFromFile(QString fname);
	void generateFile(QString fname);
	void setId(unsigned int id);
	void loadFromParameterFile(QString fname);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
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
