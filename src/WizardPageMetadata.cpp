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
	QSettings c(":/config/config.ini", QSettings::IniFormat);
	_ui->editAuthor->setValidator(new QRegExpValidator(
			QRegExp(c.value("authorValid").toString()),this));
	_ui->editEmail->setValidator(new QRegExpValidator(
			QRegExp(c.value("emailValid").toString()),this));
	_ui->editModName->setValidator(new QRegExpValidator(
			QRegExp(c.value("modNameValid").toString()),this));
	_ui->editBriefDesc->setValidator(new QRegExpValidator(
			QRegExp(c.value("briefDescValid").toString()),this));
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

bool WizardPageMetadata::isComplete() const {
	int p;
	QString s = _ui->editAuthor->text();
	bool v = _ui->editAuthor->validator()->validate(s, p) ==
				 QValidator::Acceptable;
	_ui->buttonValid1->setChecked(!v);

	s = _ui->editEmail->text();
	v = _ui->editEmail->validator()->validate(s, p) ==
					 QValidator::Acceptable;
	_ui->buttonValid2->setChecked(!v);

	s = _ui->editModName->text();
	v = _ui->editModName->validator()->validate(s, p) ==
					 QValidator::Acceptable;
	_ui->buttonValid3->setChecked(!v);

	s = _ui->editBriefDesc->text();
	v = _ui->editBriefDesc->validator()->validate(s, p) ==
					 QValidator::Acceptable;
	_ui->buttonValid4->setChecked(!v);

	_ui->buttonValid5->setChecked(
			_ui->editModDesc->toPlainText().isEmpty());

	return QWizardPage::isComplete();
}
