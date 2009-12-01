/* Copyright (C) 2009 Robert Weidel

 This file is part of Charon.

 Charon is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Charon is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
 */
/**	@file MainWindow.h
 *	@brief Declaration of class MainWindow
 *
 *	@date	01.12.2009
 *	@author Robert Weidel
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui>

/// template generator main window
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	/// default constructor
	MainWindow(QWidget* parent = 0 /** [in] parent widget*/);
	~MainWindow();

protected:
	/// handle close events
	/** This saves the window geometry and exits.
	 *  A dialog "user really wants to exit" could be implemented here.
	 *  \param event		close event to handle
	 */
	virtual void closeEvent(QCloseEvent* event);

private:
	QLineEdit* _inputName;		///< plugin name
	QLineEdit* _inputDir;		///< file output directory
	QLineEdit* _inputAuthorName;	///< author's name
	QTextEdit* _pluginDoc;		///< documentation of the generated plugin
	QTableWidget* _table1;		///< table layout for Input/Output definition
	QTableWidget* _table2;		///< table layout for parameter definition
	QCheckBox* _check1;		///< check if it's an templated plugin


private slots:
	void _showHello();
	/// constructs new files an writes chosen parameters/slots
	/** This replaces @...@ by ...
	 */
	void _save();
	void _selectOutputDir();	///< browse to output directory
	/// restore window geometry
	void _readSettings();
	/// save window geometry
	void _writeSettings();
	/// adds a row with specific cell elements to table1
	void _addslot();
	/// removes last row from table1
	void _removeslot();
	/// adds a row with specific cell elements to table2
	void _addparameter();
	/// removes last row from table2
	void _removeparameter();
};

#endif // MAINWINDOW_H
