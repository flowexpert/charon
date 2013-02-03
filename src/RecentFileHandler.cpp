/*  Copyright (C) 2012 Jens-Malte Gottfried

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
/** \file RecentFileHandler.cpp
 *  \brief RecentFileHandler implementation
 *
 *  \date 11.04.2012
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "RecentFileHandler.h"

#include <QAction>
#include <QSettings>
#include <QFileInfo>
#include <QMenu>

#include "RecentFileHandler.moc"

RecentFileHandler::RecentFileHandler(QObject* pp) : QObject(pp) {
}

void RecentFileHandler::_openRecentFile() {
QAction *action = qobject_cast<QAction *>(sender());
	if (action) {
		emit openFile(action->data().toString());
	}
}

void RecentFileHandler::registerRecentFileEntries(QMenu *menu) {
	_separatorAct = menu->addSeparator();
	for (int i = 0; i < _maxRecentFiles; ++i) {
		_recentFileActs[i] = new QAction(this);
		_recentFileActs[i]->setVisible(false);
		connect(_recentFileActs[i],
				SIGNAL(triggered()), SLOT(_openRecentFile()));
		menu->addAction(_recentFileActs[i]);
	}
	_updateRecentFileActions();
}

void RecentFileHandler::_updateRecentFileActions() {
	QSettings settings;
	QStringList files = settings.value("recentFileList").toStringList();

	int numRecentFiles = qMin(files.size(), (int)_maxRecentFiles);

	for (int i = 0; i < numRecentFiles; ++i) {
		QString text = QString("&%1 %2")
			.arg(i + 1).arg(_strippedName(files[i]));
		_recentFileActs[i]->setText(text);
		_recentFileActs[i]->setData(files[i]);
		_recentFileActs[i]->setStatusTip(
				tr("Open recent file \"%1\"").arg(files[i]));
		_recentFileActs[i]->setVisible(true);
	}
	for (int j = numRecentFiles; j < _maxRecentFiles; ++j)
		_recentFileActs[j]->setVisible(false);

	_separatorAct->setVisible(numRecentFiles > 0);
}

QString RecentFileHandler::_strippedName(QString fullFileName) {
	return QFileInfo(fullFileName).fileName();
}

void RecentFileHandler::setCurrentFile(QString fileName) {
	if (fileName.isEmpty())
		return;

	QSettings settings;
	QStringList files = settings.value("recentFileList").toStringList();
	QString fname = QFileInfo(fileName).absoluteFilePath();
	files.removeAll(fname);
	files.prepend(fname);
	while (files.size() > _maxRecentFiles)
		files.removeLast();

	settings.setValue("recentFileList", files);

	_updateRecentFileActions();
}

