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

LogDialog::LogDialog(QString title, QString desc, QWidget* pp) :
		QDialog(pp), _proc(0) {
	_ui = new Ui::LogDialog;
	_ui->setupUi(this);
	setWindowTitle(title);
	_ui->infoLabel->setText(desc);
}

LogDialog::~LogDialog() {
	delete _ui;
}

void LogDialog::updateContent() {
	if (_proc) {
		_ui->logText->moveCursor(QTextCursor::End);
		_ui->logText->insertPlainText(_proc->readAll());
		_ui->logText->moveCursor(QTextCursor::End);
	}
}

void LogDialog::startProcess(QStringList args) {
	_proc = new QProcess(this);
	_ui->progressBar->show();
	connect(_proc, SIGNAL(finished(int)),_ui->progressBar, SLOT(hide()));
	connect(_proc,SIGNAL(readyRead()),SLOT(updateContent()));
	QString tcRun = QApplication::applicationDirPath() + "/tuchulcha-run";
	_proc->start(tcRun,args,QIODevice::ReadWrite|QIODevice::Text);
}
