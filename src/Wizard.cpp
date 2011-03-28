#include "Wizard.h"
#include <QDir>
#include <QSettings>
#include <QMessageBox>
#include "WizardPageStart.h"
#include "WizardPageMetadata.h"
#include "WizardPageSlots.h"
#include "WizardPageParameters.h"
#include "WizardPageSummary.h"

Wizard::Wizard(QWidget* p) :
		QWizard(p) {
	addPage(new WizardPageStart(this));
	addPage(new WizardPageMetadata(this));
	addPage(new WizardPageSlots(this));
	addPage(new WizardPageParameters(this));
	addPage(new WizardPageSummary(this));

	QSettings settings(
			"Heidelberg Collaboratory for Image Processing",
			"TemplateGenerator");

	settings.beginGroup("MainWindow");
	if (settings.contains("geometry")) {
		restoreGeometry(settings.value("geometry").toByteArray());
	}
	settings.endGroup();
}

Wizard::~Wizard() {
}

void Wizard::done(int res) {
	// save settings
	QSettings settings(
			"Heidelberg Collaboratory for Image Processing",
			"TemplateGenerator");
	if(field("loadExisting").toBool()) {
		settings.setValue("recentInput", field("loadPath"));
	}
	if (!field("author").toString().isEmpty())
		settings.setValue("author", field("author"));
	if (!field("email").toString().isEmpty())
		settings.setValue("email", field("email"));

	settings.beginGroup("Paths");
	settings.setValue("headerSeparate", field("headerSeparate"));
	if (!field("commonOut").toString().isEmpty())
		settings.setValue("commonOut", field("commonOut"));
	if (!field("headerOut").toString().isEmpty())
		settings.setValue("headerOut", field("headerOut"));
	if (!field("sourceOut").toString().isEmpty())
		settings.setValue("sourceOut", field("sourceOut"));
	settings.endGroup();

	settings.beginGroup("MainWindow");
	settings.setValue("geometry",saveGeometry());
	settings.endGroup();

	QWizard::done(res);
}
