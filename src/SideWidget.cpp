#include <QVBoxLayout>
#include <QListWidget>
#include <QStatusBar>
#include "SideWidget.h"
SideWidget::SideWidget(QWidget* pp) :
		QWidget(pp) {
	QVBoxLayout* ll = new QVBoxLayout(this);
	ll->addWidget(_list = new QListWidget());
	_list->setFrameStyle(QFrame::NoFrame);
	_list->setObjectName("sideWidget");
	_list->setEnabled(false);
	ll->addWidget(_status = new QStatusBar());
	_status->setSizeGripEnabled(false);
	setMinimumWidth(180);
}

void SideWidget::addItem(QString text) {
	_list->addItem(text);
}

void SideWidget::updateProgress(int id) {
	_list->setCurrentRow(id);
}

void SideWidget::showMessage(QString msg) {
	_status->showMessage(msg);
}
