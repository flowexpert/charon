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
/** \file   QDirEdit.h
 *  \brief  Declaration of class DirEdit.
 *  \date   15.04.2010
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef QDIREDIT_H
#define QDIREDIT_H

#include <QLineEdit>
class QToolButton;

/// Line edit with completer for directories and dialog option
class QDirEdit : public QLineEdit
{
	Q_OBJECT
private:
	/// browse button
	QToolButton* _browseButton;
	/// look for files instead of directories
	bool _files;
	/// use saveFile dialog instead of openFile dialog
	bool _writeFile;

	/// set initial values
	void _init();

protected:
	/// handle resize events
	void resizeEvent(QResizeEvent* e /** [in] resize event */);

public:
	/// inherited constructor
	explicit QDirEdit(QWidget* parent = 0 /** [in] parent widget */);

	/// inherited constructor setting content
	explicit QDirEdit(
			const QString& content /** [in] text content */,
			QWidget* parent = 0    /** [in] parent widget */);

signals:
	/// emit status of file dialog
	/** i.e. true, if dialog is open and false otherwise */
	void dialogOpen(bool status /** true if dialog opens, false if closed */);

public slots:
	/// show open dialog and use the selected directory as entry
	void fileDialog();

	/// allow files instead of directories
	void acceptFiles(
			bool files=true /** [in] whether to look for files */,
			bool write=true /** [in] show saveFile instead of openFile */);
};

#endif // QDIREDIT_H
