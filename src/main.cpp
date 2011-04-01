#include <QtGui/QApplication>
//#include "MainWindow.h"
#include "Wizard.h"
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Q_INIT_RESOURCE(resource);
	//MainWindow w;
	Wizard w;
	w.show();
	return a.exec();
}
