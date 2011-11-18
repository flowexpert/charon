/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/** \file LinkWrapperDelegate.h
 *  Declaration of class LinkWrapperDelegate
 *  \author Jens-Malte Gottfried <jmgottfried@web.de>
 *  \date 25.08.2011
 */

#ifndef LINKWRAPPERDELEGATE_H
#define LINKWRAPPERDELEGATE_H

#include <QStyledItemDelegate>
#include "InspectorDelegate.h"

/// delegate to handle parameter types
class LinkWrapperDelegate : public InspectorDelegate {
	Q_OBJECT
public:
	/// standart constructor
	/** parent is passed to QStyledItemDelegate constructor
	 *  \param parent   parent widget
	 */
	LinkWrapperDelegate(QObject* parent = 0);

	/// create own editors, if necessary
	/** Returns the widget used to edit the item specified by index for
	 *  editing. The parent widget and style option are used to control how
	 *  the editor widget appears.
	 *  \param parent   parent widget
	 *  \param option   style option
	 *  \param index    model index
	 *  \returns        editor widget
	 */
	virtual QWidget* createEditor(
			QWidget* parent, const QStyleOptionViewItem& option,
			const QModelIndex& index ) const;

	/// Sets the data for the item at the given index in the model to the
	/// contents of the given editor.
	/** \param editor   editor widget
	 *  \param model    model
	 *  \param index    item index
	 */
	virtual void setModelData (
			QWidget* editor, QAbstractItemModel* model,
			const QModelIndex & index ) const;

	/// Sets the editor data for the item at the given index to the
	/// contents of the given editor.
	/** \param editor   editor widget
	 *  \param index    item index
	 */
	virtual void setEditorData (
			QWidget* editor, const QModelIndex & index ) const;
};

#endif // LINKWRAPPERDELEGATE_H
