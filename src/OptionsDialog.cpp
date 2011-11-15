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
/** \file   OptionsDialog.cpp
 *  \brief  Implementation of class OptionsDialog
 *
 *  \date   15.11.2011
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "OptionsDialog.h"
#include "OptionsDialog.moc"
#include "ui_OptionsDialog.h"

#include <QSettings>
#include <QDir>
#include <QMessageBox>
#include <QCoreApplication>

OptionsDialog::OptionsDialog(QWidget* pp, Qt::WindowFlags f) :
	QDialog(pp,f)
{
	_ui = new Ui::OptionsDialog;
	_ui->setupUi(this);
	refresh();
}

OptionsDialog::~OptionsDialog() {
	delete _ui;
}

void OptionsDialog::refresh() {
	// set up dialog content
	QSettings settings;
	_ui->eGlobPath->setText(
		settings.value("globalPluginPath").toStringList().join("; "));
	_ui->ePrivPath->setText(
		settings.value("privatePluginPath").toStringList().join("; "));
	_ui->ePrivPathD->setText(
		settings.value("privatePluginPathD").toStringList().join("; "));
	_ui->checkSuffix->setChecked(
		settings.value("suffixedPlugins", false).toBool());
}

void OptionsDialog::apply() {
	QSettings settings;
	settings.setValue("globalPluginPath",_ui->eGlobPath->text()
		.split(QRegExp("\\s*;\\s*"),QString::SkipEmptyParts));
	settings.setValue("privatePluginPath",_ui->ePrivPath->text()
		.split(QRegExp("\\s*;\\s*"),QString::SkipEmptyParts));
	settings.setValue("privatePluginPathD",_ui->ePrivPathD->text()
		.split(QRegExp("\\s*;\\s*"),QString::SkipEmptyParts));
	settings.setValue("suffixedPlugins",_ui->checkSuffix->isChecked());
	if (check()) {
		refresh();
	}
}

void OptionsDialog::restore() {
#ifdef UNIX
	// standard install path on linux systems
	QString globalPath = "/usr/lib/charon-plugins";
#else
	// Assume global plugin dir to be the same directory
	// where the tuchulcha.exe is located
	QString globalPath = QCoreApplication::applicationDirPath();
#endif
	_ui->eGlobPath->setText(globalPath);
	_ui->ePrivPath->setText(QString());
	_ui->ePrivPathD->setText(QString());
	_ui->checkSuffix->setChecked(false);
}

void OptionsDialog::on_bBox_clicked(QAbstractButton* button) {
	switch (_ui->bBox->buttonRole(button)) {
		case QDialogButtonBox::ResetRole:
			restore();
			break;
		case QDialogButtonBox::ApplyRole:
		case QDialogButtonBox::AcceptRole:
			apply();
			break;
		default:
			break;
	}
}

bool OptionsDialog::check() {
	QSettings settings;
	bool errors = false;

	// check current path values
	QStringList pVars;
	pVars<<"globalPluginPath"<<"privatePluginPath"<<"privatePluginPathD";

	foreach (const QString& pVar, pVars) {
		QStringList paths = settings.value(pVar).toStringList();
		QStringListIterator pIter(paths);
		while (pIter.hasNext()) {
			const QString& p = pIter.next();
			if(!QDir(p).exists()) {
				errors = true;
				if (QMessageBox::question(
					0,tr("path not found"),
					tr("path <tt>%1</tt> has not been found.</br>"
						"Remove it from <em>%2</em>?").arg(p).arg(pVar),
					QMessageBox::Yes,QMessageBox::No) ==
						QMessageBox::Yes) {
					paths.removeAll(p);
				}
			}
		}
		settings.setValue(pVar,paths);
	}
	if (settings.value("globalPluginPath").toStringList().isEmpty()) {
		errors = true;
	}

	return errors;
}
