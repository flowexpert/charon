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
#include "FileManager.h"
#include "MetaData.h"

NodeTreeView::NodeTreeView(QWidget* pp) :
			QTreeView(pp),
			_model(new QStandardItemModel(1,3))
{
	setAnimated(true);
	setModel(_model);
	setDragEnabled(true);
	setSelectionMode(QAbstractItemView::SingleSelection);
	reload();
}

void NodeTreeView::reload() {
	// reset content
	_model->clear();
	QStandardItem* root = new QStandardItem("Modules");
	root->setDragEnabled(false);
	root->setEditable(false);
	root->setSelectable(false);
	_model->setItem(0,0,root);
	QStringList labels;
	labels << "Name" << "Parameter/Slot" << "Type";
	_model->setHorizontalHeaderLabels(labels);
	_model->horizontalHeaderItem(0)->setSizeHint(QSize(300,0));
	_model->horizontalHeaderItem(1)->setSizeHint(QSize(200,0));
	resizeColumnToContents(0);
	resizeColumnToContents(1);

	// load modules
	MetaData md(FileManager::instance().classesFile().toStdString());
	std::vector<std::string> classes = md.getClasses();
	for (size_t ii=0; ii < classes.size(); ii++) {
		QString name = QString::fromStdString(classes[ii]);
		std::vector<std::string> ins = md.getInputs(classes[ii]);
		std::vector<std::string> outs = md.getOutputs(classes[ii]);
		std::vector<std::string> params = md.getParameters(classes[ii]);

		// new node to be added
		QStandardItem* node = new QStandardItem(name);
		node->setEditable(false);
		node->setDragEnabled(true);
		QList<QStandardItem*> names, sTypes, cTypes, all;

		for (size_t jj=0; jj<ins.size(); jj++) {
			names << new QStandardItem(
					QString::fromStdString(ins[jj]));
			sTypes << new QStandardItem("InputSlot");
			cTypes << new QStandardItem(
					QString::fromStdString(md.getType(ins[jj],classes[ii])));
		}
		for (size_t jj=0; jj<outs.size(); jj++) {
			names << new QStandardItem(
					QString::fromStdString(outs[jj]));
			sTypes << new QStandardItem("OutputSlot");
			cTypes << new QStandardItem(
					QString::fromStdString(md.getType(outs[jj],classes[ii])));
		}
		for (size_t jj=0; jj<params.size(); jj++) {
			names << new QStandardItem(
					QString::fromStdString(params[jj]));
			sTypes << new QStandardItem("Parameter");
			cTypes << new QStandardItem(
					QString::fromStdString(md.getType(params[jj],classes[ii])));
		}
		all << names << sTypes << cTypes;
		for (int jj=0; jj < all.size(); jj++) {
			all[jj]->setEditable(false);
			all[jj]->setDragEnabled(false);
			all[jj]->setSelectable(false);
		}
		node->appendColumn(names);
		node->appendColumn(sTypes);
		node->appendColumn(cTypes);
		node->sortChildren(1);
		_model->item(0,0)->appendRow(node);
	}
}

void NodeTreeView::currentChanged(
		const QModelIndex& current, const QModelIndex& previous) {
	QTreeView::currentChanged(current, previous);

	// check if a module is selected, if below, go up to module name
	// show doc for selected module
	if (current.parent() != QModelIndex()) {
		QModelIndex idx = current;
		while (idx.parent().parent() != QModelIndex()) {
			idx = idx.parent();
		}
		emit showClassDoc(_model->data(idx).toString());
	}
}

#include "NodeTreeView.moc"
