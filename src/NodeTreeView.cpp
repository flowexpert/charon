/*
 * Created on 16. Februar 2011, 17:44
 */

#include "NodeTreeView.h"
#include <QVector>
#include "NodeProperty.h"
#include "Node.h"
#include "TreeViewItem.h"
#include <iostream>
using namespace std;

NodeTreeView::NodeTreeView(QWidget *parent) : QTreeView(parent) {
    this->setAnimated(true);
    model = new QStandardItemModel(1,1);
    this->setModel(model);
    root = new QStandardItem("TestModules");
    root->setDragEnabled(false);
    model->setItem(0,0,root);
    model->setHorizontalHeaderItem(0,new QStandardItem("Modules"));
    this->setDragEnabled(true);
    //inicon = QIcon(QPixmap("./images/inicon.png"));
    //outicon = QIcon(QPixmap("./images/outicon.png"));
}

NodeTreeView::~NodeTreeView() {
}

void NodeTreeView::addNode(Node* n){
    TreeViewItem *node = new TreeViewItem(n);
    node->setEditable(false);
    node->setDragEnabled(true);
    node->sortChildren(0,Qt::DescendingOrder);
    QVector<NodeProperty*> props = n->getProperties();
    for(int i=0;i<props.size();i++){
        QStandardItem *prop = new QStandardItem();
        QString pname = "";
        switch(props[i]->getIOType()){
            case PropType::IN:{pname = "in"; break;}
            case PropType::OUT:{pname = "out"; break;}
            case PropType::NONE:{break;}
        }
        pname += "\t"+props[i]->getName()+ "\t["+props[i]->getPropType()->ptype->getTypeName()+"]";
        prop->setText(pname);
        prop->setDragEnabled(false);
        node->appendRow(prop);
    }
    root->appendRow(node);
    this->sortByColumn(0,Qt::AscendingOrder);
}

TreeViewItem *NodeTreeView::getSelectedItem(){
    QModelIndexList list = this->selectedIndexes();

    for(int i=0;i<list.size();i++){
        TreeViewItem *tvi = (TreeViewItem*)root->child(list[i].row(),list[i].column());
        if(tvi != 0){
            return tvi;
        }
    }
    return 0;
}

/*void NodeTreeView::startDrag(Qt::DropActions ){
    cout<<"starting drag"<<endl;

    QModelIndexList list = this->selectedIndexes();
    for(int i=0;i<list.size();i++){
        TreeViewItem *tvi = (TreeViewItem*)root->child(list[i].row(),list[i].column());
        if(tvi != 0){
            cout<<tvi->getInfoString().toStdString()<<endl;
            QMimeData *md = new QMimeData;
            md->setText(tvi->getInfoString());
            QDrag *drag = new QDrag(this);
            drag->setMimeData(md);
            //drag->set
        }
    }


}*/
