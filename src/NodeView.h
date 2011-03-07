/*
 * File:   NodeView.h
 * Author: jonathan
 *
 * Created on 17. Februar 2011, 18:06
 */

#ifndef NODEVIEW_H
#define	NODEVIEW_H

#include <QGraphicsView>
#include "NodeHandler.h"

class GraphModel;

class NodeView : public QGraphicsView {
public:
    NodeView(QWidget *parent);
    virtual ~NodeView();
    GraphModel *model();
    void load(QString fname);
    void save();
private:
    NodeHandler *nodehandler;
    GraphModel *_model;
};

#endif	/* NODEVIEW_H */

