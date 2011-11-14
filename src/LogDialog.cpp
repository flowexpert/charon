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

LogDialog::LogDialog(QString title, QString desc, QWidget* pp) :
		QDialog(pp), _proc(0) {
	_ui = new Ui::LogDialog;
	_ui->setupUi(this);
	if (!title.isEmpty()) {
		setWindowTitle(title);
	}
	if (!desc.isEmpty()) {
		_ui->infoLabel->setText(desc);
	}
}

LogDialog::~LogDialog() {
	if (_proc && (_proc->state() != QProcess::NotRunning)) {
		_proc->write("quit\n");
		if(!_proc->waitForFinished(5000)) {
			_proc->terminate();
		}
	}
	delete _ui;
}

void LogDialog::updateContent() {
	if (_proc) {
		QString cur = _proc->readAll();
		_ui->logText->insertPlainText(cur);
		if (cur.contains("execution finished",Qt::CaseInsensitive)) {
			_ui->logText->insertHtml(
				tr("<br><span style=\"color:green;font-weight:bold\">"
					"Workflow execution finished.</span><br>"
					"Plugins stay loaded until you close this dialog."));
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
	setRunning();
	_proc = new QProcess(this);
	connect(_proc,SIGNAL(finished(int)),SLOT(setFinished()));
	connect(_proc,SIGNAL(readyRead()),SLOT(updateContent()));
	connect(_proc,SIGNAL(error(QProcess::ProcessError)),SLOT(error()));
	QString tcRun = QApplication::applicationDirPath() + "/tuchulcha-run";
	_proc->start(tcRun,args,QIODevice::ReadWrite|QIODevice::Text);
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
	_ui->logText->moveCursor(QTextCursor::End);
	_ui->logText->insertHtml(
				tr("<br><span style=\"color:red;font-weight:bold\">"
					"Error during process execution.</span><br>"
					"Error Code: %1"
					"<br><br>").arg(_proc->error()));
}
