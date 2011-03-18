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

WizardPageSummary::~WizardPageSummary() {
	delete _ui;
}

void WizardPageSummary::initializePage() {
	QWizardPage::initializePage();

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

	_updateOut();
}

void WizardPageSummary::_updateOut() {
	QString prefixH, prefixS;
	if (_ui->checkHeaderSep->isChecked()) {
		prefixH = _ui->editHeaderOut->text();
		prefixS = _ui->editSourceOut->text();
	}
	else {
		prefixH = _ui->editOutPath->text();
		prefixS = _ui->editOutPath->text();
	}

	_ui->editHOut->setText(
		QString("%1/%2.h").arg(prefixH).arg(field("name").toString()));
	_ui->editHxxOut->setText(
		QString("%1/%2.hxx").arg(prefixH).arg(field("name").toString()));
	_ui->editCppOut->setText(
		QString("%1/%2.cpp").arg(prefixS).arg(field("name").toString()));
	_ui->editCMakeOut->setText(
		QString("%1/CMakeLists.txt").arg(prefixS));
}
