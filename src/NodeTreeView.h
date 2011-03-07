/*
 * File:   NodeTreeView.h
 * Author: jonathan
 *
 * Created on 16. Februar 2011, 17:44
 */

#ifndef NODETREEVIEW_H
#define	NODETREEVIEW_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>

class ParameterFileModel;
class Node;
class TreeViewItem;

class NodeTreeView : public QTreeView {
public:
    NodeTreeView(QWidget *parent);
    void addNode(Node *n);
    //void startDrag(Qt::DropActions supportedActions);
    TreeViewItem *getSelectedItem();
    virtual ~NodeTreeView();
private:
    QIcon inicon,outicon;
    QStandardItemModel *model;
    QStandardItem *root;
    ParameterFileModel *_pfmodel;
};

#endif	/* NODETREEVIEW_H */

