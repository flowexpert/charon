/*
 * NodeHandler.h
 *
 *  Created on: 29.09.2010
 *      Author: jonathan
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

class NodeHandler: public QGraphicsScene {
public:
    NodeHandler(QObject *parent);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void keyReleaseEvent(QKeyEvent * keyEvent);
    void addNode(QString name, QPointF pos);
    bool connectNodeProperties(unsigned int node0,unsigned int node1,unsigned int prop0,unsigned int prop1);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void toXMLFile(QString filename);
    void fromXMLFile(QString filename);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void deleteNode(Node *node);
    void dragEnterEvent(QGraphicsSceneDragDropEvent * event);
    void dropEvent(QGraphicsSceneDragDropEvent * event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent * event);

    virtual ~NodeHandler();
private:
    void deselectAllNodes();
    ConnectionLine *cline;
    NodeProperty *startProp;
    Node* selectedNode;
    qreal scaleFactor;
    bool addLine;
};

#endif /* NODEHANDLER_H_ */
