#include "MyTabWidget.h"
#include <QFile>
#include <QtGui>


MyTabWidget::MyTabWidget(QWidget* p) :
                QTabWidget(p)
{
}

void MyTabWidget::nextPage()
{

    int temp = currentIndex();
    temp++;
    setCurrentIndex(temp);


}



#include "MyTabWidget.moc"
