/*	Copyright (C) 2009 Jens-Malte Gottfried

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
/**	@file MainWindow.cpp
 *	@brief Implementation of class MainWindow
 *
 *	@date	27.08.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include <QtGui>
#include <QWebView>

#include "MainWindow.h"
#include "ObjectInspector.h"
#include "FlowWidget.h"
#include "SelectorWidget.h"
#include "GraphModel.h"
#include "DocGenerator.h"
#include "ModelToolBar.h"
#include "FileManager.h"
#include <ParameterFile.hxx>
#include <PluginManager.h>
#include <iostream>
#include "AbstractPluginLoader.h"

#ifndef SVNINFO
#define SVNINFO
#endif

#include "MainWindow.moc"

MainWindow::MainWindow(QWidget* myParent) :
	QMainWindow(myParent), _flow(0) {

	FileManager::instance().configure();
	FileManager::instance().updateMetadata();

	setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
	setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
	setDockNestingEnabled(true);

	// simple application icon
	QPixmap appicon(":/icons/appicon.png");
	setWindowIcon(appicon);

	// Application Title
	setWindowTitle("ParamEdit");

	// object inspector
	QDockWidget* inspectorWidget = new QDockWidget(tr("ObjectInspector"), this);
	inspectorWidget->setObjectName("inspectorwidget");
	ObjectInspector* inspector = new ObjectInspector(inspectorWidget);
	inspectorWidget->setWidget(inspector);

	// help viewer
	QDockWidget* helpWidget = new QDockWidget(tr("Help Browser"), this);
	helpWidget->setObjectName("helpwidget");
	QWebView* helpBrowser = new QWebView(helpWidget);
	helpWidget->setWidget(helpBrowser);
	DocGenerator* docGen = new DocGenerator(helpBrowser, this);
	docGen->showIntro();

	// select widget
	QDockWidget* selectWidget = new QDockWidget(tr("Selector"), this);
	selectWidget->setObjectName("selectwidget");
	_selector = new SelectorWidget(selectWidget);
	selectWidget->setWidget(_selector);

	// object inspector connections
	connect(inspector, SIGNAL(statusMessage(const QString&, int)),
			statusBar(), SLOT(showMessage(const QString&, int)));
	connect(this, SIGNAL(activeGraphModelChanged(ParameterFileModel*)),
			inspector, SLOT(setModel(ParameterFileModel*)));
	connect(this, SIGNAL(enableEditors(bool)),
			inspector, SLOT(setEnabled(bool)));

	// selector widget connections
	connect(this, SIGNAL(activeGraphModelChanged(ParameterFileModel*)),
			_selector, SLOT(setModel(ParameterFileModel*)));
	connect(this, SIGNAL(enableEditors(bool)),
			_selector, SLOT(setEnabled(bool)));

	// help browser connections
	connect(this, SIGNAL(activeGraphModelChanged(ParameterFileModel*)),
			docGen, SLOT(setModel(ParameterFileModel*)));
	connect(_selector, SIGNAL(showClassDoc(QString)), docGen, SLOT(
			showClassDoc(QString)));
	connect(_selector, SIGNAL(showDocPage(QString)), docGen, SLOT(showDocPage(
			QString)));

	// add widgets to dock area
	addDockWidget(Qt::RightDockWidgetArea, inspectorWidget);
	addDockWidget(Qt::BottomDockWidgetArea, selectWidget, Qt::Vertical);
	addDockWidget(Qt::BottomDockWidgetArea, helpWidget, Qt::Vertical);

	_centralArea = new QMdiArea(this);
	setCentralWidget(_centralArea);

	connect(_centralArea, SIGNAL(subWindowActivated (QMdiSubWindow*)),
			this, SLOT(_windowActivated(QMdiSubWindow*)));

	// toolbar
	ModelToolBar* toolbar = new ModelToolBar(tr("toolbar"));
	toolbar->setObjectName("toolbar");
	addToolBar(toolbar);
	toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
	toolbar->setIconSize(QSize(32, 32));
	QAction* action;

	action = toolbar->addAction(QIcon(":/icons/new.png"), tr("new\nfile"),
			this, SLOT(openNew()));
	action->setToolTip(tr("create a new file"));

	action = toolbar->addAction(QIcon(":/icons/open.png"), tr("open\nfile"),
			this, SLOT(open()));
	action->setToolTip(tr("open an existing file"));

	action = toolbar->addAction(QIcon(":/icons/save.png"), tr("save\nfile"),
			inspector, SLOT(saveFile()));
	action->setToolTip(tr("save current document"));

	action = toolbar->addAction(QIcon(":/icons/save_as.png"), tr(
			"save\nfile as"), inspector, SLOT(saveFileAs()));
        action->setToolTip(tr("save current document\nto a new location"));

	action = toolbar->addAction(QIcon(":/icons/export.png"), tr(
			"export\nflowchart"), this, SLOT(saveFlowChart()));
	action->setToolTip(tr("export flowchart to an image file"));

	toolbar->addSeparator();
	action = toolbar->addAction(QIcon(":/icons/refresh.png"), tr("&Update Plugins"),
		this, SLOT(updateMetadata()));
	action->setToolTip(tr("update classes informations reading all plugins"));
	action = toolbar->addAction(QIcon(":/icons/runbuild.png"), tr("&Compile and load plug-in"),
		this, SLOT(compileAndLoad()));
	action->setToolTip(tr("compile new plugin from plugin source"));
	action = toolbar->addAction(QIcon(":/icons/execute.png"), tr("Execute &Workflow"),
		inspector, SLOT(executeWorkflow()));
	action->setToolTip(tr("execute workflow that is described in the current window"));
	toolbar->addSeparator();
	toolbar->addModelActions();
	toolbar->addSeparator();
	action = toolbar->addAction(QIcon(":/icons/zoomIn.png"), tr("zoom\nin"),
			this, SLOT(zoomIn()));
	action->setToolTip(tr("enlarge flowchart items"));

	action = toolbar->addAction(QIcon(":/icons/zoomOut.png"), tr("zoom\nout"),
			this, SLOT(zoomOut()));
	action->setToolTip(tr("shrink flowchart items"));

	action = toolbar->addAction(QIcon(":/icons/zoomFit.png"), tr("zoom\nfit"),
			this, SLOT(zoomFit()));
	action->setToolTip(tr("fit flowchart in view"));

	toolbar->addSeparator();
	action = toolbar->addAction(QIcon(":/icons/revert.png"), tr(
			"reset\nselected"), inspector, SLOT(delParam()));
	action->setToolTip(tr("reset selected parameter(s)\nto their defaults"));

	action = toolbar->addAction(QIcon(":/icons/revert_all.png"), tr(
			"clear\nflowchart"), inspector, SLOT(clear()));
	action->setToolTip(tr("empty flowchart and inspector"));

	toolbar->addSeparator();
	action = toolbar->addAction(QIcon(":/icons/intro.png"), tr(
			"introduction\nto paramedit"), docGen, SLOT(showIntro()));
	action->setToolTip(tr("show introductin page"));

	action = toolbar->addAction(QIcon(":/icons/help.png"),
			tr("paramedit\nhelp"), docGen, SLOT(showHelp()));
	action->setToolTip(tr("show help page"));

	connect(this, SIGNAL(activeGraphModelChanged(ParameterFileModel*)),
			toolbar, SLOT(setModel(ParameterFileModel*)));

	// file menu
	QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(QIcon(":/icons/new.png"), tr("&New"), this, SLOT(
			openNew()), QKeySequence(tr("Ctrl+N")));
	fileMenu->addAction(QIcon(":/icons/open.png"), tr("&Open"), this, SLOT(
			open()), QKeySequence(tr("Ctrl+O")));
	fileMenu->addAction(QIcon(":/icons/save.png"), tr("&Save"), inspector,
			SLOT(saveFile()), QKeySequence(tr("Ctrl+S")));
	fileMenu->addAction(QIcon(":/icons/save_as.png"), tr("Save as..."),
			inspector, SLOT(saveFileAs()), QKeySequence(tr("Ctrl+Shift+S")));
	_separatorAct = fileMenu->addSeparator();
	for (int i = 0; i < _maxRecentFiles; ++i) {
		_recentFileActs[i] = new QAction(this);
		_recentFileActs[i]->setVisible(false);
		connect(_recentFileActs[i], SIGNAL(triggered()),
			this, SLOT(_openRecentFile()));
		fileMenu->addAction(_recentFileActs[i]);
	}
	_updateRecentFileActions();
	fileMenu->addSeparator();
	fileMenu->addAction(QIcon(":/icons/refresh.png"), tr("&Update Plugins"),
			this, SLOT(updateMetadata()));
	fileMenu->addAction(QIcon(":/icons/runbuild.png"), tr("&Compile and load plug-in"), this, SLOT(
			compileAndLoad()));
	fileMenu->addAction(QIcon(":/icons/execute.png"), tr("Execute &Workflow"), inspector, SLOT(
			executeWorkflow()));
	fileMenu->addAction(QIcon(":/icons/export.png"), tr("Export flowchart"),
			this, SLOT(saveFlowChart()), QKeySequence(tr("Ctrl+F")));
	fileMenu->addAction(QIcon(":/icons/close.png"), tr("&Exit"), this, SLOT(
			close()), QKeySequence(tr("Ctrl+Q")));

	// edit menu
	QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(QIcon(":/icons/revert.png"), tr(
			"reset selected parameters"), inspector, SLOT(delParam()),
			QKeySequence(tr("Ctrl+R")));
	editMenu->addAction(QIcon(":/icons/revert_all.png"), tr("clear flowchart"),
			inspector, SLOT(clear()), QKeySequence(tr("Ctrl+Shift+R")));

	// view menu
	QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(QIcon(":/icons/zoomIn.png"), tr("zoom in"), this, SLOT(
			zoomIn()), QKeySequence(tr("Ctrl++")));
	viewMenu->addAction(QIcon(":/icons/zoomOut.png"), tr("zoom out"), this,
			SLOT(zoomOut()), QKeySequence(tr("Ctrl+-")));
	viewMenu->addAction(QIcon(":/icons/zoomFit.png"), tr("zoom fit"), this,
			SLOT(zoomFit()), QKeySequence(tr("Ctrl+HOME")));

	// window menu
	QMenu* windowMenu = menuBar()->addMenu(tr("&Window"));
	windowMenu->addAction(inspectorWidget->toggleViewAction());
	windowMenu->addAction(helpWidget->toggleViewAction());
	windowMenu->addAction(selectWidget->toggleViewAction());
	windowMenu->addSeparator();
	windowMenu->addAction(toolbar->toggleViewAction());

	// help menu
	QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(QIcon(":/icons/help.png"), tr("&Help"), docGen, SLOT(
			showHelp()), QKeySequence(tr("F1")));
	helpMenu->addAction(QIcon(":/icons/intro.png"), tr("&Introduction"),
			docGen, SLOT(showIntro()), QKeySequence(tr("Shift+F1")));
	helpMenu->addAction(appicon, tr("&About ParamEdit"), this, SLOT(
			_showAbout()));
	helpMenu->addAction(QIcon(":/icons/qt.png"), tr("About &Qt"), this, SLOT(
			_showAboutQt()));

	// nothing loaded yet, so editors have to be disabled
	emit enableEditors(false);

	// load config file
	QFile config(FileManager::instance().configFile());
	if (config.open(QIODevice::ReadOnly)) {
		QByteArray state = config.readAll();
		if (state.size() > 1)
			restoreState(state);
	}
	config.close();

	if (!myParent)
		showMaximized();

}

MainWindow::~MainWindow() {
}

void MainWindow::closeEvent(QCloseEvent *cEvent) {
	// save config file
	QFile config(FileManager::instance().configFile());
	if (config.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		config.write(saveState());
	}
	config.close();

	// inherited version
	QMainWindow::closeEvent(cEvent);
}

void MainWindow::_showAbout() const {
	QMessageBox aboutBox;
	aboutBox.setWindowTitle(tr("About ParamEdit"));
	aboutBox.setTextFormat(Qt::RichText);
	aboutBox.setIcon(QMessageBox::Information);
	aboutBox.setText(tr("This is <b>ParamEdit</b> by Jens-Malte Gottfried "
		"&lt;jmgottfried AT web.de&gt;"
		"<br /><br />"
		"Copyright (C) 2009 Heidelberg Collaboratory for Image Processing"
		"<br /><br />"
		"Compilation date:\t" __DATE__ "\t" __TIME__"<br />"
		SVNINFO "<br />"
		"<br />"
		"This program is part of tuchulcha."
		"<br /><br />"
		"tuchulcha is free software; you can redistribute it and/or "
		"modify it under the terms of the GNU Lesser General Public License as "
		"published by the Free Software Foundation; either version 3 of "
		"the License, or (at your option) any later version."
		"<br /><br />"
		"This program is distributed in the hope that it will be useful, "
		"but WITHOUT ANY WARRANTY; without even the implied warranty of "
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "
		"GNU Lesser General Public License for more details."
		"<br /><br />"
		"A copy of the GNU Lesser General Public License can be found at "
		"<a href=\"http://www.gnu.org/licenses/\">"
		"www.gnu.org/licenses/</a>."
		"<br /><br />"));
	aboutBox.exec();
}

void MainWindow::_showAboutQt() const {
	QMessageBox::aboutQt(0, tr("About Qt"));
}

void MainWindow::open(const QString& fileName) {
	FlowWidget* flowWidget = new FlowWidget(_centralArea);
	_centralArea->addSubWindow(flowWidget);
	connect(flowWidget, SIGNAL(statusMessage(const QString&, int)),
			statusBar(), SLOT(showMessage(const QString&, int)));
	flowWidget->showMaximized();
	flowWidget->load(fileName);
}

void MainWindow::openNew() {
	FlowWidget* flowWidget = new FlowWidget(_centralArea);
	_centralArea->addSubWindow(flowWidget);
	connect(flowWidget, SIGNAL(statusMessage(const QString&, int)),
			statusBar(), SLOT(showMessage(const QString&, int)));
	flowWidget->showMaximized();
}

void MainWindow::saveFlowChart() const {
	FlowWidget* active = qobject_cast<FlowWidget*> (
			_centralArea->activeSubWindow());
	if (active)
		active->saveFlowChart();
}

void MainWindow::_windowActivated(QMdiSubWindow* /*window*/) {
	FlowWidget* flow = qobject_cast<FlowWidget*> (
			_centralArea->currentSubWindow());

	if (_flow == flow)
		return;

	_flow = flow;

	if (flow) {
		ParameterFileModel* model = flow->model();
		emit activeGraphModelChanged(model);
		emit enableEditors(true);
	} else {
		emit activeGraphModelChanged(0);
		emit enableEditors(false);
	}
}

