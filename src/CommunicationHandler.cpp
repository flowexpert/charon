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

CommunicationHandler::CommunicationHandler(
	const QStringList& args, QObject* pp) :
		QThread(pp), _interactive(true), _quiet(false), _args(args) {
}

void CommunicationHandler::run() {
	// commandline argument parsing
	QStringListIterator argIter(_args);
	argIter.next(); // skip first item (command name)
	while (argIter.hasNext()) {
		QString s = argIter.next();
		if (s == "--non-interactive") {
			_interactive = false;
		}
		else if (s == "--quiet") {
			_quiet = true;
		}
		else if (s == "update") {
			emit updatePlugins();
		}
		else if (s == "run" && argIter.hasNext()) {
			emit runWorkflow(argIter.next());
		}
		else if (s == "update-dynamics" && argIter.hasNext()) {
			emit updateDynamics(argIter.next());
		}
		else {
			QTextStream qerr(stderr,QIODevice::WriteOnly);
			qerr << tr("Argument \"%1\" not recognized.").arg(s) << endl;
			QApplication::exit(-1);
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
			 << tr("Type \"quit\" to exit this application.") << endl;
	}

	// interactive command handling
	QTextStream qin(stdin,QIODevice::ReadOnly);
	QString line;
	QRegExp runRgx("^run\\s+(.+)$");
	QRegExp updRgx("^update-dynamics\\s+(.+)$");
	do {
		line = qin.readLine() ;
		if(line == "quit") {
			break;
		}
		else if (line == "update") {
			emit updatePlugins();
		}
		else if (line.contains(runRgx)) {
			emit runWorkflow(runRgx.cap(1));
		}
		else if (line.contains(updRgx)) {
			emit updateDynamics(updRgx.cap(1));
		}
		else if (!line.isEmpty()) {
			QTextStream qerr(stderr,QIODevice::WriteOnly);
			qerr << tr("Command \"%1\" not recognized.").arg(line) << endl;
		}
	} while (!line.isNull());
}
