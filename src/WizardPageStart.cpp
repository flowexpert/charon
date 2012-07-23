#include "WizardPageStart.h"
#include "ui_WizardPageStart.h"
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QAbstractButton>
#include <QTextStream>
#include <QUrl>
#include "QDirEdit.h"

WizardPageStart::WizardPageStart(QWidget* p) :
			QWizardPage(p),
			_complete(true),
			_ui(new Ui::WizardPageStart) {
	_ui->setupUi(this);
	_ui->editLoadMod->acceptFiles(true,false);
	registerField("loadPath",_ui->editLoadMod);
	registerField("loadExisting",_ui->selLoadMod);

	// update version and build info
	QString buildSystem =
#if defined (_MSC_VER)
		tr("MSVC") + " " +tr("%1 (%2bit, %3)").arg(_MSC_VER)
			.arg(sizeof(void*)*8).arg(CMAKE_INTDIR);
#elif defined (__GNUC__)
		tr("GCC") + " " +tr("%1 (%2bit, %3)")
				.arg(QString("%1.%2.%3")
					.arg(__GNUC__)
					.arg(__GNUC_MINOR__)
					.arg(__GNUC_PATCHLEVEL__))
				.arg(sizeof(void*)*8)
		#ifdef QT_DEBUG
				.arg("Debug");
		#else
				.arg("Release");
		#endif
#else
		tr("unknown compiler (%1bit)").arg(sizeof(*void)*8);
#endif

	// add information
	_ui->lVersion->setText(
		QString(TEMPGEN_VERSION)
#ifdef VCSINFO
		+QString(" (%1)").arg(VCSINFO)
#endif
	);
	_ui->lBuild->setText(QString("%1 %2").arg(__DATE__).arg(__TIME__));
	_ui->lBuildSys->setText(
		buildSystem
#ifdef BUILD_INFO
		+QString(" (%1)").arg(BUILD_INFO)
#endif
	);

	// hide boxes with unimplemented features
	_ui->boxStart->hide();
	_ui->boxLoad->hide();
}

WizardPageStart::~WizardPageStart() {
	delete _ui;
}

void WizardPageStart::initializePage() {
	QWizardPage::initializePage();

	QSettings settings;
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
