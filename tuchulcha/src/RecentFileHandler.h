/*  Copyright (C) 2012 Jens-Malte Gottfried

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
/** \file RecentFileHandler.h
 *  \brief RecentFileHandler declaration
 *
 *  \date 11.04.2012
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef RECENTFILEHANDLER_H
#define RECENTFILEHANDLER_H

#include <QObject>

class QAction;
class QMenu;

/// provide list of recent files
class RecentFileHandler : public QObject
{
	Q_OBJECT
private:
	/// maximal number of recent files
	enum {_maxRecentFiles = 5};
	/// strip file name
	/** \param fullFileName    filenane to strip
	 *  \returns               stripped name
	 */
	static QString _strippedName(QString fullFileName);
	/// \name recent file menu entry
	// \{
	/// separator above recent files
	QAction* _separatorAct;
	/// actions to open recent files
	QAction* _recentFileActs[_maxRecentFiles];
	// \}

public:
	/// default constructor
	explicit RecentFileHandler(QObject* parent = 0);
	/// add entries to the given menu
	/** \param menu            menu to add entries to */
	void registerRecentFileEntries(QMenu* menu);

public slots:
	/// insert file as current file to recent file list
	void setCurrentFile(QString fileName);

private slots:
	/// open recent file
	void _openRecentFile();
	/// updated the list of recent files
	void _updateRecentFileActions();

signals:
	/// open file with given name
	void openFile(QString fileName);
};

#endif // RECENTFILEHANDLER_H
