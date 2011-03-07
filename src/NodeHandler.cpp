/*
 * NodeHandler.cpp
 *
 *  Created on: 29.09.2010
 *      Author: jonathan
 */

#include "Node.h"
#include "ConnectionSocket.h"
#include "NodeProperty.h"
#include "ConnectionLine.h"
#include "NodeHandler.h"
#include "NodeTreeView.h"
#include "TypeHandler.h"
#include "TreeViewItem.h"

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include<QList>
#include<QFile>
#include<QTextStream>
#include<QGraphicsView>
#include<QStandardItem>
#include<iostream>
#include<math.h>

using namespace std;
NodeHandler::NodeHandler(QObject *parent) : QGraphicsScene(parent) {
    addLine = false;
    this->cline = NULL;
    scaleFactor = 1;
    TypeHandler::inizialize();
}

void NodeHandler::deselectAllNodes(){
    for(int i=0;i<this->items().size();i++)
        {
            Node *n = dynamic_cast<Node*>(this->items().at(i));
            if(n != 0)
            {
                n->setSelectedNode(false);
            }
        }
    this->selectedNode = 0;
}

void NodeHandler::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    int f = event->delta();
    qreal factor = max(0.7,1.0 + ((double)f * 0.001));
    QList<QGraphicsView*> views(this->views());
    this->scaleFactor*=factor;
    if(views.size()>0)
    {
        views.at(0)->scale(factor,factor);
    }
    QGraphicsScene::wheelEvent(event);
}

void NodeHandler::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->deselectAllNodes();
    QGraphicsItem *itm = this->itemAt(event->scenePos());
    Node *np = dynamic_cast<Node*>(itm);
    if(np != 0)
    {
        this->deselectAllNodes();
        this->selectedNode = np;
        np->setSelectedNode(true);
        /*for(int i=0;i<this->items().size();i++)
        {
            if(this->items().at(i) == np){
                this->items().at(i)->setZValue(1);
                this->items().at(i)->setSelected(true);
            }else{
                this->items().at(i)->setZValue(this->items().at(i)->zValue() - 0.1);
                this->items().at(i)->setSelected(false);
            }

        }*/
        this->setFocusItem(itm);
    }else{
        for(int i=0;i<this->items().size();i++) this->items().at(i)->setSelected(false);  //Deselect all
    }
    ConnectionSocket *cs = dynamic_cast<ConnectionSocket*>(itm);
    if(cs != 0)
    {
        NodeProperty *prop = dynamic_cast<NodeProperty*>(cs->parentItem());
        if(prop->canNewConnect()){
            if(prop->hasConnection()){
                prop->removeAllConnections();
            }else{
                this->startProp = prop;
                this->cline = new ConnectionLine(this);
                this->cline->setStartPoint(event->scenePos().x(),event->scenePos().y());
                this->cline->setEndPoint(event->scenePos().x(),event->scenePos().y());
                this->addLine = true;
            }
        }
    }else{
        QGraphicsScene::mousePressEvent(event);
    }
    update();
}

void NodeHandler::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    /*QGraphicsItem *itm = this->itemAt(event->scenePos());				//TODO move scene
    Node *np = dynamic_cast<Node*>(itm);
    if(np == 0 and event->button() == Qt::LeftButton){
        QRectF rect = this->sceneRect();
        QPointF mv = event->scenePos()-event->lastScenePos();
        cout<<mv.x()<<","<<mv.y()<<endl;
        rect.moveTo(mv);
        this->setSceneRect(rect);
    }*/
    if(addLine)
    {
        if(this->startProp->getPropType()->iotype == PropType::OUT)
        {
            this->cline->setEndPoint(event->scenePos().x(),event->scenePos().y());
        }else{
            this->cline->setStartPoint(event->scenePos().x(),event->scenePos().y());
        }
    }
    else{
        QGraphicsScene::mouseMoveEvent(event);
    }
    update();
}

