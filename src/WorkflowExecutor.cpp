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
/** @file   WorkflowExecutor.cpp
 *  @brief  Implementation of class WorkflowExecutor.
 *  @date   15.04.2010
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "WorkflowExecutor.h"
#include "FileManager.h"
#include "ObjectInspector.h"
#include "ParameterFileModel.h"
#include <QMessageBox>
#include <QDateTime>
#include <QSettings>
#include <exception>
#include <ui_LogDialog.h>
#include <charon-core/PluginManager.h>

#ifdef __GNUG__
#include <cxxabi.h>
#endif // __GNUG__

WorkflowExecutor::WorkflowExecutor(ObjectInspector* inspector, QObject* p) :
		QObject(p),
		_runAction(0),
		_manager(0),
		_log(0),
		_inspector(inspector)
{
	_logFileName = FileManager::instance().configDir().path().toAscii()
			.constData();
	_logFileName += "/executeLog.txt";
}

WorkflowExecutor::~WorkflowExecutor() {
	if (_manager)
		run();
}

QAction* WorkflowExecutor::runAction() {
	if (!_runAction) {
		_runAction = new QAction(this);
		_runAction->setShortcut(QKeySequence(tr("Ctrl+E")));
		_updateIcon();
		connect(_runAction, SIGNAL(triggered()), this, SLOT(run()));
	}
	return _runAction;
}

void WorkflowExecutor::_updateIcon() {
	if (_manager) {
		_runAction->setIcon(QIcon(":/icons/stop.png"));
		_runAction->setText(tr("Finish &workflow execution"));
		_runAction->setStatusTip(tr("Execution cleanup, unload plugins"));
	}
	else {
		_runAction->setIcon(QIcon(":/icons/execute.png"));
		_runAction->setText(tr("Execute &Workflow"));
		_runAction->setStatusTip(tr(
				"execute workflow that is shown in the current window"));
	}
}

void WorkflowExecutor::_run() {
	if (!_inspector->isEnabled())
		return;
	if(_manager)
		return;

	// setup and start workflow execution
	QSettings settings(
			"Heidelberg Collaboratory for Image Processing",
			"Tuchulcha");
	_manager = new PluginManager(
			settings.value("globalPluginPath").toString().toAscii().data(),
			settings.value("privatePluginPath").toString().toAscii().data());

	_log = new std::ofstream(_logFileName.c_str(), std::ios::trunc);
	Q_ASSERT(_log);
	Q_ASSERT(!_log->fail());
	Q_ASSERT(_log->good());
#ifdef WIN32
	sout.assign(*_log);
#else
	sout.assign(*_log, std::cout);
#endif
	sout << "Opened execution logfile." << std::endl;
	const QDateTime& startTime = QDateTime::currentDateTime();
	sout << "Time: " << startTime.toString(Qt::ISODate)
			.toAscii().constData();
	sout << std::endl;
	_pathBak = QDir::currentPath();
	const ParameterFileModel& model = *(_inspector->model());
	QString fileName = model.fileName();
	const ParameterFile& parameterFile = model.parameterFile();
	QDir::setCurrent(QFileInfo(fileName).path());
	try {
		sout << "loading Parameter file" << std::endl;
		_manager->loadParameterFile(parameterFile);
		sout << "starting execution" << std::endl;
		_manager->executeWorkflow();
	}
	catch (const std::string& msg) {
		QMessageBox::warning(0, tr("error during execution"),
			tr("Caught exception of type <b>std::string</b>."
				"<br><br>Message:<br>%1")
				.arg(msg.c_str()).replace("\n", "<br>"));
	}
	catch (const std::exception& excpt) {
		const char* name = typeid(excpt).name();
#ifdef __GNUG__
		name = abi::__cxa_demangle(name, 0, 0, 0);
#endif // __GNUG__
		qWarning("%s",
				 tr("Caught exception of type \"%1\"\n\nMessage:\n%2")
				.arg(name).arg(excpt.what()).toAscii().constData());
	}
	catch (const char* &msg) {
		QMessageBox::warning(0, tr("error during execution"),
			tr("Caught exception of type <b>char*</b>."
				"<br><br>Message:<br>%1")
				.arg(msg).replace("\n", "<br>"));
	}
	catch (...) {
		qWarning("%s", tr("Caught exception of unknown type")
				.toAscii().constData());
	}
	sout << "Execution finished.\n";
	const QDateTime& endTime = QDateTime::currentDateTime();
	sout << "Time   : " << endTime.toString(Qt::ISODate).toAscii().constData();
	sout << "\n";
	QTime runTime = QTime().addSecs(startTime.secsTo(endTime));
	sout << "Runtime: " << runTime.toString("hh:mm:ss.zzz")
			.toAscii().constData() << std::endl;
}

void WorkflowExecutor::_cleanup() {
	if(!_manager)
		return;

	_manager->reset();
	sout.assign();
	_log->close();
	QDir::setCurrent(_pathBak);

	Ui::LogDialog logDialog;
	QDialog* dialog = new QDialog(FileManager::dialogParent);
	logDialog.setupUi(dialog);
	logDialog.infoLabel->setText(tr("Workflow execution finished."));
	logDialog.logLabel->setText(
			tr("Content of logfile <tt>%1</tt>:").arg(_logFileName.c_str()));
	QFile logFile(_logFileName.c_str());
	logFile.open(QIODevice::ReadOnly | QIODevice::Text);
	logDialog.logText->insertPlainText(logFile.readAll());
	logFile.close();
	dialog->exec();
	delete _log;
	_log = 0;
	delete _manager;
	_manager = 0;
}

void WorkflowExecutor::run() {
	QSettings settings(
			"Heidelberg Collaboratory for Image Processing",
			"Tuchulcha");
	if (settings.value("waitAfterExecute", false).toBool()) {
		if(!_manager) {
			_run();
			QMessageBox::information(0, tr("execution finished"),
					tr("Workflow execution finished.\n"
						"Plugins stay loaded until "
						"the stop button is pressed."));
		}
		else
			_cleanup();
	}
	else {
		// don't wait after execution
		_run();
		_cleanup();
	}

	_updateIcon();
}

#include "WorkflowExecutor.moc"
