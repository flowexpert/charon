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

#include "LogDecorators.h"
#include "FileManager.h"

#include <QApplication>
#include <QDir>
#include <QSettings>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>

// --------------------------   Decorator   ----------------------------------
LogDecorators::Decorator::Decorator() :
		debugOutput(true) {
}

LogDecorators::Decorator::~Decorator() {
}

void LogDecorators::Decorator::processLine(QString) {
}

QString LogDecorators::Decorator::title() const {
	return QString::null;
}

QString LogDecorators::Decorator::desc() const {
	return QString::null;
}

QString LogDecorators::Decorator::filenameHint() const {
	return QString::null;
}

bool LogDecorators::Decorator::ready(QWidget*) const {
	return true;
}

QStringList LogDecorators::Decorator::postStartCommands(QWidget*) const {
	return QStringList();
}

void LogDecorators::Decorator::finishProcessing() {
}

QWidget* LogDecorators::Decorator::statusWidget() {
	return 0;
}

// ---------------------------   update   ------------------------------------
QString LogDecorators::Update::title() const {
	return tr("Plugin Information Update");
}

QString LogDecorators::Update::desc() const {
	return tr("Output of update process:");
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
			tr("wait before plugin update"),
			tr(
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

LogDecorators::Update::Update() :
		_curStatus(Invalid), _result(0), _view(new QTableView)
{
	QString p = "\\(\\w+\\)\\s+"; // prefix
	_fileRegex     = QRegExp(p+"File: (.*)");
	_passRegex     = QRegExp(p+"Created Instance \"\\w+\" of the plugin \"(\\w+)\".*");
	_noPluginRegex = QRegExp(p+"\"(\\w+)\" is no charon plugin.*");
	_failRegex     = QRegExp(p+"Could not generate metadata for module \"(\\w+)\".*");
	_warnRegex     = QRegExp("\\(WW\\)\\s+.*");
	_result = new QStandardItemModel(0,2,this);
	QStandardItem* head = new QStandardItem(tr("file"));
	_result->setHorizontalHeaderItem(0,head);
	head = new QStandardItem(tr("module"));
	_result->setHorizontalHeaderItem(1,head);
	head = new QStandardItem(tr("status"));
	_result->setHorizontalHeaderItem(2,head);
	_result->setSortRole(Qt::UserRole);
	_view->setModel(_result);
	_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_view->horizontalHeader()->setStretchLastSection(true);
	_view->verticalHeader()->hide();
	_view->setSortingEnabled(true);
	_view->setAlternatingRowColors(true);
	_view->setGridStyle(Qt::NoPen);
	_view->setSelectionBehavior(QAbstractItemView::SelectRows);
	_view->setSelectionMode(QAbstractItemView::SingleSelection);
	connect(_view->selectionModel(),
		SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
		SLOT(_searchOutput(QModelIndex)));
}

void LogDecorators::Update::processLine(QString line) {
	if (_fileRegex.exactMatch(line)) {
		if (_curStatus) {
			QList<QStandardItem*> row;
			QStandardItem* curItem(0);

			curItem = new QStandardItem(_curFile);
			curItem->setData(QFont("monospace",9),Qt::FontRole);
			row << curItem;

			curItem = new QStandardItem(_curPlugin);
			curItem->setData(QFont("monospace",9),Qt::FontRole);
			row << curItem;

			curItem = new QStandardItem(_curMsg);
			curItem->setData(_curStatus,Qt::UserRole);
			curItem->setData(QFont("sans-serif",9),Qt::FontRole);
			switch (_curStatus) {
			case Passed:
				curItem->setData(QBrush(QColor("#0A2")),Qt::ForegroundRole);
				break;
			case NoPlugin:
				curItem->setData(QBrush(QColor("#777")),Qt::ForegroundRole);
				break;
			case Warnings:
				curItem->setData(QBrush(QColor("#F84")),Qt::ForegroundRole);
				curItem->setData(QFont("sans-serif",9,QFont::DemiBold),Qt::FontRole);
				break;
			case Failed:
				curItem->setData(QBrush(QColor("#F20")),Qt::ForegroundRole);
				curItem->setData(QFont("sans-serif",9,QFont::Bold),Qt::FontRole);
				break;
			default:
				qDebug("unhandled status code: %d", _curStatus);
				break;
			}
			row << curItem;

			_result->appendRow(row);
			_view->setRowHeight(_result->rowCount()-1,20);
			_view->setColumnWidth(0,_view->width()/2);
			_view->setColumnWidth(1,_view->width()/4);
		}
		_curFile   = _fileRegex.cap(1);
		_curStatus = Failed;
		_curMsg    = tr("failed");
		_curPlugin = QString();
	}
	else if (_noPluginRegex.exactMatch(line)) {
		_curStatus = NoPlugin;
		_curMsg    = tr("no plugin");
		_curPlugin = _noPluginRegex.cap(1);
	}
	else if (_failRegex.exactMatch(line)) {
		_curPlugin = _failRegex.cap(1);
	}
	else if (_warnRegex.exactMatch(line)) {
		_curStatus = Warnings;
		_curMsg    = tr("warnings");
	}
	else if (_passRegex.exactMatch(line)) {
		_curPlugin = _passRegex.cap(1);
		if (_curStatus != Warnings) {
			_curStatus = Passed;
			_curMsg    = tr("passed");
		}
	}
}

void LogDecorators::Update::finishProcessing() {
	_view->sortByColumn(2,Qt::DescendingOrder);
	_view->resizeColumnToContents(0);
}

QWidget* LogDecorators::Update::statusWidget() {
	return _view;
}

void LogDecorators::Update::_searchOutput(const QModelIndex& idx) {
	emit filter(_result->data(_result->index(idx.row(),0),
			Qt::DisplayRole).toString());
}

// -------------------------   update dynamics   ------------------------------
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

// --------------------------   run workflow    -------------------------------
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

void LogDecorators::RunWorkflow::processLine(QString line) {
	// emit signal to highlight currently executed object
	QRegExp curObj("\\(II\\) Executing\\s*\\w*\\s*\"(\\w*)\"\\s*");
	if (curObj.exactMatch(line)) {
		emit highlightObject(curObj.cap(1));
	}
	// add status message if workflow execution finished
	if (line.contains(
		QCoreApplication::translate("CharonRun","Execution finished.")) ||
		line.contains(
		QCoreApplication::translate("CharonRun","Error during execution:"))) {
		emit finish();
		emit message(finishMessage());
	}
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
