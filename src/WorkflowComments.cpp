/*	Copyright (C) 2012 Eric Koenigs

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

#include "WorkflowComments.moc"

WorkflowComments :: WorkflowComments(
		ObjectInspector* inspector,
		QWidget* myParent) :
	QTextEdit(myParent),
	_inspector(inspector),
	_model(0)
{
	this -> setAcceptRichText(false);

	_textChangeLock = new QMutex();

	connect(this, SIGNAL(textChanged()), SLOT(save()));
}

WorkflowComments::~WorkflowComments() {
	delete _textChangeLock;
}

void WorkflowComments :: save() {
	// Get the comment from the editor and escape the newlines to HTML
	QString comment = this -> toPlainText();
	comment.replace(QRegExp("\n"), "<br>");

	ParameterFileModel* model = _inspector -> model();

	// start editing model
	if (model && isEnabled() && _textChangeLock->tryLock()) {
		// store old values
		QString oldPref = model -> prefix();
		bool oldParam = model -> onlyParams();

		// set them to editable values
		model -> setPrefix( "" );
		model -> setOnlyParams(false);

		// search for the index of the row containing the comment
		int i;
		for ( i = 0; i < model -> rowCount(); ++i ) {
			if (model -> data( model -> index(i, 0)).toString()
					.compare( "editorcomment", Qt::CaseInsensitive ) == 0 ) {
				break;
			}
		}
		// the entry doesn't exist yet, create it
		if ( i >= model -> rowCount() ) {
			model -> insertRow(i);
			model -> setData( model -> index(i, 0), "editorcomment" );
		}

		QString oldV = model->data(model->index(i,1)).toString();
		if (oldV != comment) {
			model -> setData( model -> index(i, 1), comment );
		}

		// restore the old values
		model -> setOnlyParams( oldParam );
		model -> setPrefix( oldPref );
	} // end editing model

	_textChangeLock->unlock();
}

void WorkflowComments :: load() {
	// Don't do anything if save() caused load() to be called.
	if (_textChangeLock->tryLock()) {
		// Get the current model
		ParameterFileModel* model = _inspector -> model();
		if (model) {
			// Get the comment from the parameterfile
			const QParameterFile& pf = model -> parameterFile();
			QString comment = pf.get( "editorcomment" );

			// Replace HTML newlines with escaped newlines
			comment.replace(QRegExp("<br\\s*/?>", Qt::CaseInsensitive), "\n");

			// Update the text field if it has changed
			QString curComment = this -> toPlainText();
			if ( curComment != comment ) {
				this -> setPlainText( comment );
			}
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
