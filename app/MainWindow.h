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
 *	@date	13.11.2009
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
	QLineEdit* _inputName;	///< plugin name
	QLineEdit* _inputDir;	///< file output directory
	QCheckBox* _check1;
	QCheckBox* _check2;
	QCheckBox* _check3;
	QCheckBox* _check4;
	QCheckBox* _check5;
	QCheckBox* _check6;
	QCheckBox* _check7;

private slots:
	void _showHello();
	void _save();
	void _selectOutputDir();
	/// restore window geometry
	void _readSettings();
	/// save window geometry
	void _writeSettings();
};

#endif // MAINWINDOW_H