void NodeHandler::toXMLFile(QString filename)
{
    cout<<"generating XML file!";
    QDomDocument doc("test");
    QDomElement root = doc.createElement("Nodes");
    QDomElement lroot = doc.createElement("Edges");
    int nodeID=0;
    for(int i=0;i<this->items().size();i++)
    {
        Node *n = dynamic_cast<Node*>(this->items().at(i));
        if(n != 0)
        {
            QDomElement nodeElement = doc.createElement("Node");
                        nodeElement.setAttribute("name",n->getName());
            nodeElement.setAttribute("config_file",n->getConfigFileName());
            nodeElement.setAttribute("position_x",n->scenePos().x());
            nodeElement.setAttribute("position_y",n->scenePos().y());
            nodeElement.setAttribute("id",n->getId());
            nodeID++;
            root.appendChild(nodeElement);
        }else{
            ConnectionLine *l = dynamic_cast<ConnectionLine*>(this->items().at(i));
            if(l != 0){
                QDomElement line = doc.createElement("Edge");
                line.setAttribute("node0",l->getStartProp()->getNode()->getId());
                line.setAttribute("prop0",l->getStartProp()->getNr());
                line.setAttribute("node1",l->getEndProp()->getNode()->getId());
                line.setAttribute("prop1",l->getEndProp()->getNr());
                lroot.appendChild(line);
            }
        }
    }
    doc.appendChild(root);
    doc.appendChild(lroot);
    QDomElement editProps = doc.createElement("Editor");
    if(this->views().size()>0)
    {
        editProps.setAttribute("center_x",this->views().at(0)->sceneRect().center().x());
        editProps.setAttribute("center_y",this->views().at(0)->sceneRect().center().y());
        editProps.setAttribute("zoom",this->scaleFactor);
    }

    doc.appendChild(editProps);
    QFile file(filename);
    if( !file.open( QIODevice::WriteOnly ) )
    return ;

    QTextStream ts( &file );
    ts << doc.toString();

    file.close();
    cout<<"   done"<<endl;
}

void NodeHandler::keyReleaseEvent (QKeyEvent * keyEvent )
{
    if(keyEvent->key() == 79)
    {
        this->toXMLFile("test.xml");
    }

    //cout<<keyEvent->key()<<endl;
    if(keyEvent->key() == 76)
    {
        this->fromXMLFile("/home/jonathan/workspace/QTNodeEditor/test.xml");
    }

    if(keyEvent->key() == Qt::Key_Delete && this->selectedNode != 0){
        this->deleteNode(this->selectedNode);
    }
}

void NodeHandler::deleteNode(Node *node){
    for(int i=0;i<this->items().size();i++)
        {
            Node *n = dynamic_cast<Node*>(this->items().at(i));
            if(n != 0)
            {
                if(node->getId() == n->getId()){
                    n->remove();
                    cout<<"deleting:"<<n->getName().toStdString()<<endl;
                    this->removeItem(this->items().at(i));
                    update();
                    break;
                }
            }
        }
}

void NodeHandler::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem *itm = this->itemAt(event->scenePos());
    ConnectionSocket *cs = dynamic_cast<ConnectionSocket*>(itm);
    NodeProperty *prop = 0;
    if(addLine && cs != 0)
    {
        prop = dynamic_cast<NodeProperty*>(cs->parentItem());
        if(prop != 0 && prop != startProp && startProp->canConnect(prop))
        {
            if(startProp->getIOType()==PropType::IN) //swap buffers
            {
                NodeProperty *b = startProp;
                startProp = prop;
                prop = b;
            }
            this->cline->setEndPoint(event->scenePos().x(),event->scenePos().y());
            cout<<"added new line"<<endl;
            cline->setStartEndProp(startProp,prop);
            startProp->addConnection(cline);
            prop->addConnection(cline);
            startProp->moveBy(0,0);
            prop->moveBy(0,0);
            cline = NULL;
            addLine = false;
        }else{
            this->removeItem(cline);
            cline = NULL;
            addLine = false;
        }
    }else if(addLine){
        this->removeItem(cline);
        cline = NULL;
        addLine = false;
    }

    QGraphicsScene::mouseReleaseEvent(event);
    update();
}

