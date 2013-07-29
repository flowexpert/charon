/*	Copyright (C) 2009 Jens-Malte Gottfried

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
/** \file tuchulcha.cpp
 *  \brief Main application
 *
 *  \date 27.08.2008
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include <cstdlib>
#include <QApplication>
#include <QErrorMessage>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include <QTimer>
#include <QSplashScreen>
#include <QPainter>

#include "TuchulchaWindow.h"
#include "FileManager.h"

/** Start of main application.
 * \param argc  Counter of command line arguments.
 * \param argv  Content of command line arguments.
 * \return      0 in case of sucessful execution.
 */
int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	app.setOrganizationName("Heidelberg Collaboratory for Image Processing");
	app.setApplicationName("Tuchulcha");
	Q_INIT_RESOURCE(resources);

	// translator search paths
	QStringList tPaths;
	tPaths << QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#ifdef TUCHULCHA_QM_DIR
	tPaths << TUCHULCHA_QM_DIR;
#endif
	tPaths << QCoreApplication::applicationDirPath();

	// qt system translator
	// translation of qt dialog buttons (apply, close etc.)
	QTranslator qtTranslator;
	QString tName = "qt_" + QLocale::system().name();
	foreach (QString tPath, tPaths) {
		if (qtTranslator.load(tName, tPath)) {
			break;
		}
	}
	app.installTranslator(&qtTranslator);

	// translation of tr(...) commands
	QTranslator translator;
	tName = "tuchulcha_" + QLocale::system().name();
	foreach (QString tPath, tPaths) {
		if (translator.load(tName, tPath)) {
			break;
		}
	}
	app.installTranslator(&translator);

	// main window
	TuchulchaWindow window;
	FileManager::dialogParent = &window;
#ifndef UNIX
	QErrorMessage* handler = QErrorMessage::qtHandler();
	handler->setModal(true);
#endif

	QImage sImage(":/icons/splash.png");
	QPainter sPainter(&sImage);
	sPainter.setFont(QFont("sans-serif",14,QFont::DemiBold));
	sPainter.drawText(QRectF(0.f,160.f,320.f,80.f),Qt::AlignCenter,TUCHULCHA_VERSION);
	QSplashScreen splash(QPixmap::fromImage(sImage));
	splash.setFont(QFont("sans-serif",12,QFont::DemiBold));
	splash.show();
	splash.showMessage(QApplication::translate("tuchulcha","starting Tuchulcha"));
	splash.connect(&window,SIGNAL(statusMessage(QString)),SLOT(showMessage(QString)));
	app.processEvents();

	// handle command line arguments
	QStringList args = app.arguments();
	if (args.first().compare(app.applicationName(),Qt::CaseInsensitive)==0
			|| QFileInfo(args.first()).absoluteFilePath() ==
			QFileInfo(app.applicationFilePath()).absoluteFilePath()) {
		// avoid tuchulcha command interpreted as file to open
		args.pop_front();
	}
	window.open(args);
	window.show();
	splash.finish(&window);
	return app.exec();
}
