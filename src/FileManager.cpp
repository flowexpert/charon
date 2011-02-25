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
#include <QSettings>
#include <QTextStream>
#include "PluginManager.h"
#include "ui_LogDialog.h"

#ifndef TUCHULCHA_DIR
/// Tuchulcha config path
/** Name of the directory where tuchulcha stores
 *  e.g. metadata information.
 *  (QSettings stuff goes somewhere else!)
 */
#define TUCHULCHA_DIR ".tuchulcha"
#endif
#ifndef CHARON_INSTALL
/// Charon core installation directory
#define CHARON_INSTALL
#error "CHARON_INSTALL not defined!"
#endif

FileManager* FileManager::_inst = 0;

QWidget* FileManager::dialogParent = 0;

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

		// initialize random seed for tempfile name generation
		srand(time(NULL));
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
	if (!QFile(path).exists()) {
		// write empty classes file
		std::ofstream newFile(path.toAscii().constData(), std::ios::trunc);
		newFile << "# empty classes file" << std::endl;
		newFile.close();
	}
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

void FileManager::loadPluginInformation() const {
	QString logFileName = configDir().absoluteFilePath("updateLog.txt");
	std::ofstream log(logFileName.toAscii().constData(), std::ios::trunc);
	Q_ASSERT(log.good());
#ifdef WIN32
	sout.assign(log);
#else
	sout.assign(log, std::cout);
#endif

	QDir metaPath(configDir().absoluteFilePath("metadata"));

	// delete old wrp files
	QStringList wrpFiles = metaPath.entryList(
			QStringList("*.wrp"), QDir::Files);
	for (int i=0; i < wrpFiles.size(); i++) {
		Q_ASSERT(QFileInfo(metaPath.absoluteFilePath(wrpFiles[i])).exists());
		Q_ASSERT(QFileInfo(metaPath.absoluteFilePath(wrpFiles[i])).isFile());
		Q_ASSERT(wrpFiles[i].indexOf("wrp") > 0);
		metaPath.remove(wrpFiles[i]);
		Q_ASSERT(!QFileInfo(wrpFiles[i]).exists());
	}

	QSettings settings(
			"Heidelberg Collaboratory for Image Processing",
			"Tuchulcha");
	PluginManager man(
			settings.value("globalPluginPath").toString().toAscii().data(),
			settings.value("privatePluginPath").toString().toAscii().data());
	man.createMetadata(metaPath.absolutePath().toAscii().constData());

	sout.assign();
	log.close();

	Ui::LogDialog logDialog;
	QDialog* dialog = new QDialog(dialogParent);
	logDialog.setupUi(dialog);
	logDialog.infoLabel->setText(tr("Plugin information updated."));
	logDialog.logLabel->setText(
			tr("Content of logfile <tt>%1</tt>:").arg(logFileName));
	QFile logFile(logFileName);
	logFile.open(QIODevice::ReadOnly | QIODevice::Text);
	
	logDialog.logText->insertPlainText(logFile.readAll());
	logDialog.progressBar->hide() ;
	logFile.close();
	dialog->exec();
	QApplication::processEvents() ;
}

void FileManager::updateMetadata() const {
	QFile cFile(classesFile());
	if (!cFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qWarning("%s", tr("could not open classes file for writing")
				 .toAscii().constData());
		return;
	}
	QTextStream cStream(&cFile);
	Q_ASSERT(cStream.status() == QTextStream::Ok);
	cStream << "# Tuchulcha class information file" << endl;
	cStream << "# Content is copied from files in metadata directory" << endl;
	cStream << "# This is generated during update plugins," << endl;
	cStream << "# do not edit by hand!" << endl;

	QDir metaPath(configDir().absoluteFilePath("metadata"));
	QStringList wrpFiles = metaPath.entryList(
			QStringList("*.wrp"), QDir::Files);

	for(int i=0; i<wrpFiles.size(); i++) {
		QFile cur(metaPath.absoluteFilePath(wrpFiles[i]));
		cur.open(QIODevice::ReadOnly | QIODevice::Text);
		QString content = cur.readAll();
		cur.close();
		cStream << "# from file \"" << wrpFiles[i] << "\":" << endl;
		cStream << content << "\n" << endl;
	}
	cFile.close();
}

void FileManager::configure(QWidget* parentWidget, bool force) const {
	QSettings settings(
			"Heidelberg Collaboratory for Image Processing",
			"Tuchulcha");

	if (force || !settings.contains("globalPluginPath")) {
		QString charonInstall = CHARON_INSTALL;
		settings.setValue("charonInstallPath", charonInstall);
#ifdef UNIX
		charonInstall.append("/lib/charon-plugins");
#else
		charonInstall.append("/bin");
#endif
		settings.setValue("globalPluginPath", charonInstall);
		settings.setValue("privatePluginPath", QString());

		QMessageBox::information(
				parentWidget, tr("path information"),
				tr("Your paths are set to the following values:<br>"
				   "charon-install dir: <tt>%1</tt><br>"
				   "global plugin path: <tt>%2</tt><br>"
				   "private plugin path: <tt>%3</tt><br>"
				   "These paths may be changed in the options dialog.<br>"
				   "(Edit-&gt;Options)")
					.arg(settings.value("charonInstallPath").toString())
					.arg(settings.value("globalPluginPath").toString())
					.arg(settings.value("privatePluginPath").toString()));
	}
}

bool FileManager::compileAndLoad(QWidget* parentWidget) const
		throw (AbstractPluginLoader::PluginException) {
	QSettings settings(
			"Heidelberg Collaboratory for Image Processing",
			"Tuchulcha");
	if (!settings.value("privatePluginPath").toString().isEmpty()) {
		QMessageBox::warning(
				parentWidget, tr("no private plugin path"),
				tr("You do not have any privat plugin path set!<br>"
				   "Please set it in the options dialog (Edit-&gt;Options)!"));
		return false;
	}

	QString fileName = QFileDialog::getOpenFileName(
			parentWidget, tr("Select source file"),
			QDir::homePath(), tr("Source files (*.cpp)"));

	if (fileName.size()) {
		try {
			bool ok;
			QString text = QInputDialog::getText(
					parentWidget, tr("Library dependencies"),
					tr("Specify referenced Libraries.<br>"
					   "Separate multiple libraries with ';'.<br>"
					   "Leave empty if no dependencies exist."),
					QLineEdit::Normal, "", &ok);
			if (!ok)
				return false;

			std::vector<std::string> libsVector;
			QStringList list = text.split(";", QString::SkipEmptyParts);
			for (int i = 0; i < list.size(); i++)
				libsVector.push_back(list[i].trimmed().toAscii().data());

			PluginManager man(
					settings.value("globalPluginPath")
							.toString().toAscii().data(),
					settings.value("privatePluginPath")
							.toString().toAscii().data());

			QString oldDir = QDir::currentPath();
			ok = QDir::setCurrent(
					settings.value("charonInstallPath").toString());
			if(!ok) {
				QMessageBox::warning(
						parentWidget, tr("failed to change directory"),
						tr("could not change into charon install directory "
						   "(%1)").arg(
							settings.value("charonInstallPath").toString()));
				return false;
			}
			man.compileAndLoadPlugin(
					fileName.toAscii().data(), libsVector,
					configDir().absoluteFilePath("metadata")
							.toAscii().constData());
			QDir::setCurrent(oldDir);
			updateMetadata();
		}
		catch (std::string s) {
			throw AbstractPluginLoader::PluginException(s);
		}
	}
	return false;
}

#include "FileManager.moc"
