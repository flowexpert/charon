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

	QListWidget* sWidget = new SideWidget(this);
	setSideWidget(sWidget);
	const QList<int>& pId = pageIds();
	for(int ii=0; ii < pId.size(); ii++) {
		sWidget->addItem(page(pId[ii])->title());
	}

	QSettings settings(
			"Heidelberg Collaboratory for Image Processing",
			"TemplateGenerator");

	settings.beginGroup("MainWindow");
	if (settings.contains("geometry")) {
		restoreGeometry(settings.value("geometry").toByteArray());
	}
	settings.endGroup();

	connect(this,SIGNAL(currentIdChanged(int)),
			sWidget,SLOT(updateProgress(int)));

	QFile styleFile(":/look/Style.css");
	styleFile.open(QFile::ReadOnly|QFile::Text);
	QTextStream styleStrm(&styleFile);
	QString styleString(styleStrm.readAll());
	setStyleSheet(styleString);
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

	// setup fill texts
	QString author = field("author").toString().trimmed();
	QString authoremail = QString("<a href=\"mailto:%2\">\n *      %1</a>")
			.arg(author).arg(field("email").toString().trimmed());
	QString name = field("name").toString().trimmed();
	QString doxyDoc = QString("/// %1\n/** %2\n */")
			.arg(breakLines(
					field("briefDesc").toString(), "\n/// "))
			.arg(breakLines(
					field("description").toString(), "\n *  "));
	QString pluginDoc = QString("<h2>%1</h2>\n%2")
			.arg(field("briefDesc").toString().trimmed())
			.arg(field("description").toString().trimmed());
	pluginDoc = breakLines(pluginDoc, " \"\n\t\t\t\"", "<br>");
	QString addHeaders;
	if(field("useVigra").toBool())
		addHeaders += "#include <vigra/multi_array.hxx>\n";
	if(field("useCImg").toBool())
		addHeaders += "#include <charon-utils/CImg.h>\n";
	QString paramSlots;
	QString ctorAdd;
	QString ctorCont;

	// replace placeholders
	txt.replace("@Author@",     author);
	txt.replace("@AuthorEmail@",authoremail);
	txt.replace("@ModuleName@", name, Qt::CaseSensitive);
	txt.replace("@modulename@", name.toLower(), Qt::CaseSensitive);
	txt.replace("@MODULENAME@", name.toUpper(), Qt::CaseSensitive);
	txt.replace("@Year@",       QDate::currentDate().toString("yyyy"));
	txt.replace("@Date@",       QDate::currentDate().toString("dd.MM.yyyy"));
	txt.replace("@DoxyDoc@",    doxyDoc);
	txt.replace("@addHeaders@", addHeaders);
	txt.replace("@ParamSlots@", paramSlots);
	txt.replace("@PluginDoc@",  pluginDoc);
	txt.replace("@ctorAdd@",    ctorAdd);
	txt.replace("@ctorCont@",   ctorCont);

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

QString Wizard::breakLines(QString src, QString dlm, QString nl, uint ll) {
	// replace newlines to get single line string
	src = src.trimmed();
	src.replace("\n", QString("@nl@ ").leftJustified(nl.length(), ' '));

	// reduced string width regarding delimiter (assuming tab width 4)
	int width = ll - dlm.length() - 3*dlm.count('\t');
	Q_ASSERT(width >= 40); // too small widths do not work

	QString dst = "";
	while (src.length() >= width) {
		// find split position
		int pos=src.indexOf(QRegExp("@nl@\\s*"))+4;
		if (pos < 4 || pos >= width) {
			for(int cur=0; cur<width && cur>=0;
					cur=src.indexOf(QRegExp("\\s"),cur+1)) {
				pos = cur;
			}
		}
		if (pos <= 0)
			break;

		// append beginning of src to dst, adding dlm if neccessary
		dst.append(src.left(pos).trimmed());
		src.remove(0,pos);
		src = src.trimmed();
		if(!src.isEmpty())
			dst.append(dlm);
	}
	dst.append(src.trimmed());

	// newline fixup
	dst.replace(QRegExp("@nl@ *"), nl);

	return dst;
}
