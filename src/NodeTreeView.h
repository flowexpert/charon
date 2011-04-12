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
	NodeTreeView(QWidget *parent);

	/// adds a Node to the view
	/// @param n    node to the view
	void addNode(Node *n);

	/// returns the selected item
	/// @returns the selected item
	TreeViewItem *getSelectedItem();

	/// loads nodes/modules from the given classesFile
	/// @param classesFile      filename of classesfile
	void load(QString classesFile);

	/// default destructor
	virtual ~NodeTreeView();
signals:
	/// show documentation page
	/// @param fileName	fileName of doc page
	void showDocPage(const QString& fileName);

	/// show documentation page
	/// @param doc		strint containing help page (html)
	void showClassDoc(const QString& doc);

private:
	/// handles mouse press event
	void mousePressEvent(QMouseEvent * event);

	/// the model of the view
	QStandardItemModel *_model;

	/// the first element of the view
	QStandardItem *_root;

	/// pointer of the used ParameterFileModel
	ParameterFileModel *_pfmodel;
};

#endif	/* NODETREEVIEW_H */

