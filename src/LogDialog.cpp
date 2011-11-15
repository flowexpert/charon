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

#include <QProcess>
#include <QScrollBar>
#include <QTimer>
#include <QMessageBox>
#include <QSettings>
#include <QFileInfo>

LogDialog::LogDialog(QString title, QString desc, QWidget* pp) :
		QDialog(pp), _proc(0) {
	_ui = new Ui::LogDialog;
	_ui->setupUi(this);
	if (!title.isEmpty()) {
		setWindowTitle(title);
	}
	if (!desc.isEmpty()) {
		_ui->lInfo->setText(desc);
	}
}

LogDialog::~LogDialog() {
	delete _ui;
}

void LogDialog::done(int r) {
	// terminate process if still running

	if (_proc && (_proc->state() != QProcess::NotRunning)) {
		_ui->logText->moveCursor(QTextCursor::End);
		_ui->logText->insertHtml(
			tr("<br><span style=\"color:orange;font-weight:bold\">"
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

void LogDialog::updateContent() {
	if (_proc) {
		QString cur = _proc->readAll();
		_ui->logText->insertPlainText(cur);
		if (cur.contains("execution finished",Qt::CaseInsensitive)) {
			_ui->logText->insertHtml(
				tr("<br><span style=\"color:green;font-weight:bold\">"
					"Workflow execution finished.</span><br>"
					"Plugins stay loaded until you close this dialog.<br>"));
			_ui->logText->moveCursor(QTextCursor::PreviousBlock);
			_ui->logText->moveCursor(QTextCursor::EndOfBlock);
			setFinished();
		}
		QScrollBar* bar = _ui->logText->verticalScrollBar();
		bar->setValue(bar->maximum());
	}
}

void LogDialog::startProcess(QStringList args) {
	if (_proc) {
		return;
	}

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
		_ui->logText->insertHtml(
					tr("<br><span style=\"color:red;font-weight:bold\">"
						"no working <tt>tuchulcha-run</tt> "
						"process executable found<br>"));
		_ui->lProcName->setText(
			tr("Executable: "
				"<span style=\"font-weight:bold;color:red\">(none)</span>"));
		setFinished();
		return;
	}

	_ui->lProcName->setText(
		tr("Executable: "
			"<span style=\"color:blue\"><tt>%1</tt></span>")
		.arg(QFileInfo(procName).baseName()));

	// start process
	setRunning();
	_proc = new QProcess(this);
	connect(_proc,SIGNAL(finished(int)),SLOT(setFinished()));
	connect(_proc,SIGNAL(readyRead()),SLOT(updateContent()));
	connect(_proc,SIGNAL(error(QProcess::ProcessError)),SLOT(error()));
	_proc->start(procName,args,QIODevice::ReadWrite|QIODevice::Text);
}

void LogDialog::setRunning() {
	_ui->progressBar->show();
	_ui->buttonBox->setStandardButtons(QDialogButtonBox::Abort);
}

void LogDialog::setFinished() {
	_ui->progressBar->hide();
	_ui->buttonBox->setStandardButtons(QDialogButtonBox::Close);
}

void LogDialog::error() {
	setFinished();

	_ui->logText->moveCursor(QTextCursor::End);
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

	_ui->logText->insertHtml(
				tr("<br><span style=\"color:red;font-weight:bold\">"
					"Error during process execution:</span> %1<br>")
				.arg(errorType));
}
