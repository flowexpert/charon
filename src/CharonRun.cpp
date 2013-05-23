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
	QObject(pp), _man(0), _log(0)
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
	qout << "(II) "
		<< tr("%1 plugins with debug suffix")
			.arg((debug?tr("prefering"):tr("ignoring"))) << endl;

	QStringList paths;
	QStringList libFilter;
	libFilter << "*.dll" << "*.so" << "*.dylib";
	QStringList excludeList = settings.value("excludeList").toStringList();
	QStringList exclExp;
	paths << settings.value(privPathTag).toStringList();
	paths << settings.value("globalPluginPath").toStringList();
	paths.removeDuplicates();
	paths.removeAll("");

	qout << "(II) " << tr("Paths:") << endl;
	std::vector<std::string> pathsS, exclS;
	foreach (QString cur, paths) {
		QDir curD(cur);
		cur = curD.canonicalPath();

		if (curD.exists()) {
			qout << "(II) \t" << cur << endl;
			pathsS.push_back(cur.toStdString());
			// globbing for excludes
			QStringList libs = curD.entryList(libFilter);
			foreach (QString lib, libs) {
				QFileInfo lin(lib);
				lib = lin.completeBaseName();
				lib.remove(QRegExp("_d$",Qt::CaseInsensitive));
#ifndef _MSC_VER
				lib.remove(QRegExp("^lib",Qt::CaseInsensitive));
#endif
				foreach (QString ex, excludeList) {
					QRegExp rex(ex,Qt::CaseInsensitive,QRegExp::WildcardUnix);
					if (rex.exactMatch(lib)) {
						exclExp << lib;
					}
				}
			}
		}
		else {
			qout << "(WW) \t"
				 << tr("skipping non-existing path: %1").arg(cur) << endl;
		}
	}
	qout << endl;
	exclExp.removeDuplicates();
	qout << "(DD) " << tr("found excludes:") << endl;
	foreach (QString exc, exclExp) {
		qout << "(DD) \t" << exc << endl;
		exclS.push_back(exc.toStdString());
	}
	qout << "(DD) " << endl;

	PluginManagerInterface::PluginVersionCheckLevel versionCheck =
		static_cast<PluginManagerInterface::PluginVersionCheckLevel>(
			settings.value("versionCheckLevel",2).toInt());

	// initialize plugin manager with determined settings
	_man = new PluginManager(pathsS, debug, false, versionCheck);
	_man->setExcludeList(exclS);
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
	_taskStart();

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
	_taskFinished();
}

void CharonRun::runWorkflow(QString fName) {
	_taskStart();
	QTextStream qout(stdout);
	_setupMan("executeLog.txt");

	QString pathBak = QDir::currentPath();
	fName = QFileInfo(fName).absoluteFilePath();
	QDir::setCurrent(QFileInfo(fName).path());
	QString errorMsg;
	try {
		qout << tr("Loading parameter file \"%1\"").arg(fName) << endl;
		_man->loadParameterFile(fName.toStdString());
		qout << endl;
		qout << tr("== Executing Workflow ==") << endl;
		const QDateTime& startTime = QDateTime::currentDateTime();
		qout << tr("start time: %1").arg(startTime.toString(Qt::ISODate))
			 << endl;
		_man->runWorkflow();
		const QDateTime& endTime = QDateTime::currentDateTime();
		QTime runTime = QTime().addSecs(startTime.secsTo(endTime));

		qout << tr("Execution finished.") << "\n";
		qout << tr("end time : %1").arg(endTime.toString(Qt::ISODate)) << endl;
		qout << tr("runtime  : %1").arg(
					runTime.toString("hh:mm:ss.zzz")) << endl;
	}
	catch (const std::string& msg) {
		errorMsg =
			tr("Caught exception of type \"std::string\".")
				+"\n\n"+tr("Message:")+"\n"+msg.c_str();
	}
	catch (const std::exception& excpt) {
		const char* name = typeid(excpt).name();
#ifdef __GNUG__
		name = abi::__cxa_demangle(name, 0, 0, 0);
#endif // __GNUG__
		errorMsg =
			tr("Caught exception of type \"%1\".").arg(name)
			+"\n\n"+tr("Message:")+"\n"+excpt.what();
	}
	catch (const char* &msg) {
		errorMsg =
			tr("Caught exception of type \"char*\".")
			+ "\n\n"+tr("Message:")+"\n" + msg;
	}
	catch (...) {
		errorMsg = tr("Caught exception of unknown type.");
	}
	if (!errorMsg.isEmpty()) {
		QTextStream qerr(stderr);
		qerr << "\n****************************************************\n\n"
				<< tr("Error during execution:") << "\n\n"
				<< errorMsg << "\n\n"
				<< "****************************************************"
				<< endl;
	}
	//don't reset the working Dir, some plugins may still be running
	//(e.g. FileReaderWatcher or ArgosDisplayReloader)
	//QDir::setCurrent(pathBak);

	_taskFinished();
}

void CharonRun::_taskStart() {
	emit busy();
}

void CharonRun::_taskFinished() {
	emit ready();
}

void CharonRun::exitWhenFinished() {
	QApplication::exit();
}

void CharonRun::updateDynamics(QString fName) {
	_taskStart();

	QString baseN = QFileInfo(fName).baseName();

	// delete old wrp files
	const FileManager& fm = FileManager::instance();
	QDir dynamPath = fm.configDir().absoluteFilePath("dynamics");
	if (dynamPath.exists()) {
		QStringList wrpFiles = dynamPath.entryList(
			QStringList("*.wrp"), QDir::Files);
		wrpFiles = wrpFiles.filter(QRegExp("^" + baseN + "_"));
		for (int i=0; i < wrpFiles.size(); i++) {
			Q_ASSERT(QFileInfo(dynamPath.absoluteFilePath(wrpFiles[i])).exists());
			Q_ASSERT(QFileInfo(dynamPath.absoluteFilePath(wrpFiles[i])).isFile());
			Q_ASSERT(wrpFiles[i].indexOf(".wrp") > 0);
			dynamPath.remove(wrpFiles[i]);
			Q_ASSERT(!QFileInfo(wrpFiles[i]).exists());
		}
	} else {
		Q_ASSERT(fm.configDir().mkdir("dynamics"));
	}

	_setupMan("updateLog.txt");
	_man->createDynamicMetadata(fName.toStdString(),
		dynamPath.absoluteFilePath(baseN).toStdString());
	_freeMan();

	_taskFinished();
}
