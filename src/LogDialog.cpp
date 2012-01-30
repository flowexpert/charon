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
#include "LogDialog.moc"

#include <QTextStream>
#include <QScrollBar>
#include <QTimer>
#include <QMessageBox>
#include <QSettings>
#include <QFileInfo>

LogDialog::LogDialog(Decorator* dec, QWidget* pp, Qt::WindowFlags wf) :
		QDialog(pp,wf), _decorator(dec), _proc(0) {
	_proc = new QProcess(this);
	_proc->setObjectName("proc");
	_ui = new Ui::LogDialog;
	_ui->setupUi(this);

	QString title=_decorator->title();
	QString desc=_decorator->desc();
	if (!title.isEmpty()) {
		setWindowTitle(title);
	}
	if (!desc.isEmpty()) {
		_ui->lInfo->setText(desc);
	}
	_ui->logText->document()->setDefaultStyleSheet(
		"*{white-space:pre;font-family:monospace;font-weight:normal}"
		".error {color:red;font-weight:bold;}"
		".success {color:green;font-weight:bold;font-family:sans-serif;}"
		".warning {color:orange;font-weight:normal;}"
		".info {color:gray;}"
	);
	QTextFrameFormat f;
	_curRet=new QTextCursor(_ui->logText->textCursor().insertFrame(f));
	_ui->logText->moveCursor(QTextCursor::End);
	f.setLeftMargin(10);
	_curEnd=new QTextCursor(_ui->logText->textCursor().insertFrame(f));

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
	QSettings settings;
	QString procName = tcRun;
	if ((!tcRunD.isNull()
				&& settings.value("suffixedPlugins", false).toBool())
			|| tcRun.isNull()) {
		procName = tcRunD;
	}

	if (procName.isNull()) {
		// warn if no valid executable found
		_curEnd->insertHtml(
			QString("<br><span class=\"error\">%1</span></br>")
				.arg(tr("no working %1 process executable found")
					 .arg("<tt>tuchulcha-run</tt>")));
		_ui->lProcName->setText(
			tr("Executable:")+
			QString(" <span style=\"font-weight:bold;color:red\">(%1)</span>")
				.arg(tr("none")));
		return;
	}

	_ui->lProcName->setText(
		tr("Executable: ")
			+QString(" <span style=\"color:blue\"><tt>%1</tt></span>")
				.arg(QFileInfo(procName).baseName()));

	if(_decorator->ready(this)) {
		// start process
		_proc->start(
			procName, _decorator->arguments(),
			QIODevice::ReadWrite|QIODevice::Text);
		this->_ui->lProcName->setText(_ui->lProcName->text()+" PID: "+QString::number(_proc->pid()));
	}
	else {
		// close dialog
		QTimer::singleShot(0,this,SLOT(reject()));
	}
}

LogDialog::~LogDialog() {
	delete _curEnd;
	delete _curRet;
	delete _ui;
	delete _decorator;
}

void LogDialog::done(int r) {
	// terminate process if still running

	if (_proc && (_proc->state() != QProcess::NotRunning)) {
		_curEnd->insertHtml(
			QString("<br><span class=\"warning\">%1</span><br>")
				.arg(tr("waiting for process to quit..."))+
			QString("<span class=\"warning\">%1</span><br>")
				.arg(tr("asking for termination in 1 second")));
		QScrollBar* bar = _ui->logText->verticalScrollBar();
		bar->setValue(bar->maximum());
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
		_curEnd->insertHtml(QString("<span class=\"warning\">%1</span><br>")
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

void LogDialog::on_proc_readyRead() {
	if (_proc) {
		QString origS = QString::fromLocal8Bit(_proc->readAll());
		QString formS, cur;
		QTextStream orig(&origS,QIODevice::ReadOnly);
		QTextStream form(&formS,QIODevice::WriteOnly);

		forever {
			cur = orig.readLine();
			if (cur.isNull()) {
				break;
			}
			cur = _decorator->highlightLine(cur);
			if(_decorator->finishSignal(cur)) {
				_curEnd->insertHtml(_decorator->finishMessage());
				on_proc_finished(0);
			}
			form << cur << "<br>" << endl;
		}
		_curRet->insertHtml(formS);

		// scroll down
		QScrollBar* bar = _ui->logText->verticalScrollBar();
		bar->setValue(bar->maximum());
	}
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

	_curEnd->insertHtml(
		QString("<br><span class=\"error\">%1</span> %2<br>")
			.arg(tr("Error during process execution:"))
			.arg(errorType));
}

// ============================ Decorators ===============================
LogDialog::Decorator::~Decorator() {
}

bool LogDialog::Decorator::finishSignal(QString) {
	return false;
}

QString LogDialog::Decorator::finishMessage() {
	return QString::null;
}

QString LogDialog::Decorator::title() {
	return QString::null;
}

QString LogDialog::Decorator::desc() {
	return QString::null;
}

QString LogDialog::Decorator::highlightLine(QString line) {
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
	else {
		line = QString("<span class=\"normal\">%1</span>").arg(line);
	}
	return line;
}

bool LogDialog::Decorator::ready(QWidget*) {
	return true;
}

QStringList LogDialog::Decorator::postStartCommands(QWidget*) {
	return QStringList();
}

QString LogDecorators::Update::title() {
	return QCoreApplication::translate(
		"LogDecorators::Update", "Plugin Information Update");
}

QString LogDecorators::Update::desc() {
	return QCoreApplication::translate(
		"LogDecorators::Update", "Output of update process:");
}

QStringList LogDecorators::Update::arguments() {
	QStringList args;
	args << "--non-interactive" << "update";
	return args;
}

LogDecorators::RunWorkflow::RunWorkflow(QString fileName) :
		_fileName(fileName) {
}

bool LogDecorators::RunWorkflow::ready(QWidget* pp) {
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

QStringList LogDecorators::RunWorkflow::arguments() {
	QSettings settings;
	QStringList args;
	args << "--quiet";
	if (!settings.value("delayExecution",false).toBool()) {
		args << "run" << _fileName;
	}
	return args;
}

QStringList LogDecorators::RunWorkflow::postStartCommands(QWidget* pp) {
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

QString LogDecorators::RunWorkflow::highlightLine(QString line) {
	QRegExp curObj("^\\(II\\) Executing\\s*\\w*\\s*\"(\\w*)\"\\s*$");
	if (line.contains(curObj)) {
		emit highlightObject(curObj.cap(1));
	}
	return LogDialog::Decorator::highlightLine(line);
}

bool LogDecorators::RunWorkflow::finishSignal(QString line) {
	// add status message if workflow execution finished
	return (line.contains(
		QCoreApplication::translate("CharonRun","Execution finished.")));
}

QString LogDecorators::RunWorkflow::finishMessage() {
	return QString("<br><span class=\"success\">%1</span><br>%2<br>")
		.arg(tr("Workflow execution finished."))
		.arg(tr("Plugins stay loaded until you close this dialog."));
}
