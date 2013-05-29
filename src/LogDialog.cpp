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
/** \file LogDialog.cpp
 *  \brief Implementation of class LogDialog
 *
 *  \date 13.11.2011
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "LogDialog.h"
#include "ui_LogDialog.h"
#include "FileManager.h"

#include <QTextStream>
#include <QScrollBar>
#include <QTimer>
#include <QMessageBox>
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <QMutexLocker>
#include <QStringListModel>
#include <QSortFilterProxyModel>

#include <QPainter>
#include <QSplashScreen>

LogDialog::LogDialog(Decorator* dec, QWidget* pp, Qt::WindowFlags wf) :
		QDialog(pp,wf), _log(0), _logProx(0), _decorator(dec), _proc(0),
		_logFile(0), _logMutex(new QMutex())
{
	QSettings settings;
	_proc = new QProcess(this);
	_proc->setObjectName("proc");
	_ui = new Ui::LogDialog;
	_ui->setupUi(this);

	// set up log models
	_log = new QStringListModel(this);
	_logProx = new QSortFilterProxyModel(_log);
	_logProx->setSourceModel(_log);
	_ui->logView->setModel(_logProx);

	// apply saved settings
	settings.beginGroup("LogDialog");
	_ui->checkDD->setChecked(settings.value("showDebugOutput",true).toBool());
	_ui->checkScroll->setChecked(settings.value("autoScroll",true).toBool());
	settings.endGroup();
	_decorator->debugOutput = _ui->checkDD->isChecked();

	QString title=_decorator->title();
	QString desc=_decorator->desc();
	if (!title.isEmpty()) {
		setWindowTitle(title);
	}
	if (!desc.isEmpty()) {
		_ui->lInfo->setText(desc);
	}

	resetLogWidget();
	_ui->infoDisplay->setVisible(false);

	// determine available run process executables
	QStringList testArgs;
	testArgs << "--quiet" << "--non-interactive";
	QString tcRun = QApplication::applicationDirPath() + "/tuchulcha-run";
	if (QProcess::execute(tcRun,testArgs)) {
		tcRun = QString::null;
	}
	QString tcRunD = QApplication::applicationDirPath() + "/tuchulcha-run_d";
	if (QProcess::execute(tcRunD,testArgs)) {
		tcRunD = QString::null;
	}

	// select process executable
	QString procName = tcRun;
	if ((!tcRunD.isNull()
				&& settings.value("suffixedPlugins", false).toBool())
			|| tcRun.isNull()) {
		procName = tcRunD;
	}

	if (procName.isNull()) {
		// warn if no valid executable found
		printStatus(QString("<br><span class=\"error\">%1</span></br>")
				.arg(tr("no working %1 process executable found")
					 .arg("<tt>tuchulcha-run</tt>")));
		_ui->lProcName->setText(
			tr("Executable:")+
			QString(" <span style=\"font-weight:bold;color:red\">(%1)</span>")
				.arg(tr("none")));
		return;
	}

	QString procText =
		tr("Executable: ")
			+QString(" <span style=\"color:blue\"><tt>%1</tt></span>")
				.arg(QFileInfo(procName).baseName());

	// open log file
	_logFile = new QFile(_decorator->logFileName(), this);
	_logFile->open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text);
	procText = QString("%1 %2: <span style=\"color:blue\"><tt>%3</tt></span>")
			.arg(procText).arg(tr("Log"))
			.arg(QFileInfo(_logFile->fileName()).fileName());
	_ui->lProcName->setText(procText);

	if(_decorator->ready(this)) {
		// start process
		_proc->start(
			procName, _decorator->arguments(),
			QIODevice::ReadWrite|QIODevice::Text);
	#ifdef Q_OS_LINUX
		procText =
			QString("%1 PID: <span style=\"color:blue\"><tt>%2</tt></span>")
				.arg(procText).arg(_proc->pid());
		_ui->lProcName->setText(procText);
	#endif
	}
	else {
		// close dialog
		QTimer::singleShot(0,this,SLOT(reject()));
	}
}

LogDialog::~LogDialog() {
	QSettings settings;
	settings.beginGroup("LogDialog");
	settings.setValue("showDebugOutput",_ui->checkDD->isChecked());
	settings.setValue("autoScroll",_ui->checkScroll->isChecked());
	settings.endGroup();
	delete _ui;
	delete _decorator;
	delete _logMutex;
}

void LogDialog::resetLogWidget() {
	_ui->infoDisplay->document()->setDefaultStyleSheet(
		"*{white-space:pre;font-family:monospace;font-weight:normal}"
		".error {color:red;font-weight:bold;}"
		".success {color:green;font-weight:bold;font-family:sans-serif;}"
		".warning {color:orange;font-weight:normal;}"
		".info {color:#444;}"
		".debug {color:gray;}"
	);
}

void LogDialog::done(int r) {
	// terminate process if still running

	if (_proc && (_proc->state() != QProcess::NotRunning)) {
		printStatus(
			QString("<br><span class=\"warning\">%1</span><br>")
				.arg(tr("waiting for process to quit..."))+
			QString("<span class=\"warning\">%1</span><br>")
				.arg(tr("asking for termination in 1 second")));
		_proc->write("quit\n");
		connect(_proc,SIGNAL(finished(int)),SLOT(close()));
		QTimer::singleShot(1000,this,SLOT(terminate()));
	}
	else {
		// quit event loop and return
		QDialog::done(r);
	}
}

void LogDialog::terminate(bool force) {
	if (_proc && (_proc->state() != QProcess::NotRunning)
			&& (force || QMessageBox::question(
				this,tr("confirm terminate"),
				tr("Process still running.<br>Terminate running process?"),
				QMessageBox::No,QMessageBox::Yes)==QMessageBox::Yes)) {
		printStatus(QString("<span class=\"warning\">%1</span><br>")
			.arg(tr("asking for kill in 3 seconds")));
		QTimer::singleShot(0,_proc,SLOT(terminate()));
		QTimer::singleShot(3000,this,SLOT(kill()));
	}
}

void LogDialog::kill(bool force) {
	if (_proc && (_proc->state() != QProcess::NotRunning)
			&& (force || QMessageBox::question(
				this,tr("confirm kill"),
				tr("Process did not respond.<br>Kill running process?"),
				QMessageBox::No,QMessageBox::Yes)==QMessageBox::Yes)) {
		_proc->kill();
	}
}

bool LogDialog::waitForFinished(int msecs) {
	if (_proc->state() == QProcess::Starting) {
		_proc->waitForStarted(msecs);
	}
	if (_proc->state() == QProcess::Running) {
		return _proc->waitForFinished(msecs);
	}
	return false;
}

void LogDialog::on_proc_readyReadStandardOutput() {
	QMutexLocker mLock(_logMutex);
	QString origS = QString::fromLocal8Bit(_proc->readAllStandardOutput());
	QString cur;
	QTextStream orig(&origS,QIODevice::ReadOnly);
	QStringList logList = _log->stringList();
	QTextStream logFile(_logFile);

	forever {
		cur = orig.readLine();
		if (cur.isNull()) {
			break;
		}
		logFile << cur << endl;
		if(_decorator->finishSignal(cur)) {
			printStatus(_decorator->finishMessage());
			on_proc_finished(0);
		}
		//cur = _decorator->highlightLine(cur);
		if (!cur.isNull()) {
			logList << cur;
		}
	}
	_log->setStringList(logList);

	if (_ui->checkScroll->isChecked()) {
		_ui->logView->scrollToBottom();
	}

	mLock.unlock();
}

void LogDialog::on_proc_readyReadStandardError() {
	QMutexLocker mLock(_logMutex);
	QString origS = QString::fromLocal8Bit(_proc->readAllStandardError());
	QString cur;
	QTextStream orig(&origS,QIODevice::ReadOnly);
	QStringList logList = _log->stringList();
	QTextStream logFile(_logFile);

	forever {
		cur = orig.readLine();
		if (cur.isNull()) {
			break;
		}
		if(_decorator->finishSignal(cur)) {
			printStatus(_decorator->finishMessage());
			on_proc_finished(0);
		}
		if (cur.contains(
				QRegExp("^\\(EE\\)\\s+",Qt::CaseInsensitive))) {
			logFile << cur << endl;
		}
		else {
			logFile << "(EE) " << cur << endl;
		}
		//cur = QString("<span class=\"error\">%1</span>").arg(cur);
		logList << cur;
	}

	_log->setStringList(logList);

	if (_ui->checkScroll->isChecked()) {
		_ui->logView->scrollToBottom();
	}

	mLock.unlock();
}

void LogDialog::reprint() {
	if (!_logFile || !_logFile->isOpen()) {
		// reprint only useful after initialization
		return;
	}
	QMutexLocker mLock(_logMutex);
	QString cur;
	_logFile->close();
	_logFile->open(QIODevice::ReadOnly|QIODevice::Text);
	QTextStream log(_logFile);
	QStringList logList;

	forever {
		cur = log.readLine();
		if (cur.isNull()) {
			break;
		}
		//cur = _decorator->highlightLine(cur);
		if (!cur.isNull()) {
			logList << cur;
		}
	}
	_log->setStringList(logList);

	if (_ui->checkScroll->isChecked()) {
		_ui->logView->scrollToBottom();
	}

	_logFile->close();
	_logFile->open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text);

	mLock.unlock();
}

void LogDialog::on_proc_started() {
	_ui->progressBar->show();
	_ui->buttonBox->setStandardButtons(QDialogButtonBox::Abort);

	QStringList postStart = _decorator->postStartCommands(this);
	QTextStream pout(_proc);
	foreach(const QString& cmd, postStart) {
		pout << cmd << endl;
	}
}

void LogDialog::on_proc_finished(int) {
	_ui->progressBar->hide();
	_ui->buttonBox->setStandardButtons(QDialogButtonBox::Close);
}

void LogDialog::on_proc_error(QProcess::ProcessError) {
	on_proc_finished(-1);

	QString errorType;
	switch(_proc->error()) {
	case QProcess::FailedToStart:
		errorType = tr("process failed to start");
		break;
	case QProcess::Crashed:
		errorType = tr("process crashed after start");
		break;
	case QProcess::Timedout:
		errorType = tr("process timeout");
		break;
	case QProcess::WriteError:
		errorType = tr("write error");
		break;
	case QProcess::ReadError:
		errorType = tr("read error");
		break;
	default:
		errorType = tr("unknown error");
		break;
	}

	printStatus(QString("<br><span class=\"error\">%1</span> %2<br>")
				.arg(tr("Error during process execution:"))
				.arg(errorType));
}

void LogDialog::printStatus(QString msg) {
	_ui->infoDisplay->setVisible(true);
	_ui->infoDisplay->insertHtml(msg);
}

void LogDialog::on_checkDD_toggled(bool checked) {
	if (!_logProx) {
		return;
	}
	if (checked) {
		_logProx->setFilterRegExp(QString());
	}
	else {
		_logProx->setFilterRegExp(
		QRegExp("^\\((?!DD)|^(?!\\()"));
	}
	if (_ui->checkScroll->isChecked()) {
		_ui->logView->scrollToBottom();
	}
}

void LogDialog::on_buttonSave_clicked() {
	QString selFilter;
	QString fName = QFileDialog::getSaveFileName(
				this,tr("Save Log File"),_decorator->filenameHint(),
				tr("Text File (*.txt *.log)"),
				&selFilter);
	if (fName.isEmpty())
		return;
	QMutexLocker mLock(_logMutex);
	_logFile->close();
	_logFile->copy(fName);
	_logFile->open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text);
	mLock.unlock();
}

// ============================ Decorators ===============================
LogDialog::Decorator::Decorator() :
		debugOutput(true) {
}

LogDialog::Decorator::~Decorator() {
}

bool LogDialog::Decorator::finishSignal(QString) const {
	return false;
}

QString LogDialog::Decorator::finishMessage() const {
	return QString::null;
}

QString LogDialog::Decorator::title() const {
	return QString::null;
}

QString LogDialog::Decorator::desc() const {
	return QString::null;
}

QString LogDialog::Decorator::filenameHint() const {
	return QString::null;
}

QString LogDialog::Decorator::highlightLine(QString line) const {
	// simple markup for higlighting
	if (line.contains(
			QRegExp("^\\(II\\)\\s+",Qt::CaseInsensitive))) {
		line = QString("<span class=\"info\">%1</span>").arg(line);
	}
	else if (line.contains(
			QRegExp("^\\(WW\\)\\s+",Qt::CaseInsensitive))) {
		line = QString("<span class=\"warning\">%1</span>").arg(line);
	}
	else if (line.contains(
			QRegExp("^\\(EE\\)\\s+",Qt::CaseInsensitive))) {
		line = QString("<span class=\"error\">%1</span>").arg(line);
	}
	else if (line.contains(
			QRegExp("^\\(DD\\)\\s+",Qt::CaseInsensitive))) {
		if (debugOutput) {
			line = QString("<span class=\"debug\">%1</span>").arg(line);
		}
		else {
			line = QString::null;
		}
	}
	else {
		line = QString("<span class=\"normal\">%1</span>").arg(line);
	}
	return line;
}

bool LogDialog::Decorator::ready(QWidget*) const {
	return true;
}

QStringList LogDialog::Decorator::postStartCommands(QWidget*) const {
	return QStringList();
}

QString LogDecorators::Update::title() const {
	return QCoreApplication::translate(
		"LogDecorators::Update", "Plugin Information Update");
}

QString LogDecorators::Update::desc() const {
	return QCoreApplication::translate(
		"LogDecorators::Update", "Output of update process:");
}

QStringList LogDecorators::Update::arguments() const {
	QSettings settings;
	QStringList args;
	args << "--quiet";
	if (!settings.value("delayExecution",false).toBool()) {
		args << "--non-interactive" << "update";
	}
	return args;
}

QStringList LogDecorators::Update::postStartCommands(QWidget* pp) const {
	QSettings settings;
	QStringList cmds;
	if (settings.value("delayExecution",false).toBool()) {
		QMessageBox::information(pp,
			QCoreApplication::translate("LogDecorators::Update",
				"wait before plugin update"),
			QCoreApplication::translate("LogDecorators::Update",
				"Waiting because <em>delayExecution</em> option set. "
				"You can now attach your debugger to the update process. "
				"Update will be started, when you close this message box."
			)
		);
		cmds << "update" << "quit";
	}
	return cmds;
}

QString LogDecorators::Update::filenameHint() const {
	return QDir::home().absoluteFilePath("update-modules.log");
}

QString LogDecorators::Update::logFileName() const {
	return FileManager::instance().configDir()
			.absoluteFilePath("updateLog.txt");
}

LogDecorators::UpdateDynamics::UpdateDynamics(QString fileName) :
		_fileName(fileName) {
}

QString LogDecorators::UpdateDynamics::logFileName() const {
	return FileManager::instance().configDir()
			.absoluteFilePath("updateDynLog.txt");
}

QStringList LogDecorators::UpdateDynamics::arguments() const {
	QStringList args;
	args << "--non-interactive" << "update-dynamics" << _fileName;
	return args;
}

LogDecorators::RunWorkflow::RunWorkflow(QString fileName) :
		_fileName(fileName) {
}

bool LogDecorators::RunWorkflow::ready(QWidget* pp) const {
	if(_fileName.isEmpty()) {
		QMessageBox::warning(pp,
			tr("missing workflow file"),
			tr("The workflow cannot be started because it has not "
				"been saved to disk (empty filename given). "
				"Please save it and retry execution."
			)
		);
		return false;
	}
	return true;
}

QStringList LogDecorators::RunWorkflow::arguments() const {
	QSettings settings;
	QStringList args;
	args << "--quiet";
	if (!settings.value("delayExecution",false).toBool()) {
		args << "run" << _fileName;
	}
	return args;
}

QStringList LogDecorators::RunWorkflow::postStartCommands(QWidget* pp) const {
	QSettings settings;
	QStringList cmds;
	if (settings.value("delayExecution",false).toBool()) {
		QMessageBox::information(pp,
			tr("wait before workflow execution"),
			tr("Waiting because <em>delayExecution</em> option set. "
				"You can now attach your debugger to the run process. "
				"Workflow will be started, when you close this message box."
			)
		);
		cmds << QString("run %1").arg(_fileName);
	}
	return cmds;
}

QString LogDecorators::RunWorkflow::highlightLine(QString line) const {
	QRegExp curObj("^\\(II\\) Executing\\s*\\w*\\s*\"(\\w*)\"\\s*$");
	if (line.contains(curObj)) {
		emit highlightObject(curObj.cap(1));
	}
	return LogDialog::Decorator::highlightLine(line);
}

bool LogDecorators::RunWorkflow::finishSignal(QString line) const {
	// add status message if workflow execution finished
	return (line.contains(
		QCoreApplication::translate("CharonRun","Execution finished.")) ||
		line.contains(
		QCoreApplication::translate("CharonRun","Error during execution:")));
}

QString LogDecorators::RunWorkflow::finishMessage() const {
	return QString("<br><span class=\"success\">%1</span><br>%2<br>")
		.arg(tr("Workflow execution finished."))
		.arg(tr("Plugins stay loaded until you close this dialog."));
}

QString LogDecorators::RunWorkflow::filenameHint() const {
	QFileInfo wrpFile(_fileName);
	return QString("%1/%2.log")
			.arg(wrpFile.absolutePath()).arg(wrpFile.baseName());
}

QString LogDecorators::RunWorkflow::logFileName() const {
	return FileManager::instance().configDir()
			.absoluteFilePath("executeLog.txt");
}
