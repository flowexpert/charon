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
/** \file   LogViewTest.h
 *  \brief  Declaration of test class LogViewTest
 *  \date   07.06.2013
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include <QObject>
class QListView;
class QStringListModel;
class QSortFilterProxyModel;

/// test class for the log view widget QCopyListView with a LogViewProxyModel
class LogViewTest: public QObject
{
	Q_OBJECT
	QListView*             _viewWidget; ///< list view
	QStringListModel*      _model;      ///< log model
	QSortFilterProxyModel* _filter;     ///< filtering and coloring

private slots:
	/// setup, load sample log file
	void initTestCase();
	/// final cleanup
	void cleanupTestCase();
	/// cleanup for each test
	void cleanup();
	/// check model data
	void checkData();
	/// check copied content
	void checkCopy();
	/// void check filtering
	void checkFilter();
};

