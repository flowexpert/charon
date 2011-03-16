/*	Copyright (C) 2011 Jonathan Wuest

	This file is part of Tuchulcha.

    Tuchulcha is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tuchulcha is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file   NodeTreeView.cpp
 *  @brief  Implementation of class NodeTreeView
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#include "NodeTreeView.h"
#include <QVector>
#include "NodeProperty.h"
#include "Node.h"
#include "ParameterFileModel.h"
#include "TreeViewItem.h"
#include "FileManager.h"
#include "MetaData.h"
#include "NodeTreeView.moc"
#include <iostream>
using namespace std;

NodeTreeView::NodeTreeView(QWidget *parent) : QTreeView(parent) {
	this->setAnimated(true);
	model = new QStandardItemModel(1,1);
	this->setModel(model);
	root = new QStandardItem("Modules");
	root->setDragEnabled(false);
	model->setItem(0,0,root);
	model->setHorizontalHeaderItem(0,new QStandardItem("Modules"));
	this->setDragEnabled(true);
	load(FileManager::instance().classesFile());
}

void NodeTreeView::load(QString classesFile){
	MetaData md(classesFile.toStdString());
	vector<string> classes = md.getClasses();
	for(unsigned int i=0;i<classes.size();i++){
		QString name = QString::fromStdString(classes[i]);
		vector<string> ins = md.getInputs(classes[i]);
		vector<string> outs = md.getOutputs(classes[i]);
		vector<string> params = md.getParameters(classes[i]);
		Node *node = new Node();
		node->setName(name);
		for(unsigned int j=0;j<ins.size();j++){
			node->addProperty(QString::fromStdString(ins[j]),QString::fromStdString(md.getType(ins[j],classes[i])),PropType::IN);
		}
		for(unsigned int j=0;j<outs.size();j++){
			node->addProperty(QString::fromStdString(outs[j]),QString::fromStdString(md.getType(outs[j],classes[i])),PropType::OUT);
		}
		for(unsigned int j=0;j<params.size();j++){
			node->addProperty(QString::fromStdString(params[j]),QString::fromStdString(md.getType(params[j],classes[i])),PropType::NONE);
		}
		this->addNode(node);
	}
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

void NodeTreeView::mousePressEvent(QMouseEvent *event){
    QTreeView::mousePressEvent(event);
    TreeViewItem *sel = this->getSelectedItem();
    if(sel) emit this->showClassDoc(sel->getNode()->getName());
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

