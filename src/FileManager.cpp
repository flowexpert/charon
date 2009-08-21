/*	Copyright (C) 2009 Jens-Malte Gottfried

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
/**	@file	FileManager.cpp
 *	@brief	Implementation of class FileManager.
 *	@date	02.12.2008
 *	@author	<a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
 
#include <QFile>
#include <cstdlib>
#include <ctime>
#include "FileManager.h"

FileManager* FileManager::_inst = 0;

FileManager::FileManager() {
	if(!QDir::home().exists(".paramedit"))
		QDir::home().mkdir(".paramedit");
	Q_ASSERT(configDir().exists());
}

const FileManager& FileManager::instance() {
	if(!_inst) {
		_inst = new FileManager();

		// initialize random seed for tempfile name generation
		srand(time(NULL));
	}
	return *_inst;
}

QString FileManager::configFile() const {
	// toNativeSeparators here causes the application to crash
	// on std::string destructors (something strange happens here)
	return configDir().absoluteFilePath("config");
}

QDir FileManager::configDir() const {
	QDir ret = QDir::home();
	bool ok;
	ok = ret.cd(".paramedit");
	Q_ASSERT(ok);
	return ret;
}

QString FileManager::classesFile() const {
	QString path = QDir::homePath() + "/.paramedit/classes.wrp";
	if(!QFile(path).exists()) {
		// copy sampleclasses from resource to the path
		QFile sclasses(":/class/sampleclasses.wrp");
		sclasses.copy(path);
		
		// add sample doc file
		QFile dummydoc(":/class/extras/flowestimator.txt");
		if (!configDir().exists("extras"))
			configDir().mkdir("extras");
		dummydoc.copy(QDir::homePath()
			+ "/.paramedit/extras/flowestimator.txt");
	}

	// toNativeSeparators here causes the application to crash
	// on std::string destructors (something strange happens here)
	return path;
}

QString FileManager::tempFileName() const {
	QDir path = QDir::temp();
	QString filename;
	do {
		filename = QString("paramedit-%1.tmp").arg(rand());
	} while (path.exists(filename));
	return path.filePath(filename);
}
