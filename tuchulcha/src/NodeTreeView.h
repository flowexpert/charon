/*	Copyright (C) 2011 Jonathan Wuest

	This file is part of Tuchulcha.

	Tuchulcha is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Tuchulcha is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file   NodeTreeView.h
 *  @brief  Declaration of class NodeTreeView
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#ifndef NODETREEVIEW_H
#define NODETREEVIEW_H

#include <QWidget>

class ParameterFileModel;
class Node;
class TreeViewItem;
class QTreeView;
class QStandardItemModel;
class QSortFilterProxyModel;
class QModelIndex;

namespace Ui {
	class NodeTreeView;
}

/// QTreeView for module selection
class NodeTreeView : public QWidget {
	Q_OBJECT

public:
	/// default constructor
	NodeTreeView(QWidget* parent = 0);
	~NodeTreeView();

public slots:
	/// loads nodes/modules from the classesFile
	void reload();
	/// handle documentation display on selection change
	void on_treeView_clicked(const QModelIndex& index);
	/// handle filter changes
	/** the second level of items is filtered by the given text that
	 *  is interpreted as unix wildcard regexp.
	 *  Items matching this regex are shown, others are hidden.
	 *  Given an empty string, all items are shown.
	 *  \param text         filter regexp
	 */
	void on_editFilter_textChanged(const QString& text);
	/// handle double click events
	void on_treeView_doubleClicked(const QModelIndex&);

signals:
	/// show documentation page
	/// @param className    class to show doc for
	void showClassDoc(const QString& className);
	/// initiate adding a new instance
	void addNode(QString type);

private:
	/// designer gui
	Ui::NodeTreeView* _ui;
	/// the model of the view
	QStandardItemModel* _model;
	/// filter wildcard proxy
	QSortFilterProxyModel* _filter;
	/// hide emtpy groups proxy
	QSortFilterProxyModel* _hideEmtptyGroups;
};

#endif	/* NODETREEVIEW_H */

