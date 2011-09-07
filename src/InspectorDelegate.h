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
/** \file InspectorDelegate.h
 *  Declaration of class InspectorDelegate
 *  \author Jens-Malte Gottfried <jmgottfried@web.de>
 *  \date 30.09.2009
 */

//  The filename editor widgets are based on CMake code:
//
//  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
//
//  See CMakeCopyright.txt or
//      http://www.cmake.org/HTML/Copyright.html for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notices for more information.

#ifndef INSPECTORDELEGATE_H
#define INSPECTORDELEGATE_H

#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QCompleter>
class QToolButton;

/// delegate to handle parameter types and offer specialized editors
/** This delegate takes care of showing eanough digits of float/double
 *  values and of file/path specialized editors as well as dropdown selections.
 *  Editors for int/float/double/string/etc. are handled by the
 *  default editors of the QStyledItemDelegate, only the digit number is fixed.
 *
 *  The following special types are recognized:
 *  - \c fileopen, \c openfile: open file editor
 *  - \c filewrite, \c writefile, \c filename: write file editor
 *  - \c path: path selection editor
 *  - <tt>{opt1,opt2,opt3}</tt>: selection editor (dropdown box, replace
 *      \c optX by the string options you want)
 */
class InspectorDelegate : public QStyledItemDelegate {
	Q_OBJECT
public:
	/// standart constructor
	/** parent is passed to QStyledItemDelegate constructor
	 *  \param parent   parent widget
	 */
	InspectorDelegate(QObject* parent = 0);

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

	/// filter editor events
	/** workaround for what looks like a bug in Qt on Mac OS X
	 *  where it doesn't create a QWidget wrapper for the native file dialog
	 *  so the Qt library ends up assuming the focus was lost to something else
	 *  \param object   event object
	 *  \param event    event
	 *  \retval true    event has to be handled
	 */
	bool eventFilter(QObject* object, QEvent* event);

	/// Sets the data for the item at the given index in the model to the
	/// contents of the given editor.
	/** \param editor   editor widget
	 *  \param model    model
	 *  \param index    item index
	 */
	virtual void setModelData (
			QWidget* editor, QAbstractItemModel* model,
			const QModelIndex & index ) const;

protected slots:
	/// set _fileDialogFlag
	void _setFileDialogFlag(bool flag);
protected:
	/// file dialog opened
	bool _fileDialogFlag;

};

#endif // INSPECTORDELEGATE_H
