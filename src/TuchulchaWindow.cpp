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

#include <QDockWidget>
#include <QTextBrowser>
#include <QSettings>
#include <QStatusBar>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QApplication>
#include "DocGenerator.h"

#include "WorkflowComments.h"

#include "TuchulchaWindow.h"
#include "ObjectInspector.h"
#include "FlowWidget.h"
#include "NodeTreeView.h"
#include "GraphModel.h"
#include "ModelToolBar.h"
#include "FileManager.h"
#include "LogDialog.h"
#include "LogDecorators.h"
#include "OptionsDialog.h"
#include "RecentFileHandler.h"

#include <charon-core/configVersion.h>

	/// DEFAULT_DEBUG_SUFFIX default lib suffix
#ifdef NDEBUG
	#define DEFAULT_DEBUG_SUFFIX false
#else
	#define DEFAULT_DEBUG_SUFFIX true
#endif

const int TuchulchaWindow::_saveStateVersion = 1;

TuchulchaWindow::TuchulchaWindow(QWidget* myParent) :
	QMainWindow(myParent),
	_toolBar(0), _flow(0),_docGen(0),_helpDisp(0)
{
	if (OptionsDialog::check()) {
		options();
	}

	QSettings settings;

	setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
	setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
	setDockNestingEnabled(true);

	// simple application icon
	QPixmap appicon(":/icons/appicon.png");
	setWindowIcon(appicon);

	// Application Title
	setWindowTitle("Tuchulcha");

	// object inspector
	QDockWidget* inspectorTools = new QDockWidget(
			tr("ObjectInspector Tools"), this);
	QDockWidget* inspectorWidget = new QDockWidget(
			tr("ObjectInspector"), inspectorTools);
	inspectorWidget->setObjectName("inspectorwidget");
	inspectorTools->setObjectName("inspectortools");
	_inspector = new ObjectInspector(inspectorWidget);
	inspectorTools->setWidget(_inspector);
	inspectorWidget->setWidget(_inspector->getViewer());

	// help viewer
	QDockWidget* helpWidget = new QDockWidget(tr("Help Browser"), this);
	helpWidget->setObjectName("helpwidget");
	QTextBrowser* helpBrowser = new QTextBrowser(helpWidget);
	helpWidget->setWidget(helpBrowser);
	_docGen = new DocGenerator(helpBrowser, this);
	_docGen->showIntro();
	connect(this, SIGNAL(metaDataUpdated()), _docGen, SLOT(updateMetaData()));

	// workflow comments
	QDockWidget* commentBox = new QDockWidget(tr("Workflow Comments"), this);
	commentBox -> setObjectName("RTFM Box");
	WorkflowComments* commentWidget = new WorkflowComments(commentBox);
	commentBox -> setWidget(commentWidget);

	// select widget
	QDockWidget* selectWidget = new QDockWidget(tr("Module Collection"), this);
	selectWidget->setObjectName("selectwidget");
	_selector = new NodeTreeView(selectWidget);
	selectWidget->setWidget(_selector);
	connect(this, SIGNAL(metaDataUpdated()), _selector, SLOT(reload()));

	// help browser connections
	connect(_selector, SIGNAL(showClassDoc(QString)), _docGen, SLOT(
			showClassDoc(QString)));

	// object inspector connections
	connect(_inspector, SIGNAL(statusMessage(const QString&, int)),
			statusBar(), SLOT(showMessage(const QString&, int)));
	connect(this, SIGNAL(activeGraphModelChanged(ParameterFileModel*)),
			_inspector, SLOT(setModel(ParameterFileModel*)));

	// comment widget connections
	connect(_inspector,
			SIGNAL(modelChanged(ParameterFileModel*)),
			commentWidget,
			SLOT(update(ParameterFileModel*)));

	// add widgets to dock area
	addDockWidget(Qt::RightDockWidgetArea, helpWidget);
	addDockWidget(Qt::BottomDockWidgetArea, selectWidget);
	addDockWidget(Qt::BottomDockWidgetArea, inspectorWidget);
	addDockWidget(Qt::BottomDockWidgetArea, inspectorTools);
	addDockWidget(Qt::BottomDockWidgetArea, commentBox);
	_centralArea = new CentralMdiArea(this);
	_centralArea->setViewMode(QMdiArea::TabbedView);
	_centralArea->setDocumentMode(true);
	_centralArea->setTabsMovable(true);
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
	_centralArea->setTabsClosable(true);
#endif
	setCentralWidget(_centralArea);

	connect(_centralArea, SIGNAL(subWindowActivated (QMdiSubWindow*)),
			this, SLOT(_windowActivated(QMdiSubWindow*)));
	connect(_centralArea, SIGNAL(filesDropped(QStringList)),
			this, SLOT(open(QStringList)));

	// toolbar
	_toolBar = new ModelToolBar(tr("toolbar"));
	_toolBar->setObjectName("toolbar");
	addToolBar(_toolBar);
	_toolBar->setToolButtonStyle((Qt::ToolButtonStyle)
		settings.value("toolButtonStyle",Qt::ToolButtonFollowStyle).toInt());
	_toolBar->setIconSize(QSize(32, 32));
	QAction* action;

	connect(_selector, SIGNAL(addNode(QString)), // handle double clicks
		_toolBar, SLOT(addNode(QString)));    // on selector widget

	action = _toolBar->addAction(
		QIcon::fromTheme("document-new",QIcon(":/icons/document-new.png")),
		tr("new\nfile"), this, SLOT(openNew()));
	action->setToolTip(tr("create a new file"));

	action = _toolBar->addAction(
		QIcon::fromTheme("document-open",QIcon(":/icons/document-open.png")),
		tr("open\nfile"),this, SLOT(open()));
	action->setToolTip(tr("open an existing file"));

	action = _toolBar->addAction(
		QIcon::fromTheme("document-save",QIcon(":/icons/document-save.png")),
		tr("save\nfile"), _inspector, SLOT(saveFile()));
	action->setToolTip(tr("save current document"));

	action = _toolBar->addAction(
		QIcon::fromTheme("document-save-as",QIcon(":/icons/document-save-as.png")),
		tr("save\nfile as"), _inspector, SLOT(saveFileAs()));
	action->setToolTip(tr("save current document to a new location"));

	action = _toolBar->addAction(
		QIcon::fromTheme("document-export",QIcon(":/icons/document-export.png")),
		tr("export\nflowchart"), this, SLOT(saveFlowChart()));
	action->setToolTip(tr("export flowchart to an image file"));

	_toolBar->addSeparator();
	action = _toolBar->addAction(
		QIcon::fromTheme("view-refresh",QIcon(":/icons/view-refresh.png")),
		tr("&Update\nPlugins"), this, SLOT(updateMetadata()));
	action->setToolTip(
		tr("update classes informations reading all plugins"));

	action = _toolBar->addAction(
		QIcon::fromTheme("media-playback-start",QIcon(":/icons/execute.png")),
		tr("Execute\n&Workflow"), this, SLOT(runWorkflow()));
	action->setToolTip(
		tr("execute workflow that is shown in the current window"));

	_toolBar->addSeparator();
	qobject_cast<ModelToolBar*>(_toolBar)->addModelActions();
	_toolBar->addSeparator();
	action = _toolBar->addAction(
		QIcon::fromTheme("zoom-in",QIcon(":/icons/zoom-in.png")),
		tr("zoom\nin"), this, SLOT(zoomIn()));
	action->setToolTip(tr("enlarge flowchart items"));

	action = _toolBar->addAction(
		QIcon::fromTheme("zoom-out",QIcon(":/icons/zoom-out.png")),
		tr("zoom\nout"), this, SLOT(zoomOut()));
	action->setToolTip(tr("shrink flowchart items"));

	action = _toolBar->addAction(
		QIcon::fromTheme("zoom-fit-best",QIcon(":/icons/zoom-fit-best.png")),
		tr("zoom\nfit"), this, SLOT(zoomFit()));
	action->setToolTip(tr("fit flowchart in view"));

	_toolBar->addSeparator();
	action = _toolBar->addAction(
		QIcon::fromTheme("help-faq",QIcon(":/icons/help-info.png")),
		tr("intro"), _docGen, SLOT(showIntro()));
	action->setToolTip(tr("show introductin page"));

	action = _toolBar->addAction(
		QIcon::fromTheme("help-contents",QIcon(":/icons/help-contents.png")),
		tr("help"), this, SLOT(_showHelp()));
	action->setToolTip(tr("show help page"));

	connect(this, SIGNAL(activeGraphModelChanged(ParameterFileModel*)),
			_toolBar, SLOT(setModel(ParameterFileModel*)));

	// file menu
	QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(
		QIcon::fromTheme("document-new",QIcon(":/icons/document-new.png")),
		tr("&New"), this, SLOT(openNew()),
		QKeySequence::New);
	fileMenu->addAction(
		QIcon::fromTheme("document-open",QIcon(":/icons/document-open.png")),
 		tr("&Open"), this, SLOT(open()),
		QKeySequence::Open);
	fileMenu->addAction(
		QIcon::fromTheme("document-save",QIcon(":/icons/document-save.png")),
		tr("&Save"), _inspector, SLOT(saveFile()),
		QKeySequence::Save);
	fileMenu->addAction(
		QIcon::fromTheme("document-save-as",QIcon(":/icons/document-save-as.png")),
		tr("Save &as..."), _inspector, SLOT(saveFileAs()),
		QKeySequence::SaveAs);
	_rfHandler = new RecentFileHandler(this);
	_rfHandler->registerRecentFileEntries(fileMenu);
	connect(_rfHandler,SIGNAL(openFile(QString)),SLOT(open(QString)));
	fileMenu->addSeparator();
	action = fileMenu->addAction(
		QIcon::fromTheme("view-refresh",QIcon(":/icons/view-refresh.png")),
		tr("&Update Plugins"),this, SLOT(updateMetadata()));
	action->setShortcuts(QKeySequence::Refresh);
	action = fileMenu->addAction(
		QIcon::fromTheme("media-playback-start",QIcon(":/icons/execute.png")),
		tr("Execute &Workflow"),this, SLOT(runWorkflow()));
	action->setShortcuts(QKeySequence::FindNext);
	fileMenu->addAction(
		QIcon::fromTheme("document-export",QIcon(":/icons/document-export.png")),
		tr("Export &flowchart"),this,SLOT(saveFlowChart()),
		QKeySequence::Find);
	fileMenu->addAction(
		QIcon::fromTheme("application-exit",QIcon(":/icons/application-exit.png")),
		tr("&Exit"), this, SLOT(close()),
		QKeySequence::Quit);

	// edit menu
	QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(
		QIcon::fromTheme("document-revert",QIcon(":/icons/revert.png")),
		tr("&reset selected parameters"), _inspector, SLOT(delParam()),
		QKeySequence::DeleteStartOfWord);
	editMenu->addAction(
		QIcon::fromTheme("configure",QIcon(":/icons/configure.png")),
		tr("&Options"), this, SLOT(options()),
		QKeySequence::Preferences);

	// view menu
	QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(
		QIcon::fromTheme("zoom-in",QIcon(":/icons/zoom-in.png")),
		tr("zoom &in"), this, SLOT(zoomIn()),
		QKeySequence::ZoomIn);
	viewMenu->addAction(
		QIcon::fromTheme("zoom-out",QIcon(":/icons/zoom-out.png")),
		tr("zoom &out"), this, SLOT(zoomOut()),
		QKeySequence::ZoomOut);
	viewMenu->addAction(
		QIcon::fromTheme("zoom-fit-best",QIcon(":/icons/zoom-fit.png")),
		tr("zoom &fit"), this, SLOT(zoomFit()),
		QKeySequence(tr("Ctrl+0")));
	viewMenu->addAction(
		QIcon::fromTheme("view-split-left-right",
			QIcon(":/icons/view-split-left-right.png")),
		tr("&tile windows"), _centralArea, SLOT(tileSubWindows()),
		QKeySequence::Undo);
	viewMenu->addAction(
		QIcon::fromTheme("window-duplicate",
			QIcon(":/icons/window-duplicate.png")),
		tr("&cascade windows"), _centralArea, SLOT(cascadeSubWindows()),
		QKeySequence::Redo);

	// window menu
	QMenu* windowMenu = menuBar()->addMenu(tr("&Window"));
	windowMenu->addAction(inspectorWidget->toggleViewAction());
	windowMenu->addAction(inspectorTools->toggleViewAction());
	windowMenu->addAction(helpWidget->toggleViewAction());
	windowMenu->addAction(selectWidget->toggleViewAction());
	windowMenu->addAction(commentBox->toggleViewAction());
	windowMenu->addSeparator();
	windowMenu->addAction(_toolBar->toggleViewAction());

	// help menu
	QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(
		QIcon::fromTheme("help-contents",QIcon(":/icons/help-contents.png")),
		tr("&Help"), this, SLOT(_showHelp()), QKeySequence::HelpContents);
	helpMenu->addAction(
		QIcon::fromTheme("help-faq",QIcon(":/icons/help-info.png")),
		tr("&Introduction"), _docGen, SLOT(showIntro()),
		QKeySequence::WhatsThis);
	helpMenu->addAction(
		QIcon::fromTheme("help-about",appicon),
		tr("&About Tuchulcha"), this, SLOT(_showAbout()));
	helpMenu->addAction(
		QIcon(":/icons/qt.png"),
		tr("About &Qt"), this, SLOT(_showAboutQt()));

	// load window state config
	QSettings defaultS(":/config/default.ini",QSettings::IniFormat);
	restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
	restoreState(
			defaultS.value("MainWindow/windowState").toByteArray(),
			_saveStateVersion);
	restoreState(
			settings.value("MainWindow/windowState").toByteArray(),
			_saveStateVersion);

	if(settings.value("reloadOnStartup",true).toBool())
		updateMetadata();
}

