/*	Copyright (C) 2012 Eric Koenigs

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

/**	\file WorkflowComments.cpp
 *	\brief Implementation of the WorkflowComment widget
 *
 *	\date 26.03.2012
 *	\author <a href=mailto:e.koenigs@stud.uni-heidelberg.de>Eric Koenigs</a>
 */

#include "WorkflowComments.h"

#include <QVBoxLayout>
#include <QRegExp>
#include <QTextEdit>
#include <QMutex>

#include "QParameterFile.h"
#include "ParameterFileModel.h"
#include "ObjectInspector.h"

WorkflowComments :: WorkflowComments(QWidget* myParent) :
	QTextEdit(myParent),
	_model(0),
	_textChangeLock(new QMutex())
{
	setAcceptRichText(false);
	connect(this, SIGNAL(textChanged()), SLOT(save()));
	setEnabled(false);
}

WorkflowComments::~WorkflowComments() {
	delete _textChangeLock;
}

void WorkflowComments :: save() {
	// Get the comment from the editor and escape the newlines to HTML
	QString comment = toPlainText();
	comment.replace(QRegExp("\n"), "<br>");

	if (_model && isEnabled() && _textChangeLock->tryLock()) {
		_model -> setValue("editorcomment", comment);
		_textChangeLock->unlock();
	}

}

void WorkflowComments :: load() {
	// Don't do anything if save() caused load() to be called.
	if (_model && _textChangeLock->tryLock()) {
		// Get the current model
		QString comment = _model -> getValue("editorcomment");

		// Replace HTML newlines with escaped newlines
		comment.replace(QRegExp("<br\\s*/?>", Qt::CaseInsensitive), "\n");

		// Update the text field if it has changed
		if ( toPlainText() != comment ) {
			setPlainText( comment );
		}
		_textChangeLock->unlock();
	}
}


void WorkflowComments :: update( ParameterFileModel* model ) {
	if (_model == model) {
		// model didn't change, NOP
		return;
	}
	// if a model exists, disconnect it,
	// and set the new model as the active model
	if (_model) {
		disconnect(_model, 0, this, 0);
	}

	// assign new model
	_model = model;

	// clear old content
	_textChangeLock->lock();
	clear();
	_textChangeLock->unlock();

	// reconnect the load slot to the new model
	if (_model) {
		connect(_model, SIGNAL(modelReset()), SLOT(load()));
		load();
	}
	setEnabled(_model);
}
