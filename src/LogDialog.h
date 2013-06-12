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
#include <QProcess>

namespace Ui {class LogDialog;}
class QTextCursor;
class QMutex;
class QFile;
class QStringListModel;
class LogViewProxyModel;

namespace LogDecorators{ class Decorator; }

/// class for logging display and communication with external processes
/** This widget implements the decorator pattern to handle different
 *  kinds of log dialogs like the one used during plugin update
 *  and running of workflows.
 *
 *  The decorator implementation has to be given to the instance constructor.
 */
class LogDialog : public QDialog
{
	Q_OBJECT

public:
	/// constructor
	/** \param decorator dialog decorator providing further details,
	 *                   it will be deleted in the destructor
	 *  \param parent    parent widget
	 *  \param f         window flags
	 */
	explicit LogDialog(
		LogDecorators::Decorator* decorator,
		QWidget* parent=0, Qt::WindowFlags f=0);
	virtual ~LogDialog();
	/// wait for process to finish
	/** \param msecs     time to wait
	 *  \retval false    process still running (timeout)
	 */
	bool waitForFinished(int msecs=1500);

public slots:
	/// handle process termination
	/** \param r       return value */
	virtual void done(int r);
	/// terminate process
	/** \param force   do not ask before terminating */
	void terminate(bool force=false);
	/// kill process
	/** \param force   do not ask before terminating */
	void kill(bool force=false);
	/// reparse log output
	void reprint();
	/// print status message
	void printStatus(QString msg);
	/// load config from settings
	void loadSettings();
	/// store config to settings
	void saveSettings();

private slots:
	/// update content by querying process (stdout)
	void on_proc_readyReadStandardOutput();
	/// update content by querying process (stderr)
	void on_proc_readyReadStandardError();
	/// setup abort button and show progress bar
	void on_proc_started();
	/// setup close button and hide progress bar
	void on_proc_finished();
	/// handle errors running the process
	void on_proc_error(QProcess::ProcessError);
	/// handle debug checkbox
	void on_checkDD_toggled(bool);
	/// save logfile
	void on_bSaveLog_clicked();
	/// handle buf size changes
	void on_sBufSize_valueChanged(int);
	/// search down
	void on_bSearchDown_clicked();
	/// search up
	void on_bSearchUp_clicked();
	/// search for given string
	/** \param filter  search filter (unix wildcard)
	 *  \param offset  serach offset (added to current row)
	 *  \param up      search upward (reverse) */
	void searchLog(QString filter, int offset=0, bool up=false);
	/// handle search filter changes
	void on_eFilter_textEdited(QString);

private:
	QStringListModel* _log;       ///< log model
	LogViewProxyModel* _logProx;  ///< log model proxy
	/// decorator implementation
	LogDecorators::Decorator* _decorator;
	Ui::LogDialog* _ui;           ///< designer ui
	QProcess* _proc;              ///< tuchulcha-run process
	QFile* _logFile;              ///< log content output
	QMutex* _logMutex;            ///< avoid parallel writes to log window
};

#endif // LOGDIALOG_H
