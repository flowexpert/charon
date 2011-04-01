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
	registerField("briefDesc*", _ui->editBriefDesc);
	registerField(
			"description*", _ui->editModDesc,
			"plainText", SIGNAL(textChanged()));

	// validators
	_ui->editAuthor->setValidator(new QRegExpValidator(
			QRegExp("[\\w\\s\\.\\-]+"),this));
	_ui->editEmail->setValidator(new QRegExpValidator(
			QRegExp("[\\w\\.\\-\\+%]+@[\\w\\.\\-]+\\.[\\w]{2,4}"),this));
	_ui->editModName->setValidator(new QRegExpValidator(
			QRegExp("[a-zA-Z]\\w*"),this));
	_ui->editBriefDesc->setValidator(new QRegExpValidator(
			QRegExp("[\\w\\s,;\\-]+"),this));
	_ui->editAuthor->setProperty("required", true);
	_ui->editEmail->setProperty("required", true);
	_ui->editModName->setProperty("required", true);
	_ui->editBriefDesc->setProperty("required", true);
	_ui->editModDesc->setProperty("required", true);
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
