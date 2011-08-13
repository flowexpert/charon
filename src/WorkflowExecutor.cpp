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
#include <QMessageBox>
#include <QDateTime>
#include <QSettings>
#include <QTimer>
#include <exception>
#include <QMutex>
#include <ui_LogDialog.h>
#include <charon-core/PluginManager.h>
#include "FileManager.h"
#include "ObjectInspector.h"
#include "ParameterFileModel.h"
#include "QParameterFile.h"

#ifdef __GNUG__
#include <cxxabi.h>
#endif // __GNUG__

WorkflowExecutor::WorkflowExecutor(ObjectInspector* inspector, QObject* p) :
		QThread(p),
		_runAction(0),
		_manager(0),
		_log(0),
		_inspector(inspector),
		_logFile(0),
		_logDialog(0),
		_logTimer(0)
{
	_logFileName = FileManager::instance().configDir().path().toStdString();
	_logFileName += "/executeLog.txt";
	connect(this, SIGNAL(finished()), this, SLOT(_executionFinished())) ;
	connect(this, SIGNAL(terminated()), this, SLOT(_executionTerminated())) ;
	setTerminationEnabled(true) ;
	_logTimer = new QTimer(this) ;
	connect(_logTimer, SIGNAL(timeout()), this, SLOT(_updateLogDialog())) ;
}

WorkflowExecutor::~WorkflowExecutor() {
	_cleanup() ;
	delete _logFile ;
}

