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
/** \file   TuchulchaWindow.h
 *  \brief  Declaration of class MainWindow
 *
 *  \date   27.08.2008
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QMainWindow>
#include <QPointer>
#include "CentralMdiArea.h"

class QMdiSubWindow;
class FlowWidget;
class ObjectInspector;
class ParameterFileModel;
class NodeTreeView;
class QAction;
class RecentFileHandler;
class DocGenerator;
class QProcess;
class GraphModel;

/// Management of the application's main window.
class TuchulchaWindow : public QMainWindow {
	Q_OBJECT

public:
	/// Default constructor.
	/** \param parent	pointer to parent widget
	 */
	TuchulchaWindow(QWidget* parent = 0);
	virtual ~TuchulchaWindow();

public slots:
	/// open parameterfile and display flowchart content
	/** \param fileName   File to open.
	 *                    Shows file open dialog if empty. */
	void open(const QString& fileName = QString());

	/// open parameterfiles
	/// \param files      Files to open
	void open(const QStringList& files);

	/// create new empty flowchart
	void openNew();

	/// save current flowchart
	void saveFlowChart() const;

	/// zoom in
	void zoomIn();

	/// zoom out
	void zoomOut();

	/// fit in view
	void zoomFit();

	/// update plugin information
	void updateMetadata();

	/// runn current workflow
	void runWorkflow();

	/// edit options
	void options();

	/// print status message
	void showMessage(QString msg) const;

signals:
	/// inform about changed graph to edit
	/// \param model    new model to edit, 0 if no model exists
	void activeGraphModelChanged(ParameterFileModel* model);

	/// meta data has been updated, plugins reloaded
	void metaDataUpdated();

	/// status message
	void statusMessage(const QString&) const;

protected:
	/// Close event handler.
	/// Save window state and close the window.
	/// \param event    close event
	virtual void closeEvent(QCloseEvent* event);

private slots:
	/// show about dialog
	void _showAbout();

	/// show help browser
	/** \param page     requested help page */
	void _showHelp(QString page = QString());

	/// show about Qt dialog
	void _showAboutQt();

	/// update links when new flowchart has been selected
	/** this sets the _flow pointer to the currently selected widget,
	 *  which is needed to get the zoom and model buttons working
	 *
	 *  this also emits the activeGraphModeChanged signal which
	 *  sets up the connections of the object inspector and the model tool bar
	 */
	void _updateFlowLinks();

private:
	/// tool bar
	QToolBar* _toolBar;
	/// Central widget to manage multiple windows
	CentralMdiArea* _centralArea;
	/// recent file handler
	RecentFileHandler* _rfHandler;
	/// current flow widget
	FlowWidget* _flow;
	/// object inspector
	ObjectInspector* _inspector;
	/// selector widget
	NodeTreeView* _selector;
	/// Documentation Generator
	DocGenerator* _docGen;
	/// window state version identifier
	static const int _saveStateVersion;
	/// help display process
	QPointer<QProcess> _helpDisp;
	/// setup model and view connections (for open and new)
	/** \param model    workflow model
	 *  \param fWidget  flow widget
	 */
	void _setupConnections(GraphModel* model, FlowWidget* fWidget);
};

#endif /* MAINWINDOW_H_ */
