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
/** \file CharonRun.cpp
 *  \brief Implementation of class CharonRun
 *
 *  \date 13.11.2011
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "CharonRun.h"
#include "CharonRun.moc"

#include "FileManager.h"
#include <charon-core/SplitStream.h>
#include <QSettings>
#include <fstream>
#include <charon-core/PluginManager.h>
#include <QMessageBox>
#include <QTextStream>
#include <QApplication>

CharonRun::CharonRun(QObject* pp) : QObject(pp), _lockCount(0)
{
}

void CharonRun::updatePlugins() {
	lock();
	const FileManager& fm = FileManager::instance();
	QDir configDir = fm.configDir();
	QString logFileName = configDir.absoluteFilePath("updateLog.txt");
	std::ofstream log(logFileName.toStdString().c_str(), std::ios::trunc);
	Q_ASSERT(log.good());
	sout.assign(std::cout, log);

	QDir metaPath(configDir.absoluteFilePath("metadata"));

	// delete old wrp files
	QStringList wrpFiles = metaPath.entryList(
			QStringList("*.wrp"), QDir::Files);
	for (int i=0; i < wrpFiles.size(); i++) {
		Q_ASSERT(QFileInfo(metaPath.absoluteFilePath(wrpFiles[i])).exists());
		Q_ASSERT(QFileInfo(metaPath.absoluteFilePath(wrpFiles[i])).isFile());
		Q_ASSERT(wrpFiles[i].indexOf(".wrp") > 0);
		metaPath.remove(wrpFiles[i]);
		Q_ASSERT(!QFileInfo(wrpFiles[i]).exists());
	}

	QSettings settings;
	QStringList paths;
	paths << settings.value(fm.privPathTag).toString();
	paths << settings.value("globalPluginPath").toString().split(";");
	paths.removeDuplicates();
	paths.removeAll("");
	std::vector<std::string> pathsS;
	QStringListIterator iter(paths);
	while (iter.hasNext()) {
		pathsS.push_back(iter.next().trimmed().toStdString());
	}
	PluginManager man(pathsS,
#if !defined(_MSC_VER) && defined(NDEBUG)
			// use selected option
			settings.value("suffixedPlugins", false).toBool());
#else
			// determined by compile type
			DEFAULT_DEBUG_SUFFIX);
#endif
	man.createMetadata(metaPath.absolutePath().toStdString());

	sout.assign();
	log.close();

	// update metadata file
	QFile cFile(fm.classesFile());
	if (!cFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox::warning(
			fm.dialogParent, tr("error updating meta data"),
			tr("could not open classes file for writing"));
		return;
	}
	QTextStream cStream(&cFile);
	Q_ASSERT(cStream.status() == QTextStream::Ok);
	cStream << "# Tuchulcha class information file\n";
	cStream << "# Content is copied from files in metadata directory\n";
	cStream << "# This is generated during update plugins,\n";
	cStream << "# do not edit by hand!" << endl;

	wrpFiles = metaPath.entryList(QStringList("*.wrp"), QDir::Files);
	for(int i=0; i<wrpFiles.size(); i++) {
		QFile cur(metaPath.absoluteFilePath(wrpFiles[i]));
		cur.open(QIODevice::ReadOnly | QIODevice::Text);
		QString content = cur.readAll();
		cur.close();
		cStream << tr("\n# from file \"%1\":\n").arg(wrpFiles[i]);
		cStream << content << endl;
	}
	cFile.close();
	unlock();
}

void CharonRun::runWorkflow(QString fName) {
	lock();
	QTextStream qout(stdout);
	qout << tr("running workflow \"%1\"").arg(fName) << endl;
	unlock();
}

void CharonRun::lock() {
	_lockCount++;
}

void CharonRun::unlock() {
	_lockCount--;
	if (_lockCount == 0) {
		QApplication::exit();
	}
}
