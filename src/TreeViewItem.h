/* 
 * File:   TreeViewItem.h
 * Author: jonathan
 *
 * Created on 17. Februar 2011, 18:56
 */

#ifndef TREEVIEWITEM_H
#define	TREEVIEWITEM_H

#include <QStandardItem>
#include <QMimeData>
#include <QModelIndexList>
#include "Node.h"

class TreeViewItem : public QStandardItem{
public:
    TreeViewItem(Node *node);
    QString getInfoString();
    virtual ~TreeViewItem();
private:
    QString text;

};

#endif	/* TREEVIEWITEM_H */