void MainWindow::zoomIn() {
	FlowWidget* flow = qobject_cast<FlowWidget*> (_flow);
	if (flow)
		flow->zoomIn();
}

void MainWindow::zoomOut() {
	FlowWidget* flow = qobject_cast<FlowWidget*> (_flow);
	if (flow)
		flow->zoomOut();
}

void MainWindow::zoomFit() {
	FlowWidget* flow = qobject_cast<FlowWidget*> (_flow);
	if (flow)
		flow->zoomFit();
}

void MainWindow::updateMetadata() {
	std::string logFile = FileManager::instance().configDir()
		.path().toAscii().constData();
	logFile += "/updateLog.txt";
	std::ofstream log(logFile.c_str(), std::ios::trunc);
	Q_ASSERT(log.good());
	sout.assign(log, std::cout);
	FileManager::instance().loadPluginInformation();
	FileManager::instance().updateMetadata();
	_centralArea->closeAllSubWindows();
	_selector->update();
	sout.assign();
	log.close();
}

void MainWindow::compileAndLoad() {
	try {
		_centralArea->closeAllSubWindows();
		FileManager::instance().compileAndLoad(this);
	} catch (AbstractPluginLoader::PluginException e) {
		QMessageBox msgBox;
		msgBox.setText("Error");
		msgBox.setInformativeText(QString(
				"An error occurred while trying to compile and load a plugin.\n"
					"Description of the error:\n") + e.what());
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.exec();
		return;
	}
	_selector->update();
}

