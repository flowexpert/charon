#include "WizardPageMetadata.h"
#include "ui_WizardPageMetadata.h"
#include <QSettings>
#include <QLineEdit>
#include <QPlainTextEdit>

WizardPageMetadata::WizardPageMetadata(QWidget* p) :
		QWizardPage(p),
		_ui(new Ui::WizardPageMetadata) {
	_ui->setupUi(this);

	registerField("author*", _ui->editAuthor);
	registerField("email*", _ui->editEmail);
	registerField("name*", _ui->editModName);
	registerField(
			"description*", _ui->editModDesc,
			"plainText", SIGNAL(textChanged()));
}

WizardPageMetadata::~WizardPageMetadata() {
	delete _ui;
}

void WizardPageMetadata::initializePage() {
	QWizardPage::initializePage();

	QSettings settings(
		"Heidelberg Collaboratory for Image Processing",
		"TemplateGenerator");

	_ui->editAuthor->setText(
			settings.value("author", tr("Unknown")).toString());
	_ui->editEmail->setText(
			settings.value("email", "").toString());
}
