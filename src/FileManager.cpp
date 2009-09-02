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
#include <fstream>
#include "FileManager.h"
#include <FileTool.h>
#include <ParameterFile.h>
#include <fstream>

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
	if(!QFile(QDir::homePath() + "/.paramedit/metadata").exists()) {
		QDir::home().mkpath(".paramedit/metadata");
	}
	if(!QFile(path).exists()) {
		// write empty classes file
		std::ofstream newFile(path.toAscii().constData(), std::ios::trunc);
		newFile << "# empty classes file" << std::endl;
		newFile.close();
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

void FileManager::generateMetaData() const {
	std::string metaPath = std::string(configDir().path().toAscii().data()) + "/metadata";
	std::string oldPath = FileTool::getCurrentDir();
	FileTool::changeDir(metaPath);
	std::vector<std::string> wrp_files = FileTool::getFilesWithSuffix(".wrp");
	std::ifstream inStream;
	std::ofstream outStream;
	const char* fName = classesFile().toAscii().data();
	outStream.open(fName, std::ios::trunc);

	for(unsigned int i = 0; i < wrp_files.size(); i++) {
		//&TODO Dateinamen angeben
		std::string buffer;
		inStream.open((metaPath + "/" + wrp_files[i]).c_str());
		while (!inStream.eof()) {
			std::getline(inStream, buffer);
			outStream << buffer << std::endl;
		}
		outStream << std::endl;
		inStream.close();
	}
	outStream.close();
	FileTool::changeDir(oldPath);
}
