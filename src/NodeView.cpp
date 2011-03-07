/*
 * Created on 17. Februar 2011, 18:06
 */

#include <iostream>
#include "NodeView.h"
#include "GraphModel.h"

using namespace std;

NodeView::NodeView(QWidget *parent) : QGraphicsView(parent),_model(0) {
    this->setRenderHints( QPainter::Antialiasing );
    this->setMinimumSize(500,500);
    this->setAcceptDrops(true);
    this->show();
    this->nodehandler = new NodeHandler(this);
    this->setScene(this->nodehandler);
}

GraphModel *NodeView::model(){
    return this->_model;
}

void NodeView::load(QString fname){
    cout<<"(NodeView)loading: "<<fname.toStdString()<<endl;
}

void NodeView::save(){}

NodeView::~NodeView() {
}

