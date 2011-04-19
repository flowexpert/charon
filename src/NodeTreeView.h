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
/** @file   NodeTreeView.h
 *  @brief  Declaration of class NodeTreeView
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#ifndef NODETREEVIEW_H
#define	NODETREEVIEW_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>

class ParameterFileModel;
class Node;
class TreeViewItem;


/// QTreeView for module selection
class NodeTreeView : public QTreeView {
	Q_OBJECT

public:
	/// default constructor
	NodeTreeView(QWidget* parent = 0);

public slots:
	/// loads nodes/modules from the classesFile
	void reload();

signals:
	/// show documentation page
	/// @param className    class to show doc for
	void showClassDoc(const QString& className);

protected:
	/// handle documentation display on selection change
	virtual void currentChanged(
			const QModelIndex& current, const QModelIndex& previous);

private:
	/// the model of the view
	QStandardItemModel* _model;
};

#endif	/* NODETREEVIEW_H */

