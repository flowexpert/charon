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
#include <QDateTime>

#ifdef __GNUG__
#include <cxxabi.h>
#endif // __GNUG__

CharonRun::CharonRun(QObject* pp) :
	QObject(pp), _lockCount(0), _man(0), _log(0)
{
}

CharonRun::~CharonRun() {
	if (_man || _log) {
		_freeMan();
	}
}

void CharonRun::_setupMan(QString logFileName) {
	if (_man || _log) {
		_freeMan();
	}

	const FileManager& fm = FileManager::instance();
	if (!logFileName.isEmpty()) {
		QDir configDir = fm.configDir();
		logFileName = configDir.absoluteFilePath(logFileName);
		_log = new std::ofstream(
					logFileName.toStdString().c_str(), std::ios::trunc);
		Q_ASSERT(_log);
		Q_ASSERT(!_log->fail());
		Q_ASSERT(_log->good());
		sout.assign(std::cout, *_log);
	}

	QSettings settings;
#ifdef _MSC_VER
	// use compile type in MSVC builds
#ifdef QT_DEBUG
	const bool debug = true;
	const QString privPathTag = "privatePluginPathD";
#else // QT_DEBUG
	const bool debug = false;
	const QString privPathTag = "privatePluginPath";
#endif // QT_DEBUG
#else // MSVC
	// use settings option in unix builds
	const bool debug = settings.value("suffixedPlugins", false).toBool();
	const QString privPathTag = debug?"privatePluginPathD":"privatePluginPath";
#endif // MSVC

	QTextStream qout(stdout,QIODevice::WriteOnly);
	qout << "(II) " << (debug?tr("prefering"):tr("ignoring"))
	<< " " << tr("plugins with debug suffix") << endl;

	QStringList paths;
	paths << settings.value(privPathTag).toStringList();
	paths << settings.value("globalPluginPath").toStringList();
	paths.removeDuplicates();
	paths.removeAll("");

	qout << "(II) Paths: " << endl;

	std::vector<std::string> pathsS;
	QStringListIterator iter(paths);
	while (iter.hasNext()) {
		QString cur = iter.next().trimmed();
		qout << "(II)     " << cur << endl;
		pathsS.push_back(cur.toStdString());
	}
	qout << endl;

	// initialize plugin manager with determined settings
	_man = new PluginManager(pathsS, debug);
}

void CharonRun::_freeMan() {
	if (_man) {
		delete _man;
		_man = 0;
	}
	sout.assign(std::cout);
	if (_log) {
		_log->close();
		delete _log;
		_log = 0;
	}
}

void CharonRun::updatePlugins() {
	lock();

	// delete old wrp files
	const FileManager& fm = FileManager::instance();
	QDir metaPath = fm.configDir().absoluteFilePath("metadata");
	QStringList wrpFiles = metaPath.entryList(
			QStringList("*.wrp"), QDir::Files);
	for (int i=0; i < wrpFiles.size(); i++) {
		Q_ASSERT(QFileInfo(metaPath.absoluteFilePath(wrpFiles[i])).exists());
		Q_ASSERT(QFileInfo(metaPath.absoluteFilePath(wrpFiles[i])).isFile());
		Q_ASSERT(wrpFiles[i].indexOf(".wrp") > 0);
		metaPath.remove(wrpFiles[i]);
		Q_ASSERT(!QFileInfo(wrpFiles[i]).exists());
	}

	_setupMan("updateLog.txt");
	_man->createMetadata(metaPath.absolutePath().toStdString());
	_freeMan();

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
	_setupMan("executeLog.txt");

	QString pathBak = QDir::currentPath();
	fName = QFileInfo(fName).absoluteFilePath();
	QDir::setCurrent(QFileInfo(fName).path());
	// error occurs in _manager->executeWorkflow.
	// perhaps try/catch-block unneccessary.
	QString errorMsg;
	try {
		qout << tr("Loading parameter file \"%1\"").arg(fName) << endl;
		_man->loadParameterFile(fName.toStdString());
		qout << endl;
		qout << tr("== Executing Workflow ==") << endl;
		const QDateTime& startTime = QDateTime::currentDateTime();
		qout << tr("start time: %1").arg(startTime.toString(Qt::ISODate))
			 << endl;
		_man->executeWorkflow();
		const QDateTime& endTime = QDateTime::currentDateTime();
		QTime runTime = QTime().addSecs(startTime.secsTo(endTime));

		qout << tr("Execution finished.") << "\n";
		qout << tr("end time : %1").arg(endTime.toString(Qt::ISODate)) << endl;
		qout << tr("runtime  : %1").arg(
					runTime.toString("hh:mm:ss.zzz")) << endl;
	}
	catch (const std::string& msg) {
		errorMsg =
				tr("Caught exception of type \"std::string\".\n\nMessage:\n%1")
				.arg(msg.c_str());
	}
	catch (const std::exception& excpt) {
		const char* name = typeid(excpt).name();
#ifdef __GNUG__
		name = abi::__cxa_demangle(name, 0, 0, 0);
#endif // __GNUG__
		errorMsg =
			tr("Caught exception of type \"%1\".\n\nMessage:\n%2")
			.arg(name).arg(excpt.what());
	}
	catch (const char* &msg) {
		errorMsg =
			tr("Caught exception of type \"char*\".\n\nMessage:\n%1").arg(msg);
	}
	catch (...) {
		errorMsg = tr("Caught exception of unknown type");
	}
	if (!errorMsg.isEmpty()) {
		qout << "\n****************************************************\n\n"
				<< tr("Error during execution:") << "\n\n"
				<< errorMsg << "\n\n"
				<< "****************************************************"
				<< endl;
	}
	QDir::setCurrent(pathBak);

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
