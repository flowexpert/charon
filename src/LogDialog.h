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
	/// decorator class to handle different kinds of log dialogs
	class Decorator {
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
		/// add highlighting of current output line
		/** \param line    current line
		 *  \returns       current line with highlighting */
		virtual QString highlightLine(QString line) const;
		/// check if current line shows that running finished
		/** \param line    current line
		 *  \retval true   line shows that execution finished */
		virtual bool finishSignal(QString line) const;
		/// message shown if finished
		virtual QString finishMessage() const;
		/// hint for filename on save dialog
		virtual QString filenameHint() const;
		/// debug output mode
		bool debugOutput;
	};

public:
	/// constructor
	/** \param decorator dialog decorator providing further details,
	 *                   it will be deleted in the destructor
	 *  \param parent    parent widget
	 *  \param f         window flags
	 */
	explicit LogDialog(
		Decorator* decorator, QWidget* parent=0, Qt::WindowFlags f=0);
	virtual ~LogDialog();

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
	/// initialize log output widget
	void resetLogWidget();
	/// reparse log output
	void reprint();
	/// print status message
	void printStatus(QString msg);

private slots:
	/// update content by querying process (stdout)
	void on_proc_readyReadStandardOutput();
	/// update content by querying process (stderr)
	void on_proc_readyReadStandardError();
	/// setup abort button and show progress bar
	void on_proc_started();
	/// setup close button and hide progress bar
	void on_proc_finished(int);
	/// handle errors running the process
	void on_proc_error(QProcess::ProcessError);
	/// handle debug checkbox
	void on_checkDD_toggled(bool);
	/// save logfile
	void on_buttonSave_clicked();

private:
	Decorator* _decorator; ///< decorator implementation
	Ui::LogDialog* _ui;    ///< designer ui
	QProcess* _proc;       ///< tuchulcha-run process
	QTextCursor* _curRet;  ///< proc output cursor
	QTextCursor* _curEnd;  ///< other stuff cursor
	QString _logCache;     ///< cache log content
	QString _logEnd;       ///< cache end messages
	QMutex* _logMutex;     ///< avoid parallel writes to log window
};

/// LogDialog decorator implementations
namespace LogDecorators {
	/// decorator for update dialog
	class Update : public LogDialog::Decorator {
	public:
		virtual QStringList arguments() const;
		virtual QString title() const;
		virtual QString desc() const;
		virtual QString filenameHint() const;
	};

	/// decorator for update dynamics dialog
	class UpdateDynamics : public LogDialog::Decorator {
	public:
		/// constructor
		/** \param fileName worflow file to analyze */
		UpdateDynamics(QString fileName);
		virtual QStringList arguments() const;
	private:
		QString _fileName; ///< filename cache
	};

	/// decorator for run workspace dialog
	class RunWorkflow : public QObject, public LogDialog::Decorator {
		Q_OBJECT
	public:
		/// constructor
		/** \param fileName worflow file to run */
		RunWorkflow(QString fileName);
		virtual bool ready(QWidget* parent) const;
		virtual QStringList arguments() const;
		virtual bool finishSignal(QString line) const;
		virtual QString finishMessage() const;
		virtual QStringList postStartCommands(QWidget* parent) const;
		virtual QString highlightLine(QString line) const;
		virtual QString filenameHint() const;
	signals:
		/// highlight the currently active object
		/** \param objName  object name */
		void highlightObject(QString objName) const;
	private:
		QString _fileName; ///< filename cache
	};
}

#endif // LOGDIALOG_H
