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
/**	@file MainWindow.h
 *	@brief Declaration of class MainWindow
 *
 *	@date	27.08.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QMainWindow>

class QMdiArea;
class QMdiSubWindow;
class FlowWidget;
class ParameterFileModel;

///	Management of the application's main window.
class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	/**	Default constructor.
	 *	@param parent	pointer to parent widget
	 */
	MainWindow(QWidget* parent = 0);
	virtual ~MainWindow();

public slots:
	/// open parameterfile and display flowchart content
	void open();

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

	///update plugin information
	void updateMetadata();
	
signals:
	/// inform about changed graph to edit
	/// @param model	new model to edit, 0 if no model exists
	void activeGraphModelChanged(ParameterFileModel* model);
	
	/// enable/disable edit widgets
	/// @param enable	if to enable widgets or not
	void enableEditors(bool enable);

protected:
	/// Close event handler.
	/// Save window state and close the window.
	/// @param event	close event
	virtual void closeEvent(QCloseEvent* event);

private slots:
	/// show about dialog
	void _showAbout() const;

	/// show about Qt dialog
	void _showAboutQt() const;

	///	update windows when new flowchart has been selected
	///	@param window	activated window
	void _windowActivated(QMdiSubWindow* window);

private:
	/// Central widget to manage multiple windows
	QMdiArea* _centralArea;
	/// current flow widget
	FlowWidget* _flow;
};

#endif /* MAINWINDOW_H_ */
