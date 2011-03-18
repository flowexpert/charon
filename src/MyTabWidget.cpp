#include "MyTabWidget.h"
#include <QFile>
#include <QtGui>

MyTabWidget::MyTabWidget(QWidget* p) :
		QTabWidget(p) {
}

void MyTabWidget::nextPage() {
	int temp = currentIndex();
	temp++;
	if (temp < this->count())
		setCurrentIndex(temp);
}

void MyTabWidget::previousPage() {
	int temp = currentIndex();
	temp--;
	if (temp >= 0)
		setCurrentIndex(temp);
}
