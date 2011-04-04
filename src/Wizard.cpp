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
	else {
		if(visitedPages().size() > 1 && QMessageBox::question(
				this,tr("confirm exit"),
				tr("Do you really want to close?<br>"
					"All input done so far will be lost."),
				QMessageBox::Ok,QMessageBox::Cancel) ==
					QMessageBox::Cancel)
		{
			// abort closing
			return;
		}
	}

	settings.beginGroup("MainWindow");
	settings.setValue("geometry",saveGeometry());
	settings.endGroup();

	QWizard::done(res);
}

bool Wizard::_writeFiles() {
	bool res = true;
	if(field("templated").toBool()) {
		res =
			_replacePlaceholders(
				":/templates/Temp.h", field("hFileOut").toString()) &&
			_replacePlaceholders(
				":/templates/Temp.hxx", field("hxxFileOut").toString()) &&
			_replacePlaceholders(
				":/templates/Temp.cpp", field("cppFileOut").toString());
	}
	else {
		res =
			_replacePlaceholders(
				":/templates/NonTemp.h", field("hFileOut").toString()) &&
			_replacePlaceholders(
				":/templates/NonTemp.cpp", field("cppFileOut").toString());
	}
	if (field("genCMake").toBool()) {
		if(field("headerSeparate").toBool()) {
			QMessageBox::warning(
					this,tr("unimplemented"),
					tr("CMake project file generation for "
						"split header/src configuration "
						"not yet implemented."));
			return false;
		}
		res = res &&
			_replacePlaceholders(
				":/templates/CMakeLists.txt",
				field("cmakeFileOut").toString());
		{
			QFile fin(":/templates/InitFlags.cmake");
			QFile fout(QString("%1/InitFlags.cmake")
							.arg(field("commonOut").toString()));
			if (fin.copy(fout.fileName()))
				fout.setPermissions(fin.permissions() | QFile::WriteUser);
			else
				QMessageBox::warning(
						this, tr("error copying cmake files"),
						tr("Error writing file <tt>%1</tt>.<br>"
							"Perhaps it does already exist. "
							"Skipping copy.").arg(fout.fileName()));
		}
		if (field("useCImg").toBool()) {
			QFile fin(":/templates/CImgConfig.cmake");
			QFile fout(QString("%1/CImgConfig.cmake")
							.arg(field("commonOut").toString()));
			if (fin.copy(fout.fileName()))
				fout.setPermissions(fin.permissions() | QFile::WriteUser);
			else
				QMessageBox::warning(
						this, tr("error copying cmake files"),
						tr("Error writing file <tt>%1</tt>.<br>"
							"Perhaps it does already exist. "
							"Skipping copy.").arg(fout.fileName()));
		}
	}
	return res;
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

	// setup fill texts - name, date, doc
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

	// set up input slot declaration/registration
	if(field("inputSlotNames").toStringList().size() > 0) {
		const QStringList& slotNames =
				field("inputSlotNames").toStringList();
		const QStringList& slotDocs =
				field("inputSlotDocs").toStringList();
		const QStringList& slotTypes =
				field("inputSlotTypes").toStringList();
		const QList<bool>& slotOptional = ParamSlotModel::toBoolList(
				field("inputSlotOptional").toStringList());
		const QList<bool>& slotMulti = ParamSlotModel::toBoolList(
				field("inputSlotMulti").toStringList());

		Q_ASSERT(slotNames.size() == slotDocs.size());
		Q_ASSERT(slotNames.size() == slotTypes.size());
		Q_ASSERT(slotNames.size() == slotOptional.size());
		Q_ASSERT(slotNames.size() == slotMulti.size());

		paramSlots.append("\n");
		ctorCont.append("\n");
		for(int ii=0; ii<slotNames.size(); ii++) {
			paramSlots.append(
				QString("\t/// %2\n\tInputSlot<%3> %1;\n")
					.arg(slotNames[ii])
					.arg(breakLines(slotDocs[ii],"\n\t/// "))
					.arg(slotTypes[ii])
			);
			ctorCont.append(QString(
				"\tParameteredObject::_addInputSlot(\n\t\t"
				"%1, \"%1\",\n\t\t\"%2\",\n\t\t\"%3\");\n")
				.arg(slotNames[ii])
				.arg(breakLines(slotDocs[ii],"\"\n\t\t\""))
				.arg(slotTypes[ii])
			);
			if (slotOptional[ii] || slotMulti[ii]) {
				ctorAdd.append(QString(",\n\t\t%1(%2,%3)")
					.arg(slotNames[ii])
					.arg(slotOptional[ii]?"true":"false")
					.arg(slotMulti[ii]?"true":"false"));
			}
		}
	}

	// set up output slot declaration/registration
	if(field("outputSlotNames").toStringList().size() > 0) {
		const QStringList& slotNames =
				field("outputSlotNames").toStringList();
		const QStringList& slotDocs =
				field("outputSlotDocs").toStringList();
		const QStringList& slotTypes =
				field("outputSlotTypes").toStringList();

		Q_ASSERT(slotNames.size() == slotDocs.size());
		Q_ASSERT(slotNames.size() == slotTypes.size());

		paramSlots.append("\n");
		ctorCont.append("\n");
		for(int ii=0; ii<slotNames.size(); ii++) {
			paramSlots.append(
				QString("\t/// %2\n\tOutputSlot<%3> %1;\n")
					.arg(slotNames[ii])
					.arg(breakLines(slotDocs[ii],"\n\t/// "))
					.arg(slotTypes[ii])
			);
			ctorCont.append(QString(
				"\tParameteredObject::_addOutputSlot(\n\t\t"
				"%1, \"%1\",\n\t\t\"%2\",\n\t\t\"%3\");\n")
				.arg(slotNames[ii])
				.arg(breakLines(slotDocs[ii],"\"\n\t\t\""))
				.arg(slotTypes[ii])
			);
		}
	}

	// set up parameter declaration/registration
	if(field("paramNames").toStringList().size() > 0) {
		const QStringList& paramNames =
				field("paramNames").toStringList();
		const QStringList& paramDocs =
				field("paramDocs").toStringList();
		const QStringList& paramTypes =
				field("paramTypes").toStringList();
		const QStringList& paramDefaults =
				field("paramDefaults").toStringList();
		const QList<bool>& paramLists = ParamSlotModel::toBoolList(
				field("paramLists").toStringList());

		Q_ASSERT(paramNames.size() == paramDocs.size());
		Q_ASSERT(paramNames.size() == paramTypes.size());
		Q_ASSERT(paramNames.size() == paramDefaults.size());
		Q_ASSERT(paramNames.size() == paramLists.size());

		paramSlots.append("\n");
		ctorCont.append("\n");
		for(int ii=0; ii<paramNames.size(); ii++) {
			paramSlots.append(
				QString("\t/// %2\n\t%4<%3> %1;\n")
					.arg(paramNames[ii])
					.arg(breakLines(paramDocs[ii],"\n\t/// "))
					.arg(paramTypes[ii])
					.arg(paramLists[ii]?"ParameterList":"Parameter")
			);
			QString tempTypeSpec;
			QString devVal;
			if(!paramDefaults[ii].isEmpty()) {
				if (paramLists[ii]) {
					ctorAdd.append(QString(",\n\t\t%1(\"%2\")")
						.arg(paramNames[ii])
						.arg(paramDefaults[ii]));
				}
				else {
					if(paramTypes[ii].trimmed() == "std::string") {
						tempTypeSpec = QString("<%1>").arg(paramTypes[ii]);
						devVal = QString("\"%1\", ").arg(paramDefaults[ii]);
					}
					else
						devVal = QString("%1, ").arg(paramDefaults[ii]);
				}
			}
			ctorCont.append(QString(
				"\tParameteredObject::_addParameter%3(\n\t\t"
				"%1, \"%1\",\n\t\t\"%2\",\n\t\t%4\"%5\");\n")
				.arg(paramNames[ii])
				.arg(breakLines(paramDocs[ii],"\"\n\t\t\""))
				.arg(tempTypeSpec)
				.arg(devVal)
				.arg(paramTypes[ii])
			);
		}
	}

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
	txt.replace("@useCImg@",    field("useCImg" ).toBool() ? "1" : "0");
	txt.replace("@useVigra@",   field("useVigra").toBool() ? "1" : "0");

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
