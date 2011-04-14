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
/** @file   TreeViewItem.h
 *  @brief  Declaration of class TreeViewItem
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#ifndef TREEVIEWITEM_H
#define	TREEVIEWITEM_H

#include <QStandardItem>
#include <QMimeData>
#include <QModelIndexList>

class Node;

/// QStandardItem that uses a node for data generatrion
class TreeViewItem : public QStandardItem{

public:

	/// default constructor
	/// @param node     node to load data from
	TreeViewItem(Node *node);

	/// @returns the nodes config file (if set)
	QString getInfoString();

	/// @returns the set node
	Node *getNode();

	/// default destructor
	virtual ~TreeViewItem();
private:
	/// text of the item
	QString _text;

	/// set node
	Node *_node;

};

#endif	/* TREEVIEWITEM_H */

