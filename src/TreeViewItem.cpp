/* 
 * File:   TreeViewItem.cpp
 * Author: jonathan
 * 
 * Created on 17. Februar 2011, 18:56
 */

#include "TreeViewItem.h"
#include <iostream>
using namespace std;


TreeViewItem::TreeViewItem(Node *node) : QStandardItem(node->getName()) {
    this->text = node->getConfigFileName();
    
}

QString TreeViewItem::getInfoString(){
    return this->text;
}

TreeViewItem::~TreeViewItem() {
}

