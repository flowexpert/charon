#include "WizardPageStart.h"
#include "Wizard.h"
#include "ui_Wizard.h"
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include "QDirEdit.h"

WizardPageStart::WizardPageStart(QWidget* p) :
	QWizardPage(p), _complete(true)
{
}

void WizardPageStart::initializePage() {
	QSettings settings(
		"Heidelberg Collaboratory for Image Processing",
		"TemplateGenerator");
	QAbstractButton* selLoadMod = findChild<QAbstractButton*>("selLoadMod");
	Q_ASSERT(selLoadMod);
	QDirEdit* editLoadMod = findChild<QDirEdit*>("editLoadMod");
	Q_ASSERT(editLoadMod);
	editLoadMod->acceptFiles(true,false);
	editLoadMod->setText(
			settings.value("recentInput", QDir::homePath()).toString());
	registerField("loadPath",editLoadMod);
	registerField("load",selLoadMod);
}

bool WizardPageStart::isComplete() const {
	return _complete;
}

void WizardPageStart::checkComplete() {
	QAbstractButton* selNewMod = findChild<QAbstractButton*>("selNewMod");
	Q_ASSERT(selNewMod);
	QLineEdit* editLoadMod = findChild<QLineEdit*>("editLoadMod");
	Q_ASSERT(editLoadMod);
	bool old = _complete;
	_complete =
			selNewMod->isChecked() ||
			(
				(editLoadMod->text().indexOf(
						QRegExp("(cpp|h|hxx)",Qt::CaseInsensitive)) >= 0) &&
				QFileInfo(editLoadMod->text()).isFile());
	if (_complete != old)
		emit completeChanged();
}
