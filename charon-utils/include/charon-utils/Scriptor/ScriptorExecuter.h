/*  Copyright (C) 2012 mox

	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file Scriptor_Executor.h
 *  Declaration of the class Scriptor_Executor.
 *  Scriptor_Executor is a helper class for ScriptorBase
 *  \author <a href="mailto:de_mox@zoho.com">
 *      mox</a>
 *  \date 29.02.2012
 */

#ifndef _SCRIPTOR_EXECUTER_H
#define _SCRIPTOR_EXECUTER_H

#ifdef _MSC_VER
#ifdef ScriptorBase_EXPORTS
/// Visual C++ specific code
#define ScriptorBase_DECLDIR __declspec(dllexport)
#else
#define ScriptorBase_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define ScriptorBase_DECLDIR
#endif


#include <QObject>
#include <QProcess>
#include <QString>


/// Class to execute a script and write it's output to sout
/**
 * This class exists because a TemplatedParameteredObject cannot be a QObject,
 * so everything pertaining to QObject is outsourced to ScriptorExecuter.
 * You should not get in direct contact with this class.
 * All errorhandling for ScriptorExecuter is done in ScriptorBase
 * */
class ScriptorExecuter: public QObject
{
		Q_OBJECT
	public:

		/// constructor
		/**
		 * \param executable path to the script
		 * \param arguments  a list of arguments to the script
		 * \param wordDir    the directory to call the script from
		 * */
		ScriptorExecuter(QString executable, QStringList arguments, QString workDir);

		/// run the script
		int run();

		/// get the exit code of the script
		int exitCode();

	private:

		/// QProcess that actually executs the script
		QProcess _proc;

		/// path to the script
		QString _executable;

		/// arguments to pass to the script
		QStringList _arguments;

		/// exit code from the last run of the sript
		int _exitCode;

	private slots:

		/// reads the script's standard output and writes it to sout
		void _readStandardOut();
		/// reads the script's error output and writes it to sout
		void _readErrorOut();
};

#endif // _SCRIPTOR_EXECUTER_H
