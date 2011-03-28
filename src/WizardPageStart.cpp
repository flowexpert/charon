#include "WizardPageStart.h"
#include "ui_WizardPageStart.h"
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QAbstractButton>
#include "QDirEdit.h"

WizardPageStart::WizardPageStart(QWidget* p) :
			QWizardPage(p),
			_complete(true),
			_ui(new Ui::WizardPageStart) {
	_ui->setupUi(this);
	_ui->editLoadMod->acceptFiles(true,false);
	registerField("loadPath",_ui->editLoadMod);
	registerField("loadExisting",_ui->selLoadMod);
}

WizardPageStart::~WizardPageStart() {
	delete _ui;
}

void WizardPageStart::initializePage() {
	QWizardPage::initializePage();

	QSettings settings(
		"Heidelberg Collaboratory for Image Processing",
		"TemplateGenerator");
	_ui->editLoadMod->setText(
			settings.value("recentInput", QDir::homePath()).toString());
}

bool WizardPageStart::isComplete() const {
	return QWizardPage::isComplete() && _complete;
}

void WizardPageStart::checkComplete() {
	bool old = _complete;
	_complete =
			_ui->selNewMod->isChecked() ||
			(
				(_ui->editLoadMod->text().indexOf(
						QRegExp("(cpp|h|hxx)",Qt::CaseInsensitive)) >= 0) &&
				QFileInfo(_ui->editLoadMod->text()).isFile());
	if (_complete != old)
		emit completeChanged();
}
