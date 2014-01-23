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
#include "ui_OptionsDialog.h"

#include <QSettings>
#include <QDir>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QVariant>
#include <QCoreApplication>

OptionsDialog::OptionsDialog(QWidget* pp, Qt::WindowFlags f) :
	QDialog(pp,f)
{
	_ui = new Ui::OptionsDialog;
	_ui->setupUi(this);
#ifdef USE_ASSISTANT
	_ui->bBox->setStandardButtons(
			_ui->bBox->standardButtons() | QDialogButtonBox::Help);
#endif
	QSettings settings;
	if (settings.allKeys().isEmpty()) {
		// use default values if settings environment empty
		restore();
		apply();
	}
	else {
		refresh();
	}
}

OptionsDialog::~OptionsDialog() {
	delete _ui;
}

void OptionsDialog::setTab(int tab) {
	if (tab >= 0 && tab < _ui->tabOptions->count()) {
		_ui->tabOptions->setCurrentIndex(tab);
	}
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
	_ui->checkDebug->setChecked(
		settings.value("suffixedPlugins", false).toBool());
	_ui->checkSplash->setChecked(
		settings.value("splashScreen",true).toBool());
	_ui->checkPathPopup->setChecked(
		settings.value("pathDialogPopup", false).toBool());
	_ui->checkDelay->setChecked(
		settings.value("delayExecution",false).toBool());
	_ui->checkReload->setChecked(
		settings.value("reloadOnStartup",false).toBool());
	_ui->comboCheckLevel->setCurrentIndex(
		settings.value("versionCheckLevel",2).toInt());
	_setExcludes(settings.value("excludeList").toStringList());
	_ui->bButtonStyle->setCurrentIndex(
		settings.value("toolButtonStyle",Qt::ToolButtonFollowStyle).toInt());
	_ui->checkRelativePaths->setChecked(
		settings.value("relativePaths",true).toBool());
}

void OptionsDialog::apply() {
	QSettings settings;
	settings.setValue("globalPluginPath",_ui->eGlobPath->text()
		.split(QRegExp("\\s*;\\s*"),QString::SkipEmptyParts));
	settings.setValue("privatePluginPath",_ui->ePrivPath->text()
		.split(QRegExp("\\s*;\\s*"),QString::SkipEmptyParts));
	settings.setValue("privatePluginPathD",_ui->ePrivPathD->text()
		.split(QRegExp("\\s*;\\s*"),QString::SkipEmptyParts));
	settings.setValue("suffixedPlugins",_ui->checkDebug->isChecked());
	settings.setValue("splashScreen",_ui->checkSplash->isChecked());
	settings.setValue("delayExecution", _ui->checkDelay->isChecked());
	settings.setValue("reloadOnStartup",_ui->checkReload->isChecked());
	settings.setValue("pathDialogPopup",_ui->checkPathPopup->isChecked());
	settings.setValue("versionCheckLevel",_ui->comboCheckLevel->currentIndex());
	settings.setValue("excludeList",_getExcludes());
	settings.setValue("toolButtonStyle",_ui->bButtonStyle->currentIndex());
	settings.setValue("relativePaths",_ui->checkRelativePaths->isChecked());
	if (check()) {
		refresh();
	}
}

void OptionsDialog::restore() {
	QSettings defaults(":/config/default.ini",QSettings::IniFormat);
#ifdef UNIX
	// standard install path on linux systems
	QString globalPath = "/usr/lib/charon-plugins";
#else
	// Assume global plugin dir to be the same directory
	// where the tuchulcha.exe is located
	QString globalPath = QCoreApplication::applicationDirPath();
#endif
	_ui->checkSplash->setChecked(true);
	_ui->eGlobPath->setText(globalPath);
	_ui->ePrivPath->setText(QString());
	_ui->ePrivPathD->setText(QString());
	_ui->checkRelease->setChecked(true);
	_ui->checkDelay->setChecked(false);
	_ui->checkReload->setChecked(false);
	_ui->checkPathPopup->setChecked(false);
	_ui->comboCheckLevel->setCurrentIndex(2);
	_setExcludes(defaults.value("excludeList").toStringList());
	_ui->bButtonStyle->setCurrentIndex(Qt::ToolButtonFollowStyle);
	_ui->checkRelativePaths->setChecked(true);
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

void OptionsDialog::on_bBox_helpRequested() {
	QString page;
	switch (_ui->tabOptions->currentIndex()) {
		case 0:
			page="#options-config";
			break;
		case 1:
			page="#options-paths";
			break;
		case 2:
			page="#options-excludes";
			break;
		case 3:
			page="#options-appearance";
			break;
		default:
			break;
	}
	emit helpRequested(
			QString("tuchulcha-options.html%1").arg(page));
}

void OptionsDialog::_setExcludes(QStringList list) const {
	_ui->tableExclude->clear();
	_ui->tableExclude->setRowCount(list.size()+1);
	for (int i = 0; i < list.size(); i++) {
		_ui->tableExclude->setItem(i,0,new QTableWidgetItem(list[i]));
	}
	_ui->tableExclude->setItem(list.size(),0,new QTableWidgetItem());
}

QStringList OptionsDialog::_getExcludes() const {
	QStringList res;
	for (int i=0; i < _ui->tableExclude->rowCount(); i++) {
		QTableWidgetItem* it = _ui->tableExclude->item(i,0);
		if (it) {
			res << it->data(Qt::DisplayRole).toString().trimmed();
		}
	}
	res.removeAll(QString(""));
	res.removeAll(QString::null);
	return res;
}

void OptionsDialog::on_tableExclude_itemChanged(QTableWidgetItem* item) const {
	QTableWidget* tab = _ui->tableExclude;
	int row = item->row();
	int rowc = tab->rowCount();
	if (item->data(Qt::DisplayRole).toString().isEmpty()) {
		if (row < rowc-1) {
			tab->removeRow(row);
		}
	}
	else {
		if (row >= rowc-1) {
			tab->insertRow(rowc);
		}
	}
}

bool OptionsDialog::check() {
	QSettings settings;
	if (settings.allKeys().isEmpty()) {
		// first start (or config deleted)
		return true;
	}
	bool errors = false;

	// make sure, that the exclude list is populated
	// with default entries if unset
	QSettings defaults(":/config/default.ini",QSettings::IniFormat);
	if(!settings.contains("excludeList")) {
		settings.setValue("excludeList",defaults.value("excludeList"));
	}

	// check current path values
	QStringList pVars, allPaths;
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
		allPaths << paths;
	}
	if (allPaths.isEmpty()) {
		return true; // something went wrong if all paths vanished
	}

	return errors;
}
