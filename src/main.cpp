#include <QApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>
#include <QDir>
#include "Wizard.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setOrganizationName("Heidelberg Collaboratory for Image Processing");
	app.setApplicationName("TemplateGenerator");

	// translator search paths
	QStringList tPaths;
	tPaths << QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#ifdef TEMPLATE_GENERATOR_QM_DIR
	tPaths << TEMPLATE_GENERATOR_QM_DIR;
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
	tName = "charon-templates_" + QLocale::system().name();
	foreach (QString tPath, tPaths) {
		if (translator.load(tName, tPath)) {
			break;
		}
	}
	app.installTranslator(&translator);

	Q_INIT_RESOURCE(resource);
	Wizard wiz;
	wiz.show();
	return app.exec();
}
