#include "Wizard.h"
#include <QDir>
#include <QDate>
#include <QSettings>
#include <QMessageBox>
#include <QTextStream>
#include "WizardPageStart.h"
#include "WizardPageMetadata.h"
#include "WizardPageSlots.h"
#include "WizardPageParameters.h"
#include "WizardPageSummary.h"
#include "SideWidget.h"

Wizard::Wizard(QWidget* p) :
		QWizard(p) {
	addPage(new WizardPageStart(this));
	addPage(new WizardPageMetadata(this));
	addPage(new WizardPageSlots(this));
	addPage(new WizardPageParameters(this));
	addPage(new WizardPageSummary(this));
	setSideWidget(new SideWidget(this));

	QSettings settings(
			"Heidelberg Collaboratory for Image Processing",
			"TemplateGenerator");

	settings.beginGroup("MainWindow");
	if (settings.contains("geometry")) {
		restoreGeometry(settings.value("geometry").toByteArray());
	}
	settings.endGroup();

	connect(this,SIGNAL(currentIdChanged(int)),
			sideWidget(),SLOT(updateProgress(int)));
}

Wizard::~Wizard() {
}

void Wizard::done(int res) {
	// save settings
	QSettings settings(
			"Heidelberg Collaboratory for Image Processing",
			"TemplateGenerator");

	if (res == QDialog::Accepted) {
		if (!_writeFiles())
			return;

		if(field("loadExisting").toBool()) {
			settings.setValue("recentInput", field("loadPath"));
		}
		settings.setValue("author", field("author"));
		settings.setValue("email", field("email"));

		settings.beginGroup("Paths");
		settings.setValue("headerSeparate", field("headerSeparate"));
		settings.setValue("commonOut", field("commonOut"));
		settings.setValue("headerOut", field("headerOut"));
		settings.setValue("sourceOut", field("sourceOut"));
		settings.endGroup();
	}

	settings.beginGroup("MainWindow");
	settings.setValue("geometry",saveGeometry());
	settings.endGroup();

	QWizard::done(res);
}

bool Wizard::_writeFiles() {
	if(field("templated").toBool()) {
		return
			_replacePlaceholders(
				":/templates/Temp.h", field("hFileOut").toString()) &&
			_replacePlaceholders(
				":/templates/Temp.hxx", field("hxxFileOut").toString()) &&
			_replacePlaceholders(
				":/templates/Temp.cpp", field("cppFileOut").toString());
	}
	else {
		return
			_replacePlaceholders(
				":/templates/NonTemp.h", field("hFileOut").toString()) &&
			_replacePlaceholders(
				":/templates/NonTemp.cpp", field("cppFileOut").toString());
	}
	return false;
}

bool Wizard::_replacePlaceholders(QString src, QString dst) {
	// load template file from src
	QFile srcFile(src);
	if (!srcFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, tr("template read failed"),
				tr("Failed to read from %1.").arg(src));
		return false;
	}
	QTextStream srcStrm(&srcFile);
	QString txt = srcStrm.readAll();

	QString author = field("author").toString().trimmed();
	QString authoremail = QString("<a href=\"mailto:%2\">%1</a>")
			.arg(author).arg(field("email").toString().trimmed());
	QString name = field("name").toString().trimmed();

	// setup fill texts
	QString doxyDoc = QString("/// %1\n/** %2\n */")
			.arg(field("briefDesc").toString())
			.arg(field("description").toString());
	QString pluginDoc;

	QString addHeaders;
	if(field("useVigra").toBool())
		addHeaders += "#include <vigra/multi_array.hxx>\n";
	if(field("useCImg").toBool())
		addHeaders += "#include <charon-utils/CImg.h>\n";

	QString paramSlots;
	QString ctorAdd;
	QString ctorCont;

	// replace placeholders
	txt.replace("@Author@", author);
	txt.replace("@AuthorEmail@", authoremail);
	txt.replace("@ModuleName@", name, Qt::CaseSensitive);
	txt.replace("@modulename@",name.toLower(), Qt::CaseSensitive);
	txt.replace("@MODULENAME@",name.toUpper(), Qt::CaseSensitive);
	txt.replace("@Year@",QDate::currentDate().toString("yyyy"));
	txt.replace("@Date@",QDate::currentDate().toString("dd.MM.yyyy"));
	txt.replace("@DoxyDocstring@", doxyDoc);
	txt.replace("@addHeaders@", addHeaders);
	txt.replace("@ParamSlots@", paramSlots);
	txt.replace("@PluginDoc@", pluginDoc);
	txt.replace("@ctorAdd@", ctorAdd);
	txt.replace("@ctorCont@", ctorCont);

	// write result to dst
	QFile dstFile(dst);
	if (!dstFile.open(QIODevice::WriteOnly | QIODevice::Truncate |
				QIODevice::Text)) {
		QMessageBox::warning(this, tr("write result failed"),
				tr("Failed to write to %1.").arg(dst));
		return false;
	}
	QTextStream dstStrm(&dstFile);
	dstStrm << txt;

	return true;
}
