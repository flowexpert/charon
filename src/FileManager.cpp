/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/** \file   FileManager.cpp
 *  \brief  Implementation of class FileManager.
 *  \date   02.12.2008
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "FileManager.h"
#include <QMessageBox>
#include <QSettings>
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

#ifdef QT_DEBUG
const QString FileManager::privPathTag = "privatePluginPathD";
#else // debug build
const QString FileManager::privPathTag = "privatePluginPath";
#endif

FileManager::FileManager() {
	if (!QDir::home().exists(TUCHULCHA_DIR))
		QDir::home().mkdir(TUCHULCHA_DIR);
	QString configPath = QDir::homePath() + "/" + TUCHULCHA_DIR;
	QFileInfo configPathInfo(configPath);
	if (!configPathInfo.exists())
		qFatal("%s",
				(tr("Tuchulcha configuration directory \"%1\" "
					"could not be created. Are permissions set correctly?")
				.arg(configPath))
				.toAscii().constData());
	if (!configPathInfo.isDir())
		qFatal("%s",
				(tr("Tuchulcha configuration directory \"%1\" "
					"exists, but is not a directory. Please delete "
					"\"%1\"")
				.arg(configPath))
				.toAscii().constData());
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
		qFatal("%s", (tr("failed to change to directory \"%1\"")
				.arg(QDir::homePath() + "/" + TUCHULCHA_DIR))
				.toAscii().constData());
	return ret;
}

QString FileManager::classesFile() const {
	QString path = QDir::homePath() + "/" + TUCHULCHA_DIR + "/classes.wrp";
	if (!QFile(QDir::homePath() + "/" + TUCHULCHA_DIR + "/metadata").exists()) {
		QDir::home().mkpath(TUCHULCHA_DIR "/metadata");
	}
	QFile newFile(path);
	if (!newFile.exists()) {
		// write empty classes file
		if (newFile.open(QFile::WriteOnly | QFile::Truncate)) {
			QTextStream qout(&newFile);
			qout << tr("# empty classes file.") << endl;
		}
		newFile.close();
	}
	return path;
}

void FileManager::configure(bool force) const {
	QSettings settings;

	// check current values
	if(!settings.contains("globalPluginPath") ||
			settings.value("globalPluginPath").toString().isEmpty() ||
			!QDir(settings.value("globalPluginPath").toString()).exists()) {
		settings.remove("globalPluginPath");
		force = true;
	}
	if(settings.contains(privPathTag) &&
			(settings.value(privPathTag).toString().isEmpty() ||
			!QDir(settings.value(privPathTag).toString()).exists())) {
		settings.remove(privPathTag);
		force = true;
	}

	if (force) {
		if (!settings.contains("globalPluginPath")) {
#ifdef UNIX
			// standard install path on linux systems
			QString globalPath = "/usr/lib/charon-plugins";
#else
			// Assume global plugin dir to be the same directory
			// where the tuchulcha.exe is located
			QString globalPath = QCoreApplication::applicationDirPath();
#endif
			settings.setValue("globalPluginPath", globalPath);
		}

		QString globalPath = settings.value("globalPluginPath").toString();
		if (globalPath.isEmpty()) {
			globalPath = tr("(none)");
		}
		QString privatPath = settings.value(privPathTag).toString();
		if (privatPath.isEmpty()) {
			privatPath = tr("(none)");
		}

		QMessageBox::information(
			0, tr("Welcome"), tr(
				"Welcome to Tuchulcha!<br><br>"
				"Tuchulcha loads its information about existing modules "
				"directly from the module files.<br>"
				"Please check, if the paths to the modules' directories "
				"are set correctly:<br><br>"
				"<b>global plugin path:</b><br><tt>%1</tt><br><br>"
				"<b>private plugin path:</b><br><tt>%2</tt><br><br>"
				"This is just a <em>guess</em> based on information "
				"about standard locations and the application directory.<br>"
				"These paths may be changed in the options dialog.<br>"
				"(Edit-&gt;Options)<br><br>"
				"Please press (<b>File-&gt;update plugins</b>) "
				"after leaving this dialogue and after changing "
				"the mentioned settings.")
				.arg(globalPath).arg(privatPath)
		);
	}
}

#include "FileManager.moc"
