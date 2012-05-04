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

/**	\file WorkflowComments.h
 *	\brief Declarations of class WorkflowComments
 *
 *	\date 26.03.2012
 *	\author <a href=mailto:e.koenigs@stud.uni-heidelberg.de>Eric Koenigs</a>
 */

#ifndef WORKFLOWCOMMENTS_H_
#define WORKFLOWCOMMENTS_H_

#include <QTextEdit>

class ObjectInspector;
class ParameterFileModel;
class QMutex;

/// A widget to display comments for the current workflow
class WorkflowComments : public QTextEdit {
	Q_OBJECT

	public:
		/**	Default Constructor
		 * \param parent	pointer to the parent widget
		 */
		WorkflowComments(QWidget* parent = 0);

		~WorkflowComments();

	public slots:
		///	save the comment to the model
		void save();

		/// load the comment from the model and
		/// display it in the text field
		void load();

		/// change the active model and call load()
		/// \param model	the new model
		void update( ParameterFileModel* model );
	

	private:
		/// The currently active model.
		ParameterFileModel* _model;

		/**	Stop load() from doing anything while save() is executing.
		 *	This prevents weird behavior of the QTextEdit box due to 
		 *	emitted signals while editting the model.
		 */
		QMutex* _textChangeLock;
};

#endif
