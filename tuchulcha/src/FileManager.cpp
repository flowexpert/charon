/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/** \file   FileManager.cpp
 *  \brief  Implementation of class FileManager.
 *  \date   02.12.2008
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "FileManager.h"
#include <QTextStream>

#ifndef TUCHULCHA_DIR
/// Tuchulcha config path
/** Name of the directory where tuchulcha stores
 *  e.g. metadata information.
 *  (QSettings stuff goes somewhere else!)
 */
#define TUCHULCHA_DIR ".tuchulcha"
#endif

FileManager* FileManager::_inst = 0;

QWidget* FileManager::dialogParent = 0;

FileManager::FileManager() {
	// set up tuchulcha dir and path structure
	if (!QDir::home().exists(TUCHULCHA_DIR)) {
		QDir::home().mkdir(TUCHULCHA_DIR);
	}
	QString configPath = QDir::homePath() + "/" + TUCHULCHA_DIR;
	QFileInfo configPathInfo(configPath);
	if (!configPathInfo.exists()) {
		qFatal("%s",
				(tr("Tuchulcha configuration directory \"%1\" "
					"could not be created. Are permissions set correctly?")
				.arg(configPath))
				.toLocal8Bit().constData());
	}
	else if (!configPathInfo.isDir()) {
		qFatal("%s",
				(tr("Tuchulcha configuration directory \"%1\" "
					"exists, but is not a directory. Please delete "
					"\"%1\".")
				.arg(configPath))
				.toLocal8Bit().constData());
	}
	else if (!QFileInfo(configPath+"/metadata").exists()) {
		QDir::home().mkpath(TUCHULCHA_DIR "/metadata");
	}
}

FileManager::~FileManager() {
}

const FileManager& FileManager::instance() {
	if (!_inst) {
		_inst = new FileManager();
	}
	return *_inst;
}

QDir FileManager::configDir() const {
	QDir ret = QDir::home();
	bool ok = ret.cd(TUCHULCHA_DIR);
	if (!ok)
		qFatal("%s", (tr("Failed to change to directory \"%1\".")
				.arg(QDir::homePath() + "/" + TUCHULCHA_DIR))
				.toLocal8Bit().constData());
	return ret;
}

QString FileManager::classesFile() const {
	return QDir::homePath() + "/" + TUCHULCHA_DIR + "/classes.wrp";
}
