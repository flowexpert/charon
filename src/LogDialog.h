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
class QStandardItemModel;

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
	bool waitForFinished(int msecs=500);

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

/// LogDialog decorator implementations
namespace LogDecorators {
	/// log decorator base class to handle different kinds of log dialogs
	class Decorator : public QObject {
		Q_OBJECT
	public:
		Decorator();
		virtual ~Decorator();
		/// title string
		virtual QString title() const;
		/// description string
		virtual QString desc() const;
		/// check if process may be started
		/** \param parent  parent widget (for dialog/message boxes)
		 *  \retval false  process may not be started */
		virtual bool ready(QWidget* parent) const;
		/// determine command line arguments
		virtual QStringList arguments() const = 0;
		/// commands sent to the proccess after start
		/** \param parent  parent widget (for dialog/message boxes)
		 *  \returns       list of (interactive) commands */
		virtual QStringList postStartCommands(QWidget* parent) const;
		/// hint for filename on save dialog
		virtual QString filenameHint() const;
		/// logfile name for output logging
		virtual QString logFileName() const = 0;
		/// debug output mode
		bool debugOutput;
		/// check if current line shows that running finished
		/** \param line    current line
		 *  \retval true   line shows that execution finished */
		virtual void processLine(QString line);
		/// inform about finished processing
		virtual void finishProcessing();

	signals:
		void finish();         ///< finish signal
		void message(QString); ///< status message

	};

	/// decorator for update dialog
	class Update : public Decorator {
	public:
		Update();
		virtual QStringList arguments() const;
		virtual QStringList postStartCommands(QWidget* parent) const;
		virtual QString title() const;
		virtual QString desc() const;
		virtual QString filenameHint() const;
		virtual QString logFileName() const;
		virtual void processLine(QString line);
		virtual void finishProcessing();
	private:
		QString _summary;       ///< update summary
		QString _curFile;       ///< file name cache
		QString _curStatus;     ///< status cache
		QRegExp _fileRegex;     ///< file name regexp
		QRegExp _noPluginRegex; ///< no plugin info
		QRegExp _passRegex;     ///< plugin passed regexp
		QStandardItemModel* _result; ///< update summary
	};

	/// decorator for update dynamics dialog
	class UpdateDynamics : public Decorator {
	public:
		/// constructor
		/** \param fileName worflow file to analyze */
		UpdateDynamics(QString fileName);
		virtual QStringList arguments() const;
		virtual QString logFileName() const;
	private:
		QString _fileName; ///< filename cache
	};

	/// decorator for run workspace dialog
	class RunWorkflow : public Decorator {
		Q_OBJECT
	public:
		/// constructor
		/** \param fileName worflow file to run */
		RunWorkflow(QString fileName);
		virtual bool ready(QWidget* parent) const;
		virtual QStringList arguments() const;
		/// finish message to display when workflow finished
		virtual QString finishMessage() const;
		virtual QStringList postStartCommands(QWidget* parent) const;
		virtual QString filenameHint() const;
		virtual QString logFileName() const;
		virtual void processLine(QString line);
	signals:
		/// highlight the currently active object
		/** \param objName  object name */
		void highlightObject(QString objName) const;
	private:
		QString _fileName; ///< filename cache
	};
}

#endif // LOGDIALOG_H
