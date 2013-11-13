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
/** \file LogDecorators.h
 *  \brief Declaration of classes in the LogDecorators namespace
 *
 *  \date 13.11.2011
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef LOG_DECORATORS_H
#define LOG_DECORATORS_H

#include <QObject>
#include <QRegExp>
#include <QFont>

class QStandardItemModel;
class QWidget;
class QTableView;
class QModelIndex;

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
		/// decorator hook to allow log line parsing
		/** e.g. to detect finishing (to emit the corresponding signal)
		 *  \param line    current line */
		virtual void processLine(QString line);
		/// inform about finished processing
		virtual void finishProcessing();
		/// custom status widget
		virtual QWidget* statusWidget();
		/// html anchor on help page
		virtual QString helpAnchor();

	signals:
		void finish();         ///< finish signal
		void message(QString); ///< status message
		void filter(QString);  ///< set search filter
	};

	/// decorator for update dialog
	class Update : public Decorator {
		Q_OBJECT
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
		virtual QWidget* statusWidget();
		virtual QString helpAnchor();

		/// status code used in log parsing
		enum StatusCode {
			Invalid  = 0, ///< should not occur, initial value
			Passed   = 1, ///< success
			NoPlugin = 2, ///< info message: no plugin
			Warnings = 4, ///< warnings occurred
			Failed   = 6  ///< Plugin failed
		};
	private:
		QString     _summary;        ///< update summary
		QString     _curFile;        ///< file name cache
		StatusCode  _curStatus;      ///< status cache
		QString     _curPlugin;      ///< plugin name cache
		QRegExp     _fileRegex;      ///< file name regexp
		QRegExp     _noPluginRegex;  ///< no plugin info
		QRegExp     _warnRegex;      ///< no plugin info
		QRegExp     _failRegex;      ///< fail regex
		QRegExp     _passRegex;      ///< plugin passed regexp
		QRegExp     _finishRegex;    ///< plugin unloaded
		QStandardItemModel* _result; ///< update summary
		QTableView* _view;           ///< summary list
		QFont       _ttFont;         ///< typewriter font
		QFont       _sfFont;         ///< sans-serif font
	private slots:
		/// show corresponding log line on selection
		void _searchOutput(const QModelIndex&);
		/// append log row with current information
		void _appendSummaryRow();
	};

	/// decorator for update dynamics dialog
	class UpdateDynamics : public Decorator {
	public:
		/// constructor
		/** \param fileName worflow file to analyze */
		UpdateDynamics(QString fileName);
		virtual QString title() const;
		virtual QString desc() const;
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
		virtual QString helpAnchor();
	signals:
		/// highlight the currently active object
		/** \param objName  object name */
		void highlightObject(QString objName) const;
	private:
		QString _fileName; ///< filename cache
	};
}

#endif // LOG_DECORATORS_H
