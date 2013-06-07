/*  Copyright (C) 2013 Jens-Malte Gottfried

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
/** \file   LogViewTest.h
 *  \brief  Declaration of test class LogViewTest
 *  \date   07.06.2013
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include <QObject>
class QListView;
class QStringListModel;

/// test class for the log view widget QCopyListView with a LogViewProxyModel
class LogViewTest: public QObject
{
	Q_OBJECT
	QListView* _viewWidget;
	QStringListModel* _model;

private slots:
	/// setup, load sample log file
	void initTestCase();
	/// cleanup
	void cleanupTestCase();
	/// check model data
	void checkData();
	/// check copied content
	void checkCopy();
};

