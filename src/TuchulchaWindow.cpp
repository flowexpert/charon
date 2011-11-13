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
/** \file TuchulchaWindow.cpp
 *  \brief Implementation of class MainWindow
 *
 *  \date 27.08.2008
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include <QtGui>
#include <QTextBrowser>
#include "DocGenerator.h"

#include "TuchulchaWindow.h"
#include "ObjectInspector.h"
#include "FlowWidget.h"
#include "NodeTreeView.h"
#include "GraphModel.h"
#include "ModelToolBar.h"
#include "FileManager.h"
#include <PluginManager.h>
#include "AbstractPluginLoader.h"
#include "WorkflowExecutor.h"

#include "ui_OptionsDialog.h"

#include "TuchulchaWindow.moc"

TuchulchaWindow::TuchulchaWindow(QWidget* myParent) :
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
	setWindowTitle("Tuchulcha");

	// object inspector
	QDockWidget* inspectorWidget = new QDockWidget(
			tr("ObjectInspector"), this);
	inspectorWidget->setObjectName("inspectorwidget");
	ObjectInspector* inspector = new ObjectInspector(inspectorWidget);
	inspectorWidget->setWidget(inspector);

	// help viewer
	QDockWidget* helpWidget = new QDockWidget(tr("Help Browser"), this);
	helpWidget->setObjectName("helpwidget");
	QTextBrowser* helpBrowser = new QTextBrowser(helpWidget);
	helpWidget->setWidget(helpBrowser);
	DocGenerator* docGen = new DocGenerator(helpBrowser, this);
	docGen->showIntro();
	connect(this, SIGNAL(metaDataUpdated()), docGen, SLOT(updateMetaData()));

	// select widget
	QDockWidget* selectWidget = new QDockWidget(tr("Selector"), this);
	selectWidget->setObjectName("selectwidget");
	_selector = new NodeTreeView(selectWidget);
	selectWidget->setWidget(_selector);
	connect(this, SIGNAL(metaDataUpdated()), _selector, SLOT(reload())) ;

	// help browser connections
	connect(_selector, SIGNAL(showClassDoc(QString)), docGen, SLOT(
			showClassDoc(QString)));

	// object inspector connections
	connect(inspector, SIGNAL(statusMessage(const QString&, int)),
			statusBar(), SLOT(showMessage(const QString&, int)));
	connect(this, SIGNAL(activeGraphModelChanged(ParameterFileModel*)),
			inspector, SLOT(setModel(ParameterFileModel*)));
	connect(this, SIGNAL(enableEditors(bool)),
			inspector, SLOT(setEnabled(bool)));

	// add widgets to dock area
	addDockWidget(Qt::RightDockWidgetArea, inspectorWidget);
	addDockWidget(Qt::BottomDockWidgetArea, selectWidget, Qt::Vertical);
	addDockWidget(Qt::BottomDockWidgetArea, helpWidget, Qt::Vertical);
	_centralArea = new QMdiArea(this);
	setCentralWidget(_centralArea);

	connect(_centralArea, SIGNAL(subWindowActivated (QMdiSubWindow*)),
			this, SLOT(_windowActivated(QMdiSubWindow*)));

	WorkflowExecutor* executor = new WorkflowExecutor(inspector, this);

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

	action = toolbar->addAction(QIcon(":/icons/save_as.png"),
		tr("save\nfile as"), inspector, SLOT(saveFileAs()));
	action->setToolTip(tr("save current document\nto a new location"));

	action = toolbar->addAction(QIcon(":/icons/export.png"),
		tr("export\nflowchart"), this, SLOT(saveFlowChart()));
	action->setToolTip(tr("export flowchart to an image file"));

	toolbar->addSeparator();
	action = toolbar->addAction(QIcon(":/icons/refresh.png"),
		tr("&Update Plugins"), this, SLOT(updateMetadata()));
	action->setToolTip(
			tr("update classes informations reading all plugins"));
	toolbar->addAction(executor->runAction());
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
	action = toolbar->addAction(QIcon(":/icons/revert.png"),
		tr("reset\nselected"), inspector, SLOT(delParam()));
	action->setToolTip(tr("reset selected parameter(s)\nto their defaults"));

	toolbar->addSeparator();

	action = toolbar->addAction(QIcon(":/icons/intro.png"), tr(
			"introduction\nto tuchulcha"), docGen, SLOT(showIntro()));
	action->setToolTip(tr("show introductin page"));

	action = toolbar->addAction(QIcon(":/icons/help.png"),
			tr("tuchulcha\nhelp"), docGen, SLOT(showHelp()));
	action->setToolTip(tr("show help page"));

	connect(this, SIGNAL(activeGraphModelChanged(ParameterFileModel*)),
			toolbar, SLOT(setModel(ParameterFileModel*)));

	// file menu
	QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(QIcon(":/icons/new.png"), tr("&New"), this,
		SLOT(openNew()), QKeySequence(tr("Ctrl+N")));
	fileMenu->addAction(QIcon(":/icons/open.png"), tr("&Open"), this,
		SLOT(open()), QKeySequence(tr("Ctrl+O")));
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
			this, SLOT(updateMetadata()), QKeySequence("Ctrl+U"));
	fileMenu->addAction(executor->runAction());
	fileMenu->addAction(QIcon(":/icons/export.png"), tr("Export &flowchart"),
			this, SLOT(saveFlowChart()), QKeySequence(tr("Ctrl+F")));
	fileMenu->addAction(QIcon(":/icons/close.png"), tr("&Exit"), this,
		SLOT(close()), QKeySequence(tr("Ctrl+Q")));

	// edit menu
	QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(QIcon(":/icons/revert.png"), tr(
			"&reset selected parameters"), inspector, SLOT(delParam()),
			QKeySequence(tr("Ctrl+R")));
	editMenu->addAction(QIcon(":/icons/configure.png"),tr("Options"),
			this, SLOT(_options()));

	// view menu
	QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(QIcon(":/icons/zoomIn.png"), tr("zoom in"), this,
		SLOT(zoomIn()), QKeySequence(tr("Ctrl++")));
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
	helpMenu->addAction(appicon, tr("&About Tuchulcha"), this, SLOT(
			_showAbout()));
	helpMenu->addAction(QIcon(":/icons/qt.png"), tr("About &Qt"), this, SLOT(
			_showAboutQt()));

	// nothing loaded yet, so editors have to be disabled
	emit enableEditors(false);

	// load window state config
	QSettings settings;
	if (settings.value("windowState").isValid()) {
		QByteArray state = settings.value("windowState").toByteArray();
		restoreState(state);
	}

	if (!myParent)
		showMaximized();

}

TuchulchaWindow::~TuchulchaWindow() {
}

void TuchulchaWindow::closeEvent(QCloseEvent *cEvent) {
	// save window state config
	QSettings settings;
	settings.setValue("windowState", saveState());

	_centralArea->closeAllSubWindows();

	// inherited version
	QMainWindow::closeEvent(cEvent);
}

void TuchulchaWindow::_showAbout() {
	QMessageBox aboutBox(this);
	aboutBox.setWindowTitle(tr("About Tuchulcha"));
	aboutBox.setTextFormat(Qt::RichText);
	aboutBox.setIcon(QMessageBox::Information);

	QString buildSystem =
#if defined (_MSC_VER)
	tr("MSVC %1 (%2bit, %3)\n").arg(_MSC_VER)
			.arg(sizeof(void*)*8).arg(CMAKE_INTDIR);
#elif defined (__GNUC__)
	tr("GCC %1.%2.%3 (%4bit, %5)\n")
			.arg(__GNUC__)
			.arg(__GNUC_MINOR__)
			.arg(__GNUC_PATCHLEVEL__)
			.arg(sizeof(void*)*8)
	#ifdef QT_DEBUG
			.arg("Debug");
	#else
			.arg("Release");
	#endif
#else
	tr("unknown compiler (%1bit)\n").arg(sizeof(*void)*8);
#endif

	aboutBox.setText(
		tr("This is <b>Tuchulcha %1</b><br />written by %2")
		.arg(TUCHULCHA_VERSION)
		.arg("<a href=\"mailto:jmgottfriedATweb.de\">Jens-Malte Gottfried</a>")
		+QString("<br /><br />")
		+tr("Copyright &copy; %1 Heidelberg Collaboratory for Image Processing")
		.arg("2009-2010")
		+QString("<br /><br />")
		+tr("Built %1 %2<br />with %3")
		.arg(__DATE__).arg(__TIME__).arg(buildSystem)
		+QString("<br />")
#ifdef VCSINFO
		+tr("VCS Info: %1")
		.arg(VCSINFO)
		+QString("<br />")
#endif
		+tr("Built against libraries:")
		+QString("<ul><li>charon-core %1</li>"
				"<li>Qt %3</li></ul>")
		.arg(CHARON_CORE_VERSION).arg(QT_VERSION_STR)
#ifdef BUILD_INFO
		+tr("Build info: %1").arg(BUILD_INFO)+QString("<br />")
#endif
		+QString("<br />")
		+tr("This program is part of tuchulcha.")
		+QString("<br /><br />")
		+tr("tuchulcha is free software; you can redistribute it and/or "
		"modify it under the terms of the GNU Lesser General Public License as "
		"published by the Free Software Foundation; either version 3 of "
		"the License, or (at your option) any later version.")
		+QString("<br /><br />")
		+tr("This program is distributed in the hope that it will be useful, "
		"but WITHOUT ANY WARRANTY; without even the implied warranty of "
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "
		"GNU Lesser General Public License for more details.")
		+QString("<br /><br />")
		+tr("A copy of the GNU Lesser General Public License can be found at "
		"<a href=\"http://www.gnu.org/licenses/\">"
		"www.gnu.org/licenses/</a>.")
	);
	aboutBox.exec();
}

void TuchulchaWindow::_showAboutQt() {
	QMessageBox::aboutQt(this, tr("About Qt"));
}

void TuchulchaWindow::open(const QString& fileName) {
	FlowWidget* flowWidget = new FlowWidget(_centralArea);
	_centralArea->addSubWindow(flowWidget);
	connect(flowWidget, SIGNAL(statusMessage(const QString&, int)),
			statusBar(), SLOT(showMessage(const QString&, int)));
	flowWidget->showMaximized();
	flowWidget->load(fileName);
}

void TuchulchaWindow::openNew() {
	FlowWidget* flowWidget = new FlowWidget(_centralArea);
	_centralArea->addSubWindow(flowWidget);
	connect(flowWidget, SIGNAL(statusMessage(const QString&, int)),
			statusBar(), SLOT(showMessage(const QString&, int)));
	flowWidget->showMaximized();
}

void TuchulchaWindow::saveFlowChart() const {
	FlowWidget* active = qobject_cast<FlowWidget*> (
			_centralArea->activeSubWindow());
	if (active)
		active->saveFlowChart();
}

void TuchulchaWindow::_windowActivated(QMdiSubWindow* /*window*/) {
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

void TuchulchaWindow::zoomIn() {
	FlowWidget* flow = qobject_cast<FlowWidget*> (_flow);
	if (flow)
		flow->zoomIn();
}

void TuchulchaWindow::zoomOut() {
	FlowWidget* flow = qobject_cast<FlowWidget*> (_flow);
	if (flow)
		flow->zoomOut();
}

void TuchulchaWindow::zoomFit() {
	FlowWidget* flow = qobject_cast<FlowWidget*> (_flow);
	if (flow)
		flow->zoomFit();
}

void TuchulchaWindow::updateMetadata() {
	FileManager::instance().loadPluginInformation();
	FileManager::instance().updateMetadata();
	
	_centralArea->closeAllSubWindows();
	emit metaDataUpdated() ;
}

void TuchulchaWindow::_openRecentFile() {
QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		open(action->data().toString());
}

void TuchulchaWindow::_updateRecentFileActions() {
	QSettings settings;
	QStringList files = settings.value("recentFileList").toStringList();

	int numRecentFiles = qMin(files.size(), (int)_maxRecentFiles);

	for (int i = 0; i < numRecentFiles; ++i) {
		QString text = tr("&%1 %2").arg(i + 1).arg(_strippedName(files[i]));
		_recentFileActs[i]->setText(text);
		_recentFileActs[i]->setData(files[i]);
		_recentFileActs[i]->setStatusTip(
				tr("Open recent file \"%1\"").arg(files[i]));
		_recentFileActs[i]->setVisible(true);
	}
	for (int j = numRecentFiles; j < _maxRecentFiles; ++j)
		_recentFileActs[j]->setVisible(false);

	_separatorAct->setVisible(numRecentFiles > 0);
}

QString TuchulchaWindow::_strippedName(const QString& fullFileName) const {
	return QFileInfo(fullFileName).fileName();
}

void TuchulchaWindow::setCurrentFile(const QString& fileName) {
	if (fileName.isEmpty())
		return;

	QSettings settings;
	QStringList files = settings.value("recentFileList").toStringList();
	QString fname = QFileInfo(fileName).absoluteFilePath();
	files.removeAll(fname);
	files.prepend(fname);
	while (files.size() > _maxRecentFiles)
		files.removeLast();

	settings.setValue("recentFileList", files);

	_updateRecentFileActions();
}

void TuchulchaWindow::_options() {
	QDialog dialog(this);
	Ui::OptionsDialog options;
	options.setupUi(&dialog);

	static const QString& privPathTag = FileManager::privPathTag;

	// set up dialog content
	QSettings settings;
	options.editGlobalPath->setText(
			settings.value("globalPluginPath").toString());
	options.editPrivatePath->setText(
			settings.value(privPathTag).toString());
	options.checkWait->setChecked(
			settings.value("waitAfterExecute", false).toBool());
	options.checkThreaded->setChecked(
			settings.value("executeThreaded", false).toBool());

#ifdef QT_NO_DEBUG
	// show this label in tuchulcha_d only
	options.labelPPathDesc->setVisible(false);
#endif

#if !defined(_MSC_VER) && defined(NDEBUG)
	// user may decide to load suffixed modules in unix release builds
	options.checkSuffix->setChecked(
			settings.value("suffixedPlugins", false).toBool());
#else
	// fixed in win (msvc), or if debug/release versions exist
	options.checkSuffix->setEnabled(false);
	options.checkSuffix->setChecked(DEFAULT_DEBUG_SUFFIX);
#endif

	// set new values
	if (dialog.exec() == QDialog::Accepted) {
		settings.setValue(
				"globalPluginPath",
				options.editGlobalPath->text());
		if (options.editGlobalPath->text().isEmpty()) {
			settings.remove("globalPluginPath");
		}
		settings.setValue(privPathTag, options.editPrivatePath->text());
		if (options.editPrivatePath->text().isEmpty()) {
			settings.remove(privPathTag);
		}
		settings.setValue(
				"waitAfterExecute",
				(options.checkWait->checkState() != Qt::Unchecked));
		settings.setValue(
				"executeThreaded",
				(options.checkThreaded->checkState() != Qt::Unchecked));
#if !defined(_MSC_VER) && defined(NDEBUG)
		if (options.checkSuffix->isChecked()) {
			settings.setValue("suffixedPlugins", true);
		}
		else {
			settings.remove("suffixedPlugins");
		}
#endif
	}
}
