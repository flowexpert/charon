/*  Copyright (C) 2013 Jens-Malte Gottfried

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
/** \file QCopyListView.h
 *  \brief Declaration of class QCopyListView
 *
 *  \date 21.05.2013
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
#ifndef QCOPYLISTVIEW_H
#define QCOPYLISTVIEW_H

#include <QListView>

/// customized variant of QListView with reworked handling of copy shortcut
/** QAbstractItemView only copies one currently active cell to the clipboard.
 *  For a log viewer, also a continguous selection (multiple log lines)
 *  sould be able to be copied into the clipboard. This is handled by
 *  a reworked variant of the keyPressEvent handler.
 */
class QCopyListView : public QListView
{
	Q_OBJECT
public:
	/// default constructor
	explicit QCopyListView(QWidget * parent = 0 /** [in] parent widget */);
	virtual ~QCopyListView();
	/// get mime-data representation of selected indices
	/** \returns a plain text and a html representation*/
	QMimeData* getSelectedContent() const;

protected:
	/// reworked key press event handler able to copy multipe lines
	/// into the clipboard
	virtual void keyPressEvent(QKeyEvent * event /** [in] key event */);
};

#endif // QCOPYLISTVIEW_H
