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
	QLineEdit* _inputName;		///< plugin name
	QLineEdit* _inputDir;		///< file output directory
	QLineEdit* _inputAuthorName;	///< author's name
	QLineEdit* _inputIOName1;	///< input/output name
	QLineEdit* _inputIOName2;	///< input/output name 2
	QLineEdit* _inputIOName3;	///< input/output name 3
	QLineEdit* _inputIOName4;	///< input/output name 4
	QLineEdit* _inputIOName5;	///< input/output name 5
	QTextEdit* _inputIODocumentation; ///< documentation for I/O-slot 1
	QLineEdit* _inputIOTyp;		///< typ of I/O-slot 1
	QLineEdit* _inputParaName;	///< name of first parameter
	QTextEdit* _inputParaDocumentation; ///< documentation of first parameter
	QLineEdit* _inputParaTyp;	///< parameter typ
	QLineEdit* _inputParaDefault;	    ///< default value of first parameter
	QTableWidget* table1;		///< table layout for I/O/parameter definition
	QCheckBox* _check1;		///< check if it's an templated plugin
	QCheckBox* _check2;		///< not used
	QCheckBox* _check3;
	QCheckBox* _check4;
	QCheckBox* _check5;
	QCheckBox* _check6;
	QCheckBox* _check7;
	QComboBox* _combo1;		///< choose between Input/Output

private slots:
	void _showHello();
	void _save();			///< constructs new files an writes chosen parameters/slots
	void _selectOutputDir();	///< browse to output directory
	/// restore window geometry
	void _readSettings();
	/// save window geometry
	void _writeSettings();
};

#endif // MAINWINDOW_H
