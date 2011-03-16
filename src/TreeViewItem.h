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

class TreeViewItem : public QStandardItem{

public:
	TreeViewItem(Node *node);
	QString getInfoString();
	Node *getNode();
	virtual ~TreeViewItem();
private:
	QString text;
	Node *node;

};

#endif	/* TREEVIEWITEM_H */

