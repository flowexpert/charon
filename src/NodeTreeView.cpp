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
#include "ui_NodeTreeView.h"
#include <QTreeView>
#include <QStandardItemModel>
#include <QVBoxLayout>

NodeTreeView::NodeTreeView(QWidget* pp) : QWidget(pp) {
	_ui = new Ui::NodeTreeView;
	_ui->setupUi(this);
	_model = new QStandardItemModel(1,3,_ui->treeView);
	_ui->treeView->setAnimated(true);
	_ui->treeView->setModel(_model);
	_ui->treeView->setDragEnabled(true);
	_ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
	reload();
}

NodeTreeView::~NodeTreeView() {
	delete _ui;
}

void NodeTreeView::reload() {
	// reset content
	_model->clear();
	QList<QStandardItem*> root;
	root << new QStandardItem(tr("Modules"));
	root << new QStandardItem;
	root << new QStandardItem;
	for(int ii=0; ii<root.size(); ii++) {
		root[ii]->setDragEnabled(false);
		root[ii]->setEditable(false);
		root[ii]->setSelectable(false);
		_model->setItem(0,ii,root[ii]);
	}
	QStringList labels;
	labels << tr("Name") << tr("Parameter/Slot") << tr("Type");
	_model->setHorizontalHeaderLabels(labels);
	_model->horizontalHeaderItem(0)->setSizeHint(QSize(300,0));
	_model->horizontalHeaderItem(1)->setSizeHint(QSize(200,0));
	_ui->treeView->resizeColumnToContents(0);
	_ui->treeView->resizeColumnToContents(1);

	// load modules
	MetaData md(FileManager::instance().classesFile());
	QStringList classes = md.getClasses();
	for (int ii=0; ii < classes.size(); ii++) {
		QString name = classes[ii];
		QStringList ins = md.getInputs(classes[ii]);
		QStringList outs = md.getOutputs(classes[ii]);
		QStringList params = md.getParameters(classes[ii]);

		// new node to be added
		QStandardItem* node = new QStandardItem(name);
		node->setEditable(false);
		node->setDragEnabled(true);
		QList<QStandardItem*> names, sTypes, cTypes, all;

		for (int jj=0; jj<ins.size(); jj++) {
			names << new QStandardItem(ins[jj]);
			sTypes << new QStandardItem(tr("InputSlot"));
			cTypes << new QStandardItem(md.getType(ins[jj],classes[ii]));
		}
		for (int jj=0; jj<outs.size(); jj++) {
			names << new QStandardItem(outs[jj]);
			sTypes << new QStandardItem(tr("OutputSlot"));
			cTypes << new QStandardItem(md.getType(outs[jj],classes[ii]));
		}
		for (int jj=0; jj<params.size(); jj++) {
			names << new QStandardItem(params[jj]);
			sTypes << new QStandardItem(tr("Parameter"));
			cTypes << new QStandardItem(md.getType(params[jj],classes[ii]));
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
		root[0]->appendRow(node);
	}
	root[0]->sortChildren(0);
	for(int ii=0; ii < root[0]->rowCount(); ii++) {
		root[0]->child(ii)->sortChildren(1);
	}
	_ui->treeView->setExpanded(root[0]->index(), true);
}

void NodeTreeView::on_treeView_clicked(const QModelIndex& current) {
	// check if a module is selected, if below, go up to module name
	// show doc for selected module
	if (current.parent().isValid()) {
		QModelIndex idx = current;
		while (idx.parent().parent().isValid()) {
			idx = idx.parent();
		}
		emit showClassDoc(_model->data(idx).toString());
	}
}

void NodeTreeView::on_editFilter_textChanged(const QString& text) {
	QRegExp rgx(text, Qt::CaseInsensitive, QRegExp::WildcardUnix);
	for(int ii=0; ii < _model->rowCount(); ii++) {
		QModelIndex root = _model->index(ii,0);
		for(int sub=0; root.child(sub,0).isValid(); sub++) {
			QModelIndex cur = root.child(sub,0);
			if(text.isEmpty()) {
				_ui->treeView->setRowHidden(sub,root,false);
			}
			else {
				QString cont = cur.data().toString();
				_ui->treeView->setRowHidden(sub,root,!cont.contains(rgx));
			}
		}
	}
}

#include "NodeTreeView.moc"
