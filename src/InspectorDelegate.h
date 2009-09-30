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
/// \file InspectorDelegate.h
/// Declaration of class InspectorDelegate
/// \author Jens-Malte Gottfried <jmgottfried@web.de>
/// \date 30.09.2009

//  The filename editor widgets are based on CMake code:
//
//  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
//
//  See CMakeCopyright.txt or http://www.cmake.org/HTML/Copyright.html for details.
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

/// delegate to handle parameter types
class InspectorDelegate : public QStyledItemDelegate {
	Q_OBJECT
public:
	/// standart constructor
	///
	/// parent is passed to QStyledItemDelegate constructor
	/// \param parent	parent widget
	InspectorDelegate(QObject* parent = 0);

	/// create own editors, if necessary
	///
	/// Returns the widget used to edit the item specified by index for
	/// editing. The parent widget and style option are used to control how
	/// the editor widget appears.
	/// \param parent	parent widget
	/// \param option	style option
	/// \param index	model index
	virtual QWidget* createEditor(QWidget* parent,
		const QStyleOptionViewItem& option,
		const QModelIndex& index ) const;

	/// handle editor events
	virtual bool editorEvent (QEvent* event, QAbstractItemModel* model,
			const QStyleOptionViewItem& option, const QModelIndex& index);
	/// filter editor events
	///
	/// workaround for what looks like a bug in Qt on Mac OS X
	/// where it doesn't create a QWidget wrapper for the native file dialog
	/// so the Qt library ends up assuming the focus was lost to something else
	/// \param object	event object
	/// \param event	event
	bool eventFilter(QObject* object, QEvent* event);

protected slots:
	/// set _fileDialogFlag
	void _setFileDialogFlag(bool flag);
protected:
	/// file dialog opened
	bool _fileDialogFlag;

};

/// Editor widget for editing paths or file paths
///
/// based on class QCMakeFileEditor from CMake project
class InspectorFileEditor : public QLineEdit {
	Q_OBJECT
public:
	/// standart constructor
	/// \param p	parent widget
	/// \param var	initial variable value
	InspectorFileEditor(QWidget* p, const QString& var);
protected slots:
	/// handle clicks on the tool button
	virtual void chooseFile() = 0;
signals:
	/// signalize creation and destrucion of open file dialog
	void fileDialogExists(bool);
protected:
	/// handle resize events
	void resizeEvent(QResizeEvent* e);
	QToolButton* _toolButton;	///< tool button
	QString _variable;			///< content
};

/// editor widget for editing files
///
/// based on class QCMakePathEditor from CMake project
class InspectorPathEditor : public InspectorFileEditor
{
	Q_OBJECT
public:
	/// standart constructor
	/// \param p	parent widget
	/// \param var	parameter name (used in open dialog title)
	InspectorPathEditor(QWidget* p = 0, const QString& var = QString());
public slots:
	virtual void chooseFile();
};

/// editor widget for editing paths
///
/// based on class QCMakeFilePathEditor from CMake project
class InspectorFilePathEditor : public InspectorFileEditor
{
	Q_OBJECT
public:
	/// standart constructor
	/// \param p	parent widget
	/// \param var	parameter name (used in open dialog title)
	InspectorFilePathEditor(QWidget* p = 0, const QString& var = QString());
public slots:
	virtual void chooseFile();
};

/// completer class that returns native paths
///
/// based on class QCMakeFileCompleter from CMake project
class InspectorFileCompleter : public QCompleter
{
	Q_OBJECT
public:
	/// standart constructor
	/// \param o		parent object
	/// \param dirs		complete directories (true) or filenames (false)
	InspectorFileCompleter(QObject* o, bool dirs);
	/// Returns the path for the given index. The completer object uses this
	/// to obtain the completion text from the underlying model.
	/// \param idx		model index
	virtual QString pathFromIndex(const QModelIndex& idx) const;
};

#endif // INSPECTORDELEGATE_H