void MainWindow::_openRecentFile() {
QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		open(action->data().toString());
}

void MainWindow::_updateRecentFileActions() {
	QSettings settings("Heidelberg Collaboratory for Image Processing",
					   "Tuchulcha");
	QStringList files = settings.value("recentFileList").toStringList();

	int numRecentFiles = qMin(files.size(), (int)_maxRecentFiles);

	for (int i = 0; i < numRecentFiles; ++i) {
		QString text = tr("&%1 %2").arg(i + 1).arg(_strippedName(files[i]));
		_recentFileActs[i]->setText(text);
		_recentFileActs[i]->setData(files[i]);
		_recentFileActs[i]->setVisible(true);
	}
	for (int j = numRecentFiles; j < _maxRecentFiles; ++j)
		_recentFileActs[j]->setVisible(false);

	_separatorAct->setVisible(numRecentFiles > 0);
}

QString MainWindow::_strippedName(const QString& fullFileName) const {
	return QFileInfo(fullFileName).fileName();
}

void MainWindow::setCurrentFile(const QString& fileName) {
	if (fileName.isEmpty())
		return;

	QSettings settings("Heidelberg Collaboratory for Image Processing",
					   "Tuchulcha");
	QStringList files = settings.value("recentFileList").toStringList();
	files.removeAll(fileName);
	files.prepend(fileName);
	while (files.size() > _maxRecentFiles)
		files.removeLast();

	settings.setValue("recentFileList", files);

	_updateRecentFileActions();
}
