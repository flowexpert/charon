/*	Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file tuchulcha-run.cpp
 *  \brief workflow execution process
 *
 *  \date 13.11.2011
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>

#include "CommunicationHandler.h"
#include "CharonRun.h"

/** Start of main application.
 * \param argc  Counter of command line arguments.
 * \param argv  Content of command line arguments.
 * \return      0 in case of sucessful execution.
 */
int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	app.setOrganizationName("Heidelberg Collaboratory for Image Processing");
	app.setApplicationName("Tuchulcha");

	// translation of qt dialog buttons (apply, close etc.)
	QTranslator qtTranslator;
	qtTranslator.load(
		"qt_" + QLocale::system().name(),
		QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	app.installTranslator(&qtTranslator);

	// translation of tr(...) commands
	QTranslator translator;
	translator.load("tuchulcha_" + QLocale::system().name());
	app.installTranslator(&translator);

	CommunicationHandler comm(app.arguments());
	CharonRun run;
	run.connect(&comm,SIGNAL(updatePlugins()),SLOT(updatePlugins()));
	run.connect(&comm,SIGNAL(runWorkflow(QString)),SLOT(runWorkflow(QString)));
	run.connect(&comm,SIGNAL(started()),SLOT(lock()));
	run.connect(&comm,SIGNAL(finished()),SLOT(unlock()));

	comm.start();
	int ret = app.exec();
	comm.wait();

	return ret;
}
