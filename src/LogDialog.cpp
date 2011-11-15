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
#include <QTextStream>
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
	_ui->logText->document()->setDefaultStyleSheet(
		".error {color:red;font-weight:bold;}"
		".success {color:green;font-weight:bold;}"
		".warning {color:orange;font-weight:bold;}"
		".info {color:gray;}"
	);
	QTextFrameFormat f;
	_curRet=new QTextCursor(_ui->logText->textCursor().insertFrame(f));
	_ui->logText->moveCursor(QTextCursor::End);
	f.setLeftMargin(10);
	_curEnd=new QTextCursor(_ui->logText->textCursor().insertFrame(f));
}

LogDialog::~LogDialog() {
	delete _curEnd;
	delete _curRet;
	delete _ui;
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

void LogDialog::updateContent() {
	if (_proc) {
		QString origS = _proc->readAll();
		QString formS, cur;
		QTextStream orig(&origS,QIODevice::ReadOnly);
		QTextStream form(&formS,QIODevice::WriteOnly);

		do {
			cur = orig.readLine();

			// simple markup for higlighting
			if (cur.contains(
					QRegExp("^\\(II\\)\\s+",Qt::CaseInsensitive))) {
				cur = QString("<span class=\"info\">%1</span>").arg(cur);
			}
			else if (cur.contains(
					QRegExp("^\\(WW\\)\\s+",Qt::CaseInsensitive))) {
				cur = QString("<span class=\"warning\">%1</span>").arg(cur);
			}
			else if (cur.contains(
					QRegExp("^\\(EE\\)\\s+",Qt::CaseInsensitive))) {
				cur = QString("<span class=\"error\">%1</span>").arg(cur);
			}

			// add status message if workflow execution finished
			if (cur.contains("execution finished",Qt::CaseInsensitive)) {
				_curEnd->insertHtml(
					QString("<br><span class=\"success\">%1</span><br>%2<br>")
						.arg(tr("Workflow execution finished."))
						.arg(tr("Plugins stay loaded until you close "
							"this dialog.")));
				setFinished();
			}
			form << cur << "<br>" << endl;
		} while (!cur.isNull());
		_curRet->insertHtml(formS);

		// scroll down
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
		_curEnd->insertHtml(
					QString("<br><span class=\"error\">%1</span></br>")
						.arg("no working <tt>tuchulcha-run</tt> "
						"process executable found"));
		_ui->lProcName->setText(
			tr("Executable:")+
			QString(" <span style=\"font-weight:bold;color:red\">(%1)</span>")
				.arg(tr("none")));
		setFinished();
		return;
	}

	_ui->lProcName->setText(
		tr("Executable: ")
			+QString(" <span style=\"color:blue\"><tt>%1</tt></span>")
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
