#include <QApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>
#include "Wizard.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setOrganizationName("Heidelberg Collaboratory for Image Processing");
	app.setApplicationName("TemplateGenerator");

	// translation of qt dialog buttons (apply, close etc.)
	QTranslator qtTranslator;
	qtTranslator.load(
		"qt_" + QLocale::system().name(),
		QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	app.installTranslator(&qtTranslator);

	// translation of tr(...) commands
	QTranslator translator;
	translator.load(
		"charon-templates_" + QLocale::system().name(),
#ifdef TEMPLATE_GENERATOR_QM_DIR
		TEMPLATE_GENERATOR_QM_DIR);
#else
		QCoreApplication::applicationDirPath());
#endif
	app.installTranslator(&translator);

	Q_INIT_RESOURCE(resource);
	Wizard wiz;
	wiz.show();
	return app.exec();
}
