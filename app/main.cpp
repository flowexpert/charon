#include <QtGui/QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	Q_INIT_RESOURCE(resource);
    MainWindow w;
    w.show();
    return a.exec();
}
