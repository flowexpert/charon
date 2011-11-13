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
		QThread(pp), interactive(true), _quiet(false)
{
	// commandline argument parsing
	QStringListIterator argIter(args);
	argIter.next(); // skip first item (command name)
	while (argIter.hasNext()) {
		QString s = argIter.next();
		if (s == "--quiet") {
			_quiet = true;
		}
		if (s == "update") {
			interactive = false;
		}
		else {
			QTextStream qerr(stderr,QIODevice::WriteOnly);
			qerr << tr("Argument \"%1\" not recognized.").arg(s) << endl;
		}
	}
}

CommunicationHandler::~CommunicationHandler() {
}

void CommunicationHandler::run() {
	if (!interactive) {
		emit updatePlugins();
		return;
	}

	if (!_quiet) {
		QTextStream qout(stdout,QIODevice::WriteOnly);
		qout << tr("Tuchulcha Workflow Executor version %1")
				.arg(TUCHULCHA_VERSION) << "\n"
			 << tr("type \"quit\" to exit this application.") << endl;
	}

	QTextStream qin(stdin,QIODevice::ReadOnly);
	QString line;
	do {
		line = qin.readLine().trimmed().toLower();
		if(line == "quit") {
			break;
		}
		else if (!line.isEmpty()) {
			QTextStream qerr(stderr,QIODevice::WriteOnly);
			qerr << tr("Command \"%1\" not recognized.").arg(line) << endl;
		}
	} while (!line.isNull());
	QApplication::exit();
}
