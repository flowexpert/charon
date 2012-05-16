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
#include <QSortFilterProxyModel>
#include <QVBoxLayout>

/// filter proxy subclass keeping all top-level entities
class MySortFilterProxy : public QSortFilterProxyModel {
protected:
	/// include top-level entities in the model
	virtual bool filterAcceptsRow(
		int source_row, const QModelIndex& source_parent) const {
		return source_parent.isValid() ?
			QSortFilterProxyModel::filterAcceptsRow(source_row,source_parent) :
			true;
	}
};

NodeTreeView::NodeTreeView(QWidget* pp) : QWidget(pp) {
	_ui = new Ui::NodeTreeView;
	_ui->setupUi(this);
	_model = new QStandardItemModel(1,1);
	_filter = new MySortFilterProxy();
	_ui->treeView->setAnimated(true);
	_filter->setSourceModel(_model);
	_filter->setFilterCaseSensitivity(Qt::CaseInsensitive);
	_filter->setSortCaseSensitivity(Qt::CaseInsensitive);
	_filter->setDynamicSortFilter(true);
	_ui->treeView->setModel(_filter);
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
	QStandardItem* root = new QStandardItem(tr("Modules"));
	root->setDragEnabled(false);
	root->setEditable(false);
	root->setSelectable(false);
	_model->setItem(0,0,root);
	QStringList labels;
	labels << tr("Names");
	_model->setHorizontalHeaderLabels(labels);

	// load modules
	MetaData md(FileManager::instance().classesFile());
	const QStringList& classes = md.getClasses();
	foreach (const QString& cur, classes) {
		// new node to be added
		QStandardItem* node = new QStandardItem(cur);
		node->setEditable(false);
		node->setDragEnabled(true);
		root->appendRow(node);
	}

	// update view
	_ui->treeView->setExpanded(_filter->mapFromSource(root->index()), true);
	_ui->treeView->sortByColumn(0,Qt::AscendingOrder);
}

void NodeTreeView::on_treeView_clicked(const QModelIndex& cur) {
	// check if a module is selected, if below, go up to module name
	// show doc for selected module
	if (cur.parent().isValid()) {
		emit showClassDoc(_filter->data(cur).toString());
	}
}

void NodeTreeView::on_editFilter_textChanged(const QString& text) {
	_filter->setFilterWildcard(text);
}

#include "NodeTreeView.moc"
