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
#include <QDebug>

/// filter proxy subclass keeping all top-level entities
class MySortFilterProxy : public QSortFilterProxyModel {
public:
	/// default constructor
	MySortFilterProxy(QObject* pp=0) : QSortFilterProxyModel(pp) {}

protected:
	/// include top-level entities in the model
	virtual bool filterAcceptsRow(
		int source_row, const QModelIndex& source_parent) const {
		return !source_parent.isValid() ||
			QSortFilterProxyModel::filterAcceptsRow(source_row,source_parent);
	}
};

/// filer proxy to hide top-level entities without children
class NoEmptyGroupsProxy : public QSortFilterProxyModel {
public:
	/// default constructor
	NoEmptyGroupsProxy(QObject* pp=0) : QSortFilterProxyModel(pp) {}

protected:
	/// hide top-level entities without valid children
	virtual bool filterAcceptsRow(
		int source_row, const QModelIndex& source_parent) const {
		QModelIndex cur = sourceModel()->index(source_row,0,source_parent);
		return source_parent.isValid() || sourceModel()->hasChildren(cur);
	}
};

NodeTreeView::NodeTreeView(QWidget* pp) : QWidget(pp) {
	_ui = new Ui::NodeTreeView;
	_ui->setupUi(this);
	_model = new QStandardItemModel(1,1,this);
	_filter = new MySortFilterProxy(this);
	_hideEmtptyGroups = new NoEmptyGroupsProxy(this);
	_filter->setSourceModel(_model);
	_hideEmtptyGroups->setSourceModel(_filter);
	_filter->setFilterCaseSensitivity(Qt::CaseInsensitive);
	_filter->setSortCaseSensitivity(Qt::CaseInsensitive);
	_ui->treeView->setModel(_hideEmtptyGroups);
	reload();
}

NodeTreeView::~NodeTreeView() {
	delete _ui;
}

void NodeTreeView::reload() {
	// reset content
	_model->clear();
	QStandardItem* root = new QStandardItem(tr("All Modules"));
	root->setDragEnabled(false);
	root->setEditable(false);
	root->setSelectable(false);
	int rootIndex = 0 ;
	_model->setItem(rootIndex++,0,root);

	QStandardItem* untaggedRoot = new QStandardItem(tr("Untagged"));
	untaggedRoot->setDragEnabled(false);
	untaggedRoot->setEditable(false);
	untaggedRoot->setSelectable(false);
	_model->setItem(rootIndex++,0,untaggedRoot);

	QStringList labels;
	labels << tr("Names");
	_model->setHorizontalHeaderLabels(labels);

	QMap<QString,QStandardItem*> allTags ;

	// load modules
	MetaData md(FileManager::instance().classesFile());
	const QStringList& classes = md.getClasses();
	QStringListIterator classIter(classes);
	while (classIter.hasNext()) {
		const QString cur = classIter.next();
		// new node to be added
		QStandardItem* node = new QStandardItem(cur);
		node->setEditable(false);
		node->setDragEnabled(true);
		root->appendRow(node);
		
		// get list of tags for this module
		QStringList tags = md.getTags(cur) ;

		//put module in the untagged list if it has no tags
		if (tags.isEmpty()) {
			QStandardItem* cnode = new QStandardItem(cur);
			cnode->setEditable(false);
			cnode->setDragEnabled(true);
			untaggedRoot->appendRow(cnode);
		}

		QStringListIterator tagIter(tags);
		while (tagIter.hasNext()) {
			const QString tag = tagIter.next();
			int subIndex = 0 ; // index of current "/"
			QString parentTag = "" ;
			// split tag at each "/" and create subnodes
			do {
				QStandardItem* cnode = new QStandardItem(cur);
				cnode->setEditable(false);
				cnode->setDragEnabled(true);

				subIndex = tag.indexOf("/",subIndex+1) ;
				QString subtag = tag.mid(0,subIndex) ; 
				
				if(!allTags.contains(subtag))
				{
					//current innermost tag
					QString subsubtag = subtag.mid(subtag.lastIndexOf("/")+1,-1) ;
					QStandardItem* newRoot = new QStandardItem(subsubtag) ;
					newRoot->setDragEnabled(false);
					newRoot->setEditable(false);
					newRoot->setSelectable(false);
					if(parentTag.isEmpty()) //handle root nodes differently
						_model->setItem(rootIndex++,0,newRoot);
					else
						allTags[parentTag]->appendRow(newRoot) ;
					allTags.insert(subtag,newRoot) ;
				}
				parentTag = subtag ;
				allTags[subtag]->appendRow(cnode) ;
			} while(subIndex != -1) ;
		}
	}
	_model->sort(0,Qt::AscendingOrder);
	_filter->invalidate();
	_hideEmtptyGroups->invalidate();

	// update view
	_ui->treeView->sortByColumn(0,Qt::AscendingOrder);
	_ui->treeView->setExpanded(_ui->treeView->model()->index(0,0), true);
}

void NodeTreeView::on_treeView_clicked(const QModelIndex& cur) {
	if (cur.parent().isValid()) {
		emit showClassDoc(cur.model()->data(cur).toString());
	}
}

void NodeTreeView::on_editFilter_textChanged(const QString& text) {
	_filter->setFilterWildcard(text);
	_hideEmtptyGroups->invalidate();
	_ui->treeView->sortByColumn(0,Qt::AscendingOrder);
	_ui->treeView->setExpanded(_ui->treeView->model()->index(0,0), true);
	_ui->treeView->scrollToTop();
	_ui->editFilter->setFrame(_ui->treeView->model()->rowCount());
}

void NodeTreeView::on_treeView_doubleClicked(const QModelIndex& idx) {
	if (idx.parent().isValid()) {
		emit addNode(idx.model()->data(idx).toString());
	}
}
