/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file CombinedLinkEditor.h
 *  Declaration of class CombinedLinkEditor
 *  \author Jens-Malte Gottfried <jmgottfried@web.de>
 *  \date 31.08.2011
 */

#ifndef COMBINEDLINKEDITOR_H
#define COMBINEDLINKEDITOR_H

#include <QLineEdit>
#include "CombinedLinkEditor.h"

class QToolButton;

/// delegate to handle parameter types
class CombinedLinkEditor : public QLineEdit {
	Q_OBJECT
public:
	/// standard constructor
	/** parent is passed to constructor
	 *  \param parent   parent widget
	 */
	CombinedLinkEditor(QWidget* edit, QStringList completerList, QWidget* parent = 0);

	/// gets the non-link custom editor widget
	QWidget* getCustomEdit();

protected:
	/// handle resize events
	virtual void resizeEvent(QResizeEvent* e /** [in] resize event */);

private:
	/// custom line editor for all non-link data
	QWidget* _customEdit;

	

	/// Button for toggling between link edit and normal edit
	QToolButton* _toggleButton;
};

#endif // COMBINEDLINKEDITOR_H
