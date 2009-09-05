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
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QStringList>
#include <iostream>
#include "PluginManager.h"

FileManager* FileManager::_inst = 0;

FileManager::FileManager() {
	if (!QDir::home().exists(".paramedit"))
		QDir::home().mkdir(".paramedit");
	Q_ASSERT(configDir().exists());
}

const FileManager& FileManager::instance() {
	if (!_inst) {
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
	if (!QFile(QDir::homePath() + "/.paramedit/metadata").exists()) {
		QDir::home().mkpath(".paramedit/metadata");
	}
	if (!QFile(path).exists()) {
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

void FileManager::updatePlugins() const {
	ParameterFile pf(std::string(
			FileManager::instance().configDir().path().toAscii().data())
			+ "/Paths.config");

	std::string charon_utils_install = pf.get<std::string> (
			"charon-utils-install");
	std::string additionalPluginPath = pf.get<std::string> (
			"additional-plugin-path");

	try {
		PluginManager man(charon_utils_install + "/lib/charon-plugins",
				additionalPluginPath);
		man.createMetadata(std::string(
				FileManager::instance().configDir().path().toAscii().data())
				+ "/metadata");
	} catch (AbstractPluginLoader::PluginException e) {
		std::cerr << e.what() << std::endl;
	}
}

void FileManager::generateMetaData() const {
	std::string metaPath = std::string(configDir().path().toAscii().data())
			+ "/metadata";
	std::string oldPath = FileTool::getCurrentDir();
	FileTool::changeDir(metaPath);
	std::vector<std::string> wrp_files = FileTool::getFilesWithSuffix(".wrp");
	std::ifstream inStream;
	std::ofstream outStream;
	const char* fName = classesFile().toAscii().data();
	outStream.open(fName, std::ios::trunc);

	for (unsigned int i = 0; i < wrp_files.size(); i++) {
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

void FileManager::configure(QWidget * parent, bool force) const {
	if (force
			|| !QFile(QString(QDir::homePath() + "/.paramedit/Paths.config")).exists()) {
		QString path = QFileDialog::getExistingDirectory(parent, QString(
				"Specify your personal plugin path"), QDir::homePath(),
				QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

		QFile pathsConfig;
		if (FileTool::exists("../share/tuchulcha/Paths.config")) {
			pathsConfig.setFileName("../share/tuchulcha/Paths.config");
		} else if (FileTool::exists("/usr/local/share/tuchulcha/Paths.config")) {
			pathsConfig.setFileName("/usr/local/share/tuchulcha/Paths.config");
		} else if (FileTool::exists("/usr/share/tuchulcha/Paths.config")) {
			pathsConfig.setFileName("/usr/share/tuchulcha/Paths.config");
		}
		pathsConfig.copy(QDir::homePath() + "/.paramedit/Paths.config");
		ParameterFile
				pf(
						(QDir::homePath() + "/.paramedit/Paths.config").toAscii().data());
		pf.set<std::string> ("additional-plugin-path", path.toAscii().data());
		pf.save(
				(QDir::homePath() + "/.paramedit/Paths.config").toAscii().data());
	}
}

bool FileManager::compileAndLoad(QWidget * parent) const
		throw (AbstractPluginLoader::PluginException) {
	if (!(ParameterFile(
			(QDir::homePath() + "/.paramedit/Paths.config").toAscii().data()).get<
			std::string> ("additional-plugin-path").size())) {
		QMessageBox msgBox;
		msgBox.setText("You need a private plugin path.");
		msgBox.setInformativeText(
				"Do you want to specify a private plugin path?");
		msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
		msgBox.setDefaultButton(QMessageBox::No);
		int ret = msgBox.exec();

		switch (ret) {
		case QMessageBox::No:
			return false;
		case QMessageBox::Yes:
			configure(parent, true);
			if (!(ParameterFile(
					(QDir::homePath() + "/.paramedit/Paths.config").toAscii().data()).get<
					std::string> ("additional-plugin-path").size())) {
				return false;
			}
			break;
		}
	}

	QString fileName = QFileDialog::getOpenFileName(parent, QString(
			"Select source file"), "/home", QString("Source files (*.cpp)"));

	if (fileName.size()) {
		try {
			std::string charon_utils_install = ParameterFile((QDir::homePath()
					+ "/.paramedit/Paths.config").toAscii().data()).get<
					std::string> ("charon-utils-install");

			bool ok;
			QString text = QInputDialog::getText(parent, QString(
					"Library dependencies"), QString(
					"Specify referenced Libraries.\n"
						"Separate multiple libraries with ';'.\n"
						"Leave empty if no dependencies exist."),
					QLineEdit::Normal, "", &ok);

			if (!ok) {
				return false;
			}

			std::vector<std::string> libsVector;

			QStringList list = text.split(";", QString::SkipEmptyParts);

			for (int i = 0; i < list.size(); i++) {
				libsVector.push_back(list[i].trimmed().toAscii().data());
				std::cout << libsVector[i] << std::endl;
			}

			PluginManager
					man(
							charon_utils_install + "/lib/charon-plugins",
							ParameterFile(
									(QDir::homePath()
											+ "/.paramedit/Paths.config").toAscii().data()).get<
									std::string> ("additional-plugin-path"));
			std::string oldDir = FileTool::getCurrentDir();
			FileTool::changeDir(charon_utils_install);
			man.compileAndLoadPlugin(fileName.toAscii().data(), libsVector,
					std::string(configDir().path().toAscii().data())
							+ "/metadata");
			FileTool::changeDir(oldDir);

			generateMetaData();
		} catch (AbstractPluginLoader::PluginException e) {
			throw e;
		} catch (std::string s) {
			throw AbstractPluginLoader::PluginException(s);
		}
	}
	return false;
}
