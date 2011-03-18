#include "WizardPageSummary.h"
#include "ui_WizardPageSummary.h"
#include <QSettings>
#include <QCheckBox>
#include <QLineEdit>
#include <QDir>

WizardPageSummary::WizardPageSummary(QWidget* p) :
		QWizardPage(p),
		_ui(new Ui::WizardPageSummary) {
	_ui->setupUi(this);

	registerField("headerSeparate", _ui->checkHeaderSep);
	registerField("commonOut", _ui->editOutPath);
	registerField("headerOut", _ui->editHeaderOut);
	registerField("sourceOut", _ui->editSourceOut);
}

void WizardPageSummary::initializePage() {
	QSettings settings(
		"Heidelberg Collaboratory for Image Processing",
		"TemplateGenerator");

	settings.beginGroup("Paths");
	_ui->checkHeaderSep->setChecked(
			settings.value("headerSeparate", false).toBool() ?
			Qt::Unchecked : Qt::Checked);
	_ui->checkHeaderSep->toggle();

	_ui->editOutPath->setText(
			settings.value("commonOut", QDir::homePath()).toString());
	_ui->editHeaderOut->setText(
			settings.value("headerOut", QDir::homePath()).toString());
	_ui->editSourceOut->setText(
			settings.value("sourceOut", QDir::homePath()).toString());
	settings.endGroup();
}
