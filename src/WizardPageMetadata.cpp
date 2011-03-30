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
	QPalette pv = palette();
	pv.setColor(QPalette::Base, QColor("#EFE"));
	QPalette pi = palette();
	pi.setColor(QPalette::Base, QColor("#FCC"));

	QString s = _ui->editAuthor->text();
	bool valid = _ui->editAuthor->validator()->validate(s, p) ==
				 QValidator::Acceptable;
	_ui->editAuthor->setPalette(valid?pv:pi);

	s = _ui->editEmail->text();
	valid = _ui->editEmail->validator()->validate(s, p) ==
					 QValidator::Acceptable;
	_ui->editEmail->setPalette(valid?pv:pi);

	s = _ui->editModName->text();
	valid = _ui->editModName->validator()->validate(s, p) ==
					 QValidator::Acceptable;
	_ui->editModName->setPalette(valid?pv:pi);

	s = _ui->editBriefDesc->text();
	valid = _ui->editBriefDesc->validator()->validate(s, p) ==
					 QValidator::Acceptable;
	_ui->editBriefDesc->setPalette(valid?pv:pi);

	s = _ui->editModDesc->toPlainText();
	valid = !s.isEmpty();
	_ui->editModDesc->setPalette(valid?pv:pi);

	return QWizardPage::isComplete();
}