QAction* WorkflowExecutor::runAction() {
	if (!_runAction) {
		_runAction = new QAction(this);
		_runAction->setShortcut(QKeySequence(tr("Ctrl+E")));
		_updateIcon();
		connect(_runAction, SIGNAL(triggered()), this, SLOT(execute()));
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

void WorkflowExecutor::run() {
	const QDateTime& startTime = QDateTime::currentDateTime();
	sout << "Time: " << startTime.toString(Qt::ISODate).toStdString()
			<< std::endl;

	try {
		_manager->executeWorkflow();
	}
	catch (const std::string& msg) {
		QMutexLocker locker(&_mutex) ;
		
		_executionMessage = tr("Caught exception of type <b>std::string</b>."
				"<br><br>Message:<br>%1")
				.arg(msg.c_str()).replace("\n", "<br>");
	}
	catch (const std::exception& excpt) {
		QMutexLocker locker(&_mutex) ;
		const char* name = typeid(excpt).name();
#ifdef __GNUG__
		name = abi::__cxa_demangle(name, 0, 0, 0);
#endif // __GNUG__
		_executionMessage = 
				 tr("Caught exception of type \"%1\"\n\nMessage:\n%2")
				.arg(name).arg(excpt.what()).toAscii().constData();
	}
	catch (const char* &msg) {
		QMutexLocker locker(&_mutex) ;
		_executionMessage =  tr("Caught exception of type <b>char*</b>."
				"<br><br>Message:<br>%1")
				.arg(msg).replace("\n", "<br>");
	}
	catch (...) {
		QMutexLocker locker(&_mutex) ;
		_executionMessage = tr("Caught exception of unknown type") ;
	}

	sout << "Execution finished.\n";
	const QDateTime& endTime = QDateTime::currentDateTime();
	sout << "Time   : " << endTime.toString(Qt::ISODate).toStdString();
	sout << std::endl;

	QTime runTime = QTime().addSecs(startTime.secsTo(endTime));
	sout << "Runtime: " << runTime.toString("hh:mm:ss.zzz").toStdString()
			<< std::endl;
}

void WorkflowExecutor::_execute() {
	if (!_inspector->isEnabled())
		return;
	if (_manager)
		return;
	_executionMessage = QString("");

	// setup and start workflow execution
	QSettings settings(
			"Heidelberg Collaboratory for Image Processing",
			"Tuchulcha");
	QStringList paths;
	paths << settings.value(FileManager::privPathTag).toString();
	paths << settings.value("globalPluginPath").toString().split(";");
	paths.removeDuplicates();
	paths.removeAll("");
	std::vector<std::string> pathsS;
	QStringListIterator iter(paths);
	while (iter.hasNext()) {
		pathsS.push_back(iter.next().trimmed().toStdString());
	}
	_manager = new PluginManager(pathsS,
#if not defined(_MSC_VER) and defined(NDEBUG)
			// use selected option
			settings.value("suffixedPlugins", false).toBool());
#else
			// determined by compile type
			DEFAULT_DEBUG_SUFFIX);
#endif
	_updateIcon() ;
	_log = new std::ofstream(_logFileName.c_str(), std::ios::trunc);
	Q_ASSERT(_log);
	Q_ASSERT(!_log->fail());
	Q_ASSERT(_log->good());

	// only log to file wich is shown in progress dialog
	sout.assign(*_log);

	_logDialog = new Ui::LogDialog;
	QDialog* dialog = new QDialog(FileManager::dialogParent);
	_logDialog->setupUi(dialog);
	_logDialog->progressBar->show() ;
	_logDialog->logLabel->setText(
			tr("Content of logfile <tt>%1</tt>:").arg(_logFileName.c_str()));
	_logFile = new QFile(_logFileName.c_str());
	_logFile->open(QIODevice::ReadOnly | QIODevice::Text);
	dialog->show();
	dialog->raise() ;
	dialog->activateWindow() ;
	_updateLogDialog() ;
	_logTimer->setInterval(settings.value("logTimerInveral", 500).toInt()) ;
	QApplication::processEvents() ;
	
	_pathBak = QDir::currentPath();
	const ParameterFileModel& model = *(_inspector->model());
	QString fileName = model.fileName();
	const QParameterFile& parameterFile = model.parameterFile();
	QDir::setCurrent(QFileInfo(fileName).path());
	// error occurs in _manager->executeWorkflow.
	// perhaps try/catch-block unneccessary.
	QString unexpectedErrorMsg;
	try {
		sout << "loading Parameter file" << std::endl;
		_manager->loadParameterFile(parameterFile.toParameterFile());
		sout << "executing Parameter file" << std::endl;
		_updateLogDialog() ;
		QApplication::processEvents() ;
		if (settings.value("executeThreaded",false).toBool()) {
			start();
			_logTimer->start();
		}
		else {
			run();
			_executionFinished();
		}
	}
	catch (const std::string& msg) {
		unexpectedErrorMsg = tr(
				"Caught exception of type \"std::string\".\n\nMessage:\n%1")
				.arg(msg.c_str());
	}
	catch (const std::exception& excpt) {
		const char* name = typeid(excpt).name();
#ifdef __GNUG__
		name = abi::__cxa_demangle(name, 0, 0, 0);
#endif // __GNUG__
		unexpectedErrorMsg =
			tr("Caught exception of type \"%1\".\n\nMessage:\n%2")
			.arg(name).arg(excpt.what());
	}
	catch (const char* &msg) {
		unexpectedErrorMsg =
			tr("Caught exception of type \"char*\".\n\nMessage:\n%1").arg(msg);
	}
	catch (...) {
		unexpectedErrorMsg = tr("Caught exception of unknown type");
	}
	if (!unexpectedErrorMsg.isEmpty()) {
		sout << "\n****************************************************\n\n"
				<< "Unexpected error during load or after execute.\n"
				<< "Please report this to the charon-developers!\n\n"
				<< unexpectedErrorMsg.toStdString() << "\n\n"
				<< "****************************************************"
				<< std::endl;
		_updateLogDialog();
	}
}

void WorkflowExecutor::_cleanup() {
	if(!_manager)
		return;

	_manager->reset();
	sout.assign();
	_log->close();
	QDir::setCurrent(_pathBak);

	delete _log;
	_log = 0;
	delete _manager;
	_manager = 0;

	_logTimer->stop() ;
	_updateLogDialog() ;
	delete _logDialog ;
}

void WorkflowExecutor::_executionFinished()
{
	_updateLogDialog() ;
	_logDialog->infoLabel->setText(tr("Workflow execution finished."));
	_logDialog->progressBar->hide() ;
	QString message ;
	{
		QMutexLocker locker(&_mutex) ;
		message = _executionMessage ;
		_executionMessage.clear() ;
	}

	if(!message.isEmpty())
	{
		sout << "\n****************************************************\n\n"
				<< "Error during execution:\n\n"
				<< message.toStdString() << "\n\n"
				<< "****************************************************"
				<< std::endl;
		_updateLogDialog();
	}

	QSettings settings(
			"Heidelberg Collaboratory for Image Processing",
			"Tuchulcha");
	if (settings.value("waitAfterExecute", false).toBool()) {
		sout << "\n---------------------------------------------------\n\n"
				<< "Workflow execution finished.\n"
				<< "Plugins stay loaded until the stop button is pressed.\n\n"
				<< "---------------------------------------------------"
				<< std::endl;
		_updateLogDialog();
	}
	else
		_cleanup();
	_updateIcon();
}

void WorkflowExecutor::_executionTerminated()
{
	QMessageBox::information(0, tr("execution terminated"),
			tr("Workflow execution thread terminated!"));
	_cleanup() ;
	_updateIcon();

}

void WorkflowExecutor::_updateLogDialog()
{
	
	if(_logDialog && _logFile && _logFile->isReadable())
		while(!_logFile->atEnd() && _logFile->error() == QFile::NoError)
		{
			_logDialog->logText->moveCursor(QTextCursor::End) ;
			_logDialog->logText->insertPlainText(_logFile->readLine());
		}
}


void WorkflowExecutor::execute() {
	
	if(isRunning()) // workflow is executing, kill it?
	{
		QMessageBox msgBox;
		msgBox.setText("Terminate Workflow?");
		msgBox.setInformativeText(tr("The workflow is still being executed.\n"
				"Terminating it may lead to crashs and dataloss! Proceed?"));
		msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
		msgBox.setDefaultButton(QMessageBox::No);
		int ret = msgBox.exec();	
		if(ret == QMessageBox::Yes)
		{	terminate() ;	}
	}
	else
	{
		if (_manager) { // workflow is finished but still loaded
			_cleanup() ;
			_updateIcon();
		}
		else { // workflow ready to start
			_execute() ;
		}
	}
}

#include "WorkflowExecutor.moc"
