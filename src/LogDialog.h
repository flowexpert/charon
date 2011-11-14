/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file LogDialog.h
 *  \brief Declaration of class LogDialog
 *
 *  \date 13.11.2011
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include <QDialog>

namespace Ui {class LogDialog;}
class QProcess;

/// class for logging display and communication with external processes
class LogDialog : public QDialog
{
	Q_OBJECT

public:
	/// constructor
	/** \param title   window title
	 *  \param desc    info label content
	 *  \param parent  parent widget
	 */
	explicit LogDialog(QString title="", QString desc="", QWidget* parent = 0);
	virtual ~LogDialog();
	/// start tuchulcha-run process with given arguments
	/** \param args    command line arguments */
	void startProcess(QStringList args);

public slots:
	/// update content by querying process
	void updateContent();
	/// setup abort button and show progress bar
	void setRunning();
	/// setup close button and hide progress bar
	void setFinished();
	/// handle errors running the process
	void error();
	/// handle process termination
	/** \param r       return value */
	virtual void done(int r);
	/// terminate process
	void terminate();

private:
	Ui::LogDialog* _ui; ///< designer ui
	QProcess* _proc;    ///< tuchulcha-run process
};

#endif // LOGDIALOG_H
