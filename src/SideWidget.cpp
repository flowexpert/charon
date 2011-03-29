#include "SideWidget.h"
#include "ui_SideWidget.h"

SideWidget::SideWidget(QWidget* pp) :
	QWidget(pp),
	_ui(new Ui::SideWidget)
{
	_ui->setupUi(this);
}

SideWidget::~SideWidget()
{
	delete _ui;
}

void SideWidget::updateProgress(int id) {
	_ui->listProgress->setCurrentRow(id);
}