TuchulchaWindow::~TuchulchaWindow() {
	if (_helpDisp) {
		_helpDisp->terminate();
		_helpDisp->waitForFinished(1e3);
	}
}

void TuchulchaWindow::closeEvent(QCloseEvent *cEvent) {
	// save window state config
	QSettings settings;
	settings.beginGroup("MainWindow");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("windowState", saveState(_saveStateVersion));
	settings.endGroup();

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
	tr("MSVC") + " " +tr("%1 (%2bit, %3)").arg(_MSC_VER)
		.arg(sizeof(void*)*8).arg(CMAKE_INTDIR)+"\n";
#elif defined (__GNUC__)
	tr("GCC") + " " +tr("%1 (%2bit, %3)")
			.arg(QString("%1.%2.%3")
				.arg(__GNUC__)
				.arg(__GNUC_MINOR__)
				.arg(__GNUC_PATCHLEVEL__))
			.arg(sizeof(void*)*8)
	#ifdef QT_DEBUG
			.arg("Debug");
	#else
			.arg("Release");
	#endif
#else
	+"\n"+tr("unknown compiler (%1bit)").arg(sizeof(*void)*8)+"\n";
#endif

	aboutBox.setText(
		tr("This is <b>Tuchulcha %1</b><br />written by %2 and others")
		.arg(TUCHULCHA_VERSION)
		.arg("<a href=\"mailto:jmgottfriedATweb.de\">Jens-Malte Gottfried</a>")
		+QString("<br /><br />")
		+tr("Copyright &copy; %1 Heidelberg Collaboratory for Image Processing")
		.arg("2009-2013")
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

void TuchulchaWindow::_showHelp(QString page) {
#ifdef USE_ASSISTANT
	static QString collectionPath;
	if (collectionPath.isNull()) {
		// look for collection file
		QStringList hPaths;
		hPaths << QCoreApplication::applicationDirPath();
#ifdef TUCHULCHA_QHC_DIR
		hPaths << TUCHULCHA_QHC_DIR;
#endif
		foreach (QString path, hPaths) {
			if (QDir(path).exists("tuchulcha.qhc")) {
				collectionPath = path;
				break;
			}
		}
		if (collectionPath.isEmpty()) {
			// fallback
			_docGen->showHelp();
			return;
		}
	}
	if (_helpDisp && _helpDisp->state() != QProcess::Running) {
		delete _helpDisp;
	}
	if (_helpDisp.isNull()) {
		_helpDisp = new QProcess(this);
		_helpDisp->setProcessChannelMode(QProcess::ForwardedChannels);
		_helpDisp->connect(_helpDisp,
				SIGNAL(finished(int,QProcess::ExitStatus)),
				SLOT(deleteLater()));
		_helpDisp->connect(_helpDisp,
				SIGNAL(error(QProcess::ProcessError)),
				SLOT(deleteLater()));
		QStringList args;
		args << QLatin1String("-collectionFile")
			<< QLatin1String("tuchulcha.qhc")
			<< QLatin1String("-enableRemoteControl");
		_helpDisp->setWorkingDirectory(collectionPath);
		_helpDisp->start(QLatin1String("assistant"), args);
		if (!_helpDisp->waitForStarted()) {
			// fallback
			_docGen->showHelp();
			return;
		}
		_helpDisp->waitForReadyRead(150); // no output, equivalent to sleep(150)
		QByteArray pout;
		pout.append("show contents;expandToc 1;\n");
		_helpDisp->write(pout);
		_helpDisp->waitForReadyRead(150);
	}
	if (page.isEmpty()) {
		page = "tuchulcha-usage.html";
	}
	QByteArray pout;
	pout.append(QString("setSource qthelp://org.doxygen.tuchulcha/doc/%1;syncContents;\n").arg(page));
	_helpDisp->write(pout);
#else
	Q_UNUSED(page);
	_docGen->showHelp();
#endif
}

void TuchulchaWindow::open(const QString& fileName, bool maximized) {
	GraphModel* model = new GraphModel(
				QString(), this, FileManager::instance().classesFile());
	connect(model, SIGNAL(fileNameChanged (QString)),
		_rfHandler, SLOT(setCurrentFile(QString)));
	connect(model, SIGNAL(statusMessage(QString)),
			SLOT(showMessage(QString)));
	if (model->load(fileName)) {
		FlowWidget* flowWidget = new FlowWidget(model, _centralArea);
		_centralArea->addSubWindow(flowWidget);
		connect(flowWidget, SIGNAL(statusMessage(QString)),
				SLOT(showMessage(QString)));
		connect(flowWidget,SIGNAL(destroyed()), model, SLOT(deleteLater()));
		if (maximized) {
			flowWidget->showMaximized();
		} else {
			flowWidget->showNormal();
		}
	}
	else {
		model->deleteLater();
	}
}

void TuchulchaWindow::openNew() {
	GraphModel* model = new GraphModel(
				QString(), this, FileManager::instance().classesFile());
	FlowWidget* flowWidget = new FlowWidget(model, _centralArea);
	_centralArea->addSubWindow(flowWidget);
	connect(flowWidget,SIGNAL(destroyed()), model, SLOT(deleteLater()));
	connect(model, SIGNAL(fileNameChanged (QString)),
			_rfHandler, SLOT(setCurrentFile(QString)));
	connect(model, SIGNAL(statusMessage(QString)),
			SLOT(showMessage(QString)));
	connect(flowWidget, SIGNAL(statusMessage(QString)),
			SLOT(showMessage(QString)));
	connect(flowWidget, SIGNAL(nodeTypeSelected(QString)),
			_docGen, SLOT(showClassDoc(QString)));
	flowWidget->showMaximized();
}

void TuchulchaWindow::showMessage(QString msg) const {
	statusBar()->showMessage(msg, 5000);
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
	} else {
		emit activeGraphModelChanged(0);
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
	QString fName = QString::null;
	if(_flow) {
		fName = _flow->model()->fileName();
	}

	// close first, window will ask for saving if needed
	_centralArea->closeAllSubWindows();
	LogDialog dialog(new LogDecorators::Update);
	connect(&dialog, SIGNAL(helpRequested(QString)), SLOT(_showHelp(QString)));
	dialog.exec();
	emit metaDataUpdated();

	// restore recent workflow, if any
	if (!fName.isEmpty()) {
		QCoreApplication::processEvents();
		open(fName);
	}
}

void TuchulchaWindow::runWorkflow() {
	if (!_flow)
		return;
	_inspector->saveFile();
	LogDecorators::RunWorkflow* dec =
		new LogDecorators::RunWorkflow(_flow->model()->fileName());
	ParameterFileModel* model = _inspector->model();
	QString oldPref = model->prefix();
	connect(
		dec,SIGNAL(highlightObject(QString)),
		model,SLOT(setPrefix(QString)));
	LogDialog dialog(dec);
	connect(&dialog, SIGNAL(helpRequested(QString)), SLOT(_showHelp(QString)));
	dialog.exec();
	model->setPrefix(oldPref);
}

void TuchulchaWindow::options() {
	OptionsDialog dialog(isVisible()?this:0);
	connect(&dialog,SIGNAL(helpRequested(QString)),
			SLOT(_showHelp(QString)));
	dialog.exec();
	QSettings settings;
	_toolBar->setToolButtonStyle((Qt::ToolButtonStyle)
		settings.value("toolButtonStyle",Qt::ToolButtonFollowStyle).toInt());
}

void TuchulchaWindow::open(const QStringList& files) {
	if (files.isEmpty()) {
		return;
	}
	if (files.size() == 1) {
		open(files.at(0));
	}
	else {
		for (int a = 0; a < files.size(); a++) {
			open(files.at(a), false);
		}
	}
}
