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
	QString procName =
		(settings.value("suffixedPlugins", false).toBool() || tcRun.isNull())?
		tcRunD : tcRun;

	if (procName.isNull()) {
		// warn if no valid executable found
		_curEnd->insertHtml(
					QString("<br><span class=\"error\">%1</span></br>")
						.arg("no working <tt>tuchulcha-run</tt> "
						"process executable found"));
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

	// start process
	_proc->start(
		procName, _decorator->arguments(),
		QIODevice::ReadWrite|QIODevice::Text);
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
			tr("<br><span class=\"warning\">"
				"Waiting for process to terminate</span><br>"));
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
		_proc->terminate();
	}
}

void LogDialog::on_proc_readyRead() {
	if (_proc) {
		QString origS = _proc->readAll();
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

QStringList LogDialog::Decorator::postStartCommands(QWidget*) {
	return QStringList();
}

QString LogDecorators::Update::title() {
	return QCoreApplication::translate(
		"UpdateDecorator","Plugin Information Update");
}

QString LogDecorators::Update::desc() {
	return QCoreApplication::translate(
		"UpdateDecorator","Output of update process:");
}

QStringList LogDecorators::Update::arguments() {
	QStringList args;
	args << "--non-interactive" << "update";
	return args;
}

LogDecorators::RunWorkflow::RunWorkflow(QString fileName) :
	_fileName(fileName) {
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
			QCoreApplication::translate("RunDecorator",
				"wait before workflow execution"),
			QCoreApplication::translate("RunDecorator",
				"Waiting because <em>delayExecution</em> option set. "
				"You can now attach your debugger to the run process. "
				"Workflow will be started, when you close this message box."
			)
		);
		cmds << QString("run %1").arg(_fileName);
	}
	return cmds;
}

bool LogDecorators::RunWorkflow::finishSignal(QString line) {
	// add status message if workflow execution finished
	return (line.contains("execution finished",Qt::CaseInsensitive));
}

QString LogDecorators::RunWorkflow::finishMessage() {
	return QString("<br><span class=\"success\">%1</span><br>%2<br>")
		.arg(QCoreApplication::translate("RunDecorator",
			"Workflow execution finished."))
		.arg(QCoreApplication::translate("RunDecorator",
			"Plugins stay loaded until you close this dialog."));
}
