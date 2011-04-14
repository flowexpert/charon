/*  Copyright (C) 2011 Jonathan Wuest

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

NodeTreeView::NodeTreeView(QWidget* pp) :
			QTreeView(pp),
			_model(new QStandardItemModel(1,1)),
			_root(new QStandardItem("Modules"))
{
	setAnimated(true);
	setModel(_model);
	_root->setDragEnabled(false);
	_model->setItem(0,0,_root);
	_model->setHorizontalHeaderItem(0,new QStandardItem("Modules"));
	setDragEnabled(true);
	load(FileManager::instance().classesFile());
}

void NodeTreeView::load(QString classesFile){
	MetaData md(classesFile.toStdString());
	std::vector<std::string> classes = md.getClasses();
	for(size_t ii=0; ii < classes.size(); ii++){
		QString name = QString::fromStdString(classes[ii]);
		std::vector<std::string> ins = md.getInputs(classes[ii]);
		std::vector<std::string> outs = md.getOutputs(classes[ii]);
		std::vector<std::string> params = md.getParameters(classes[ii]);
		Node* node = new Node();
		node->setName(name);
		for (unsigned int j=0;j<ins.size();j++) {
			node->addProperty(
					QString::fromStdString(ins[j]),
					QString::fromStdString(md.getType(ins[j],classes[ii])),
					PropType::IN);
		}
		for (unsigned int j=0;j<outs.size();j++) {
			node->addProperty (
					QString::fromStdString(outs[j]),
					QString::fromStdString(md.getType(outs[j],classes[ii])),
					PropType::OUT);
		}
		for (unsigned int j=0;j<params.size();j++) {
			node->addProperty(
					QString::fromStdString(params[j]),
					QString::fromStdString(md.getType(params[j],classes[ii])),
					PropType::NONE);
		}
		addNode(node);
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
		pname += "\t"+props[i]->getName()+
				 "\t["+props[i]->getPropType()->ptype->getTypeName()+"]";
		prop->setText(pname);
		prop->setDragEnabled(false);
		node->appendRow(prop);
	}
	_root->appendRow(node);
	sortByColumn(0,Qt::AscendingOrder);
}

void NodeTreeView::mousePressEvent(QMouseEvent* ev){
	QTreeView::mousePressEvent(ev);
	TreeViewItem *sel = getSelectedItem();
	if(sel) emit showClassDoc(sel->getNode()->getName());
}


TreeViewItem *NodeTreeView::getSelectedItem(){
	QModelIndexList list = selectedIndexes();

	for(int i=0;i<list.size();i++){
		TreeViewItem *tvi = (TreeViewItem*)_root->child(
				list[i].row(),list[i].column());
		if(tvi != 0){
			return tvi;
		}
	}
	return 0;
}

#include "NodeTreeView.moc"