void NodeHandler::addNode(QString name, QPointF pos)
{
    new Node(name,pos.x(),pos.y(),this);
}

bool NodeHandler::connectNodeProperties(unsigned int node0,unsigned int node1,unsigned int prop0,unsigned int prop1)
{
    Node *n0=0,*n1=0;
    for(int i=0;i<this->items().size();i++){
        Node *n = dynamic_cast<Node*>(this->items().at(i));
        if(n != 0){
            if(n->getId() == node0) n0 = n;
            if(n->getId() == node1) n1 = n;
        }
    }
    if(n0 == 0 || n1 == 0){return false;}

    //else -> all found -> connect
    NodeProperty *np0,*np1;
    np0 = n0->getProperties().at(prop0);
    np1 = n1->getProperties().at(prop1);
    ConnectionLine *l = new ConnectionLine(this);
    if(np0->getIOType() == PropType::OUT){
        l->setStartEndProp(np0,np1);
    }else{
        l->setStartEndProp(np1,np0);
    }
    np0->addConnection(l);
    np1->addConnection(l);
    update();
    return true;
}

void NodeHandler::fromXMLFile(QString fname)
{
    cout<<"reading from file"<<endl;
    QFile file(fname);
    if( !file.open( QIODevice::ReadOnly) ) {
        cout<<"could not read workflow: "<<fname.toStdString()<<endl;
        return;
    }
    QDomDocument doc;
    doc.setContent(file.readAll());
    QDomElement root = doc.firstChildElement("Nodes");
    if(!root.isNull()){
        QDomElement node = root.firstChildElement("Node");
        while(!node.isNull()){
            int x = node.attribute("position_x").toInt();
            int y = node.attribute("position_y").toInt();
            QString cfile = node.attribute("config_file");
            unsigned int id = node.attribute("id").toUInt();
            Node *n = new Node("",x,y,this);
            n->setId(id);
            //update();
            if(cfile.endsWith(".xml")) n->loadFromFile(cfile);
            else if(cfile.endsWith(".wrp")) n->loadFromParameterFile(cfile);
            node = node.nextSiblingElement("Node");
        }
    }
    root = doc.firstChildElement("Edges");
    cout<<"next tag name is: "<<root.tagName().toStdString()<<endl;
    if(root.tagName() == "Edges"){
        cout<<"blabla1"<<endl;
        QDomElement edge = root.firstChildElement("Edge");
        while(!edge.isNull()){
            cout<<"blabla"<<endl;
            int node0 = edge.attribute("node0").toInt();
            int node1 = edge.attribute("node1").toInt();
            int prop0 = edge.attribute("prop0").toInt();
            int prop1 = edge.attribute("prop1").toInt();
            if(!this->connectNodeProperties(node0,node1,prop0,prop1)) cout<<"could not connect nodes"<<endl;
            edge = edge.nextSiblingElement("Edge");
        }
    }
    file.close();

    this->deselectAllNodes();
}

void NodeHandler::dragEnterEvent(QGraphicsSceneDragDropEvent*){
}

void NodeHandler::dropEvent(QGraphicsSceneDragDropEvent* event){

    NodeTreeView *ntv = (NodeTreeView*)(event->source());
    if(ntv != 0){
        Node *node = new Node("",event->scenePos().x(),event->scenePos().y(),this);
        QString fname = ntv->getSelectedItem()->getInfoString();

        if(fname.endsWith(".xml")) node->loadFromFile(fname);
        else if(fname.endsWith(".wrp")) node->loadFromParameterFile(fname);

        this->deselectAllNodes();
        node->setSelectedNode(true);
        this->selectedNode = node;
    }
}

void NodeHandler::dragMoveEvent(QGraphicsSceneDragDropEvent*){}

NodeHandler::~NodeHandler() {
    // TODO Auto-generated destructor stub
}
