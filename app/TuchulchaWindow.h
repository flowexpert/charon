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

class QMdiArea;
class QMdiSubWindow;
class FlowWidget;
class ParameterFileModel;
class NodeTreeView;
class QAction;

///	Management of the application's main window.
class TuchulchaWindow : public QMainWindow {
	Q_OBJECT

public:
	/**	Default constructor.
	 *	\param parent	pointer to parent widget
	 */
	TuchulchaWindow(QWidget* parent = 0);
	virtual ~TuchulchaWindow();

public slots:
	/// open parameterfile and display flowchart content
	/// \param fileName File to open.
	///                 Shows file open dialog if empty.
	void open(const QString& fileName = "");

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

	/// insert file as current file to recent file list
	void setCurrentFile(const QString& fileName);

signals:
	/// inform about changed graph to edit
	/// \param model	new model to edit, 0 if no model exists
	void activeGraphModelChanged(ParameterFileModel* model);

	/// enable/disable edit widgets
	/// \param enable	if to enable widgets or not
	void enableEditors(bool enable);

	/// meta data has been updated, plugins reloaded
	void metaDataUpdated() ;

protected:
	/// Close event handler.
	/// Save window state and close the window.
	/// \param event	close event
	virtual void closeEvent(QCloseEvent* event);

private slots:
	/// show about dialog
	void _showAbout();

	/// show about Qt dialog
	void _showAboutQt();

	/// edit options
	void _options();

	/// update windows when new flowchart has been selected
	/// \param window	activated window
	void _windowActivated(QMdiSubWindow* window);

	/// open recent file
	void _openRecentFile();

	/// updated the list of recent files
	void _updateRecentFileActions();

private:
	/// Central widget to manage multiple windows
	QMdiArea* _centralArea;
	/// current flow widget
	FlowWidget* _flow;
	/// selector widget
	NodeTreeView* _selector;

	/// separator above recent files
	QAction* _separatorAct;
	/// maximal number of recent files
	enum { _maxRecentFiles = 5 };
	/// actions to open recent files
	QAction* _recentFileActs[_maxRecentFiles];

	/// strip file name
	/** \param fullFileName    filenane to strip
	 *  \returns               stripped name
	 */
	QString _strippedName(const QString& fullFileName) const;
};

#endif /* MAINWINDOW_H_ */
