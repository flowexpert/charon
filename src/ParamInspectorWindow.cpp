/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/** @file ParamInspectorWindow.cpp
 *  Implementation of class ParamInspectorWindow.
 *  @date 08.08.2008
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include <QtGui>
#include <QDir>
#include <QMessageBox>
#include "ParamInspectorWindow.h"
#include "ObjectInspector.h"
#include "ParameterFileModel.h"
#include "FileManager.h"
#include "RecentFileHandler.h"

ParamInspectorWindow::ParamInspectorWindow(QWidget *pp, Qt::WindowFlags ff) :
		QMainWindow(pp, ff)
{
	_fileMenu = menuBar()->addMenu(tr("&File"));
	_inspector = new ObjectInspector(this);
	ParameterFileModel* model = new ParameterFileModel(
			QString(),_inspector, FileManager::instance().classesFile());
	model->setOnlyParams(false);
	_inspector->setModel(model);
	_inspector->setEdit(true);

	_fileMenu->addAction(tr("&Open"), this,
			SLOT(openFile()), QKeySequence(tr("Ctrl+O")));
	_rfHandler = new RecentFileHandler(this);
	_rfHandler->registerRecentFileEntries(_fileMenu);
	connect(_rfHandler,SIGNAL(openFile(QString)),SLOT(openFile(QString)));
	connect(model,SIGNAL(fileNameChanged(QString)),
			_rfHandler,SLOT(setCurrentFile(QString)));

	_fileMenu->addSeparator();
	_fileMenu->addAction(tr("Open &Metadata"), _inspector,
			SLOT(openMetaData()), QKeySequence(tr("Ctrl+M")));
	_fileMenu->addAction(tr("&Save"), _inspector,
			SLOT(saveFile()), QKeySequence(tr("Ctrl+S")));
	_fileMenu->addAction(tr("Save &as..."), _inspector,
			SLOT(saveFileAs()), QKeySequence(tr("Ctrl+Alt+S")));
	_fileMenu->addAction(tr("E&xit"), this,
			SLOT(close()), QKeySequence(tr("Ctrl+Q")));

	setCentralWidget(_inspector->getViewer());
	QDockWidget* dock = new QDockWidget(tr("Tools"),this);
	dock->setObjectName("tools");
	dock->setWidget(_inspector);
	addDockWidget(Qt::RightDockWidgetArea, dock);

	connect(_inspector, SIGNAL(statusMessage(const QString&, int)),
		statusBar(), SLOT(showMessage(const QString&, int)));

	setWindowTitle(tr("Object Inspector"));

	resize(620, 700);
	QSettings settings;
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("windowState").toByteArray());
}

ParamInspectorWindow::~ParamInspectorWindow() {
	setCentralWidget(0);
	delete _inspector;
	QDockWidget* dock = findChild<QDockWidget*>("tools");
	Q_ASSERT(dock);
	delete dock;
}

void ParamInspectorWindow::closeEvent(QCloseEvent* cEv) {
	QSettings settings;
	settings.setValue("geometry", saveGeometry());
	settings.setValue("windowState",saveState());
	QMainWindow::closeEvent(cEv);
}

void ParamInspectorWindow::openFile(QString fName) {
	try {
		_inspector->openFile(fName);
	} catch (std::string errorMsg) {
		// empty filenames are harmless, simply do nothing
		if (errorMsg.find("Empty filename") == std::string::npos)
			QMessageBox::warning(
				this, tr("Error opening file"), errorMsg.c_str());
	}
	if(_inspector->model()->fileName() != QDir::homePath()) {
		setWindowTitle(tr("Object Inspector")+QString(" [%1]")
			.arg(_inspector->model()->fileName()));
	}
	else {
		setWindowTitle(tr("Object Inspector"));
	}
}
