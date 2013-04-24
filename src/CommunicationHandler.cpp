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
/** \file   CommunicationHandler.cpp
 *  \brief  Implementation of class CommunicationHandler
 *  \date   13.11.2011
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "CommunicationHandler.h"
#include "CommunicationHandler.moc"

#include <QTextStream>
#include <QApplication>
#include <QFileInfo>

CommunicationHandler::CommunicationHandler(
	const QStringList& args, QObject* pp) :
		QThread(pp),
		_interactive(true), _quiet(false),
		_taskCount(0), _args(args), _errorCode(EXIT_SUCCESS)
{
	_helpMsg = QString(
		"Tuchulcha Workflow Executor version %1\n"
		"This executable is part of Charon-Suite\n"
		"Heidelberg Collaboratory for Image Processing,\n"
		"University of Heidelberg, 2009-2013\n"
		"http://sourceforge.net/projects/charon-suite/\n\n"
		"PURPOSE\n"
		"\tThis program will execute charon workflow files\n"
		"SYNTAX\n"
		"\ttuchlucha-run <options> command <workflow>\n"
		"COMMANDS\n"
		"\trun,               -f : run the workflow file defined in <workflow>\n"
		"\tupdate,            -u : update the plugin cache\n"
		"\tupdate-dynamics,   -d : update dynamic plugin metadata\n"
		"\thelp, --help, -? , -h : display this help text\n"
		"OPTIONS\n"
		"\t--non-interactive, -n : exit directly after processing of all "
			"given commands.\n\t                        "
			"Otherwise program will only exit after sending "
			"\"quit\"\n\t                        "
			"via stdin or on EOF (aka Ctrl+D)\n"
		"\t--quiet,           -q : suppress banner message at startup\n"
		"NOTE\n\t"
		"It is possible to combine the short options and commands\n\t"
		"e.g. like \"tuchulcha-run -nqf <workflow>\", but please note \n\t"
		"that the -d or -f argument have to be the last\n\t"
		"before the file argument and that -d and -f cannot be used\n\t"
		"together in one concatenation\n\t"
		"(but e.g. \"tuchulcha-run -nq -d <wflow1> -f <wflow2>\" works)\n\n"
	).arg(TUCHULCHA_VERSION) ;

	_helpMsgI = QString(
		"COMMANDS\n\t"
		"help\n\t\tshow this help message\n\t"
		"update\n\t\tupdate plugin information cache\n\t"
		"update-dynamics <workflow>\n\t\tupdate plugin information cache\n\t"
		"\tfor dynamic modules in workflow file\n\t"
		"run <workflow>\n\t\trun the given workflow file\n\t"
		"quit\n\t\tquit interactive mode and exit\n\t\t"
		"(running commands will be finished before exit)"
	);

	// increase task counter if sending task to task queue
	connect(this,
		SIGNAL(updatePlugins()), SLOT(_startTask()),
		Qt::DirectConnection);
	connect(this,
		SIGNAL(updateDynamics(QString)), SLOT(_startTask()),
		Qt::DirectConnection);
	connect(this,
		SIGNAL(runWorkflow(QString)), SLOT(_startTask()),
		Qt::DirectConnection);
}

bool CommunicationHandler::_checkForFileArg(
				QStringListIterator& iter, QString cmd) {
	if(!iter.hasNext()) {
		QTextStream qerr(stderr,QIODevice::WriteOnly);
		qerr << tr("No workflow file provided for command \"%1\"").arg(cmd)
			<< endl;
		_errorCode=EXIT_FAILURE;
		return false;
	}
	if (!QFileInfo(iter.peekNext()).exists()) {
		QTextStream qerr(stderr,QIODevice::WriteOnly);
		qerr << tr("File provided for command \"%1\" does not exist: %2")
					.arg(cmd).arg(iter.peekNext())
			<< endl;
		_errorCode=EXIT_FAILURE;
		return false;
	}
	return true;
}

int CommunicationHandler::errorCode() const {
	return _errorCode;
}

void CommunicationHandler::run() {
	// commandline argument parsing
	QStringListIterator argIter(_args);
	QRegExp runShortRgx("-[qnu]*(?:f|d)?");
	argIter.next(); // skip first item (command name)
	while (argIter.hasNext()) {
		QString s = argIter.next();
		if(s == "help" || s == "--help" || s == "-h" || s == "-?" || s == "/?") {
			QTextStream qout(stdout,QIODevice::WriteOnly);
			qout << _helpMsg << endl;
			return ;
		}
		else if(s == "--non-interactive" || s == "-n") {
			_interactive = false;
		}
		else if (s == "--quiet" || s == "-q") {
			_quiet = true;
		}
		else if (s == "update" || s == "-u") {
			emit updatePlugins();
		}
		else if (s == "run" || s == "-f") {
			if (!_checkForFileArg(argIter, s)) return;
			emit runWorkflow(argIter.next());
		}
		else if (s == "update-dynamics" || s == "-d") {
			if (!_checkForFileArg(argIter, s)) return;
			emit updateDynamics(argIter.next());
		}
		// concatenated form of -q -n -u -f -d, f/d must be last !
		else if (runShortRgx.exactMatch(s)) {
			if (s.contains("q")) {
				_quiet = true;
			}
			if (s.contains("n")) {
				_interactive = false;
			}
			if (s.contains("u")) {
				emit updatePlugins();
			}
			if (s.contains("f")) {
				if (!_checkForFileArg(argIter, s)) return;
				emit runWorkflow(argIter.next());
			}
			else if (s.contains("d")) {
				if (!_checkForFileArg(argIter, s)) return;
				emit updateDynamics(argIter.next());
			}
		}
		else {
			QTextStream qerr(stderr,QIODevice::WriteOnly);
			qerr << tr("Argument \"%1\" not recognized.").arg(s) << endl;
			QThread::exit(-1);
			return ;
		}
	}

	// check interactive loop
	if (!_interactive) {
		return;
	}

	if (!_quiet) {
		QTextStream qout(stdout,QIODevice::WriteOnly);
		qout << tr("Tuchulcha Workflow Executor version %1")
				.arg(TUCHULCHA_VERSION) << "\n"
			 << tr("Type \"quit\" to exit this application.") << "\n"
			 << tr("Type \"help\" for command summary.") << endl;
	}

	// interactive command handling
	QTextStream qin(stdin,QIODevice::ReadOnly);
	QTextStream qout(stdout, QIODevice::WriteOnly);
	QString line;
	QRegExp runRgx("run\\s+(\\S.*)");
	QRegExp updRgx("update-dynamics\\s+(\\S.*)");
	do {
		_printPrompt();
		line = qin.readLine();
		if(line == "quit") {
			break;
		}
		else if (line == "help") {
			qout << _helpMsgI << endl;
		}
		else if (line == "update") {
			emit updatePlugins();
		}
		else if (runRgx.exactMatch(line)) {
			emit runWorkflow(runRgx.cap(1));
		}
		else if (updRgx.exactMatch(line)) {
			emit updateDynamics(updRgx.cap(1));
		}
		else if (!line.isEmpty()) {
			QTextStream qerr(stderr,QIODevice::WriteOnly);
			qerr << tr("Command \"%1\" not recognized.").arg(line) << endl;
		}
	} while (!line.isNull());
	if (line.isNull()) {
		qout << endl; // newline e.g. if terminated by CTRL-D, i.e. EOF
	}
	if (!_quiet) {
		qout << tr("bye") << endl;
	}
}

void CommunicationHandler::_startTask() {
	_taskCount++;
}

void CommunicationHandler::taskFinished() {
	if (_taskCount == 0) {
		qDebug("%s", "Trying to decrease zero task counter");
	}
	else {
		_taskCount--;
	}
	_printPrompt();
}

void CommunicationHandler::_printPrompt() {
	if (_taskCount > 0 || _quiet || !_interactive) {
		return;
	}
	QTextStream qout(stdout, QIODevice::WriteOnly);
	qout << "TC-Run > " << flush; // input prompt (hidden if quiet)
}
