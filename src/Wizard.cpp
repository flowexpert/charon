#include "Wizard.h"
#include <QDir>
#include <QDate>
#include <QSettings>
#include <QMessageBox>
#include <QTextStream>
#include <QStatusBar>
#include "WizardPageStart.h"
#include "WizardPageMetadata.h"
#include "WizardPageSlots.h"
#include "WizardPageParameters.h"
#include "WizardPageSummary.h"
#include "SideWidget.h"
#include "StatusEventFilter.h"

Wizard::Wizard(QWidget* p) :
		QWizard(p) {
	StatusEventFilter* filter = new StatusEventFilter(this);
	installEventFilter(filter);
	addPage(new WizardPageStart(this));
	addPage(new WizardPageMetadata(this));
	addPage(new WizardPageSlots(this));
	addPage(new WizardPageParameters(this));
	addPage(new WizardPageSummary(this));

	SideWidget* sWidget = new SideWidget(this);
	setSideWidget(sWidget);
	const QList<int>& pId = pageIds();
	for(int ii=0; ii < pId.size(); ii++) {
		sWidget->addItem(page(pId[ii])->title());
	}
	connect(filter, SIGNAL(statusMessage(QString)),
			sWidget, SLOT(showMessage(QString)));

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

	// generate header and source files
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

	// generate cmake project files
	if (res && field("genCMake").toBool()) {
		if(field("headerSeparate").toBool()) {
			QMessageBox::warning(
					this,tr("unimplemented"),
					tr("CMake project file generation for "
						"split header/src configuration "
						"not yet implemented."));
			return false;
		}
		QString cMakeOut(field("cmakeFileOut").toString());
		if (QFile::exists(cMakeOut)) {
			// modify existing CMakeLists.txt to append the new module
			_updateCMakeFile();
		}
		else {
			// generate new CMakeLists.txt and copy needed files
			res = res &&
				_replacePlaceholders(
					":/templates/CMakeLists.txt",
					cMakeOut);
			QStringList ll;
			ll << "InitFlags.cmake";
			ll << "CImgConfig.cmake";
			for (int ii=0; ii < ll.size(); ii++){
				QFile fin(QString(":/templates/%1").arg(ll[ii]));
				QFile fout(QString("%1/%2")
								.arg(field("commonOut").toString())
								.arg(ll[ii]));
				fin.copy(fout.fileName());
				fout.setPermissions(fin.permissions() | QFile::WriteUser);
			}
		}
	}
	return res;
}

void Wizard::_updateCMakeFile() {
	QFile cMakeOut(field("cmakeFileOut").toString());
	QString modName = field("name").toString().trimmed();
	QSettings c(":/config/config.ini", QSettings::IniFormat);
	c.beginGroup("CMake");
	if (QMessageBox::question(
			this, tr("Append Module?"),
			tr("An existing <tt>CMakeLists.txt</tt> "
				"has been found.<br>"
				"Please inspect the file after adding a new module, "
				"Changing some options or e.g. the project "
				"title could be useful.<br><br>"
				"Append new module <em>%1<em>?")
			.arg(modName),
			QMessageBox::Yes, QMessageBox::No) ==
				QMessageBox::Yes) {
		QString coStr;
		if (cMakeOut.open(QFile::ReadOnly | QIODevice::Text)) {
			QTextStream coStrm(&cMakeOut);
			coStr = coStrm.readAll();
			cMakeOut.close();
		}
		else {
			QMessageBox::warning(
					this, tr("I/O error"),
					tr("Error reading <tt>%1</tt>")
						.arg(cMakeOut.fileName()));
			return;
		}
		int pp = coStr.indexOf("# add additional modules here");
		if (pp < 0) {
			QMessageBox::warning(
					this, tr("placeholder missing"),
					tr("Placeholder for new module data "
						"not found.<br>"
						"Skipping add."));
			return;
		}
		if (coStr.indexOf(QRegExp(
				c.value("findModRgx").toString().arg(modName))) >= 0) {
			QMessageBox::warning(
					this, tr("module found"),
					tr("Some CMake object called <em>%1</em> "
						"has been found.<br>"
						"Skipping add.").arg(modName));
			return;
		}
		QString newMod = c.value("newModStr").toString()
				.arg(modName)
				.arg(modName.toLower())
				.arg(field("templated").toBool()?
					QString("\t%1.hxx\n").arg(modName) : "");

		coStr.insert(pp, newMod);

		if (cMakeOut.open(QFile::WriteOnly| QIODevice::Text |
							QIODevice::Truncate)) {
			QTextStream coStrm(&cMakeOut);
			coStrm << coStr;
			cMakeOut.close();
		}
		else {
			QMessageBox::warning(
					this, tr("I/O error"),
					tr("Error writing <tt>%1</tt>")
						.arg(cMakeOut.fileName()));
			return;
		}
	}
}

QString Wizard::_cppTypeLookup(QString type, QString prefix) {
	QSettings s(":/config/cppTypeMap.ini", QSettings::IniFormat);
	if (s.contains(type)) {
		return s.value(type).toString();
	}
	else if (s.contains(prefix+"/"+type)) {
		return s.value(prefix+"/"+type).toString();
	}
	else if (prefix == "Parameters") {
		return "std::string";
	}
	else {
		return type;
	}
}

bool Wizard::_replacePlaceholders(QString src, QString dst) {
	if (QFile::exists(dst)) {
		QMessageBox::warning(this, tr("output file exists"),
				tr("File <tt>%1</tt> exists. Write failed.").arg(dst));
		return false;
	}
	QSettings c(":/config/config.ini", QSettings::IniFormat);
	c.beginGroup("PlaceHolderCont");

	// load template file from src
	QFile srcFile(src);
	if (!srcFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, tr("template read failed"),
				tr("Failed to read from <tt>%1</tt>.").arg(src));
		return false;
	}
	QTextStream srcStrm(&srcFile);
	QString txt = srcStrm.readAll();

	// setup fill texts - name, date, doc
	QString author = field("author").toString().trimmed();
	QString authoremail = c.value("authorEmail").toString()
			.arg(author).arg(field("email").toString().trimmed());
	QString name = field("name").toString().trimmed();
	QString doxyDoc = c.value("doxyDoc").toString()
			.arg(breakLines(
					field("briefDesc").toString(),
					c.value("doxyDocBSep").toString()))
			.arg(breakLines(
					field("description").toString(),
					c.value("doxyDocDSep").toString()));
	QString pluginDoc = c.value("pluginDoc").toString()
			.arg(field("briefDesc").toString().trimmed())
			.arg(field("description").toString().trimmed());
	pluginDoc = breakLines(
			pluginDoc,
			c.value("pluginDocSep").toString(),
			c.value("pluginDocDlm").toString());
	QString addHeaders;
	if(field("useVigra").toBool())
		addHeaders += c.value("includeVigra").toString();
	if(field("useCImg").toBool())
		addHeaders += c.value("includeCImg").toString();

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
			QString cppType = _cppTypeLookup(slotTypes[ii], "Slots");
			paramSlots.append(c.value("inputSlotDecl").toString()
					.arg(slotNames[ii])
					.arg(breakLines(
							slotDocs[ii],c.value("memDocSep").toString()))
					.arg(cppType)
			);
			ctorCont.append(c.value("inputSlotReg").toString()
				.arg(slotNames[ii])
				.arg(breakLines(
						slotDocs[ii],c.value("regDocSep").toString()))
				.arg(slotTypes[ii])
			);
			if (slotOptional[ii] || slotMulti[ii]) {
				ctorAdd.append(c.value("inputSlotOptMulti").toString()
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
			QString cppType = _cppTypeLookup(slotTypes[ii], "Slots");
			paramSlots.append(c.value("outputSlotDecl").toString()
					.arg(slotNames[ii])
					.arg(breakLines(
							slotDocs[ii],c.value("memDocSep").toString()))
					.arg(cppType)
			);
			ctorCont.append(c.value("outputSlotReg").toString()
				.arg(slotNames[ii])
				.arg(breakLines(
						slotDocs[ii],c.value("regDocSep").toString()))
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
			QString cppType = _cppTypeLookup(paramTypes[ii], "Parameters");
			paramSlots.append(
				c.value("parameterDecl").toString()
					.arg(paramNames[ii])
					.arg(breakLines(
							paramDocs[ii],c.value("memDocSep").toString()))
					.arg(cppType)
					.arg(paramLists[ii]?"ParameterList":"Parameter")
			);
			QString tempTypeSpec;
			QString devVal;
			if(!paramDefaults[ii].isEmpty()) {
				if (paramLists[ii]) {
					ctorAdd.append(c.value("paramListCtor").toString()
						.arg(paramNames[ii])
						.arg(paramDefaults[ii]));
				}
				else {
					tempTypeSpec = QString("< %1 >").arg(cppType);
					devVal = QString("%1, ").arg(paramDefaults[ii]);

					// string values encapsulated into ""
					if (cppType.contains(QRegExp(
							c.value("parameterListString").toString()))) {
						devVal = QString("\"%1\", ").arg(paramDefaults[ii]);
					}
				}
			}
			ctorCont.append(c.value("parameterReg").toString()
				.arg(paramNames[ii])
				.arg(breakLines(
						paramDocs[ii], c.value("regDocSep").toString()))
				.arg(tempTypeSpec)
				.arg(devVal)
				.arg(paramTypes[ii])
			);
		}
	}

	c.endGroup();
	c.beginGroup("PlaceHolders");

	// replace placeholders
	txt.replace(c.value("author").toString(),
				author);
	txt.replace(c.value("authorEmail").toString(),
				authoremail);
	txt.replace(c.value("name").toString(),
				name, Qt::CaseSensitive);
	txt.replace(c.value("nameLower").toString(),
				name.toLower(), Qt::CaseSensitive);
	txt.replace(c.value("nameUpper").toString(),
				name.toUpper(), Qt::CaseSensitive);
	txt.replace(c.value("year").toString(),
				QDate::currentDate().toString("yyyy"));
	txt.replace(c.value("date").toString(),
				QDate::currentDate().toString("dd.MM.yyyy"));
	txt.replace(c.value("doxyDoc").toString(),
				doxyDoc);
	txt.replace(c.value("pluginDoc").toString(),
				pluginDoc);
	txt.replace(c.value("headers").toString(),
				addHeaders);
	txt.replace(c.value("paramSlots").toString(),
				paramSlots);
	txt.replace(c.value("ctorAdd").toString(),
				ctorAdd);
	txt.replace(c.value("ctorCont").toString(),
				ctorCont);
	txt.replace(c.value("useCImg").toString(),
				field("useCImg" ).toBool() ? "1" : "0");
	txt.replace(c.value("useVigra").toString(),
				field("useVigra").toBool() ? "1" : "0");
	txt.replace(c.value("modHxx").toString(),
				field("templated").toBool() ?
					c.value("modHxxStr").toString().arg(name) : "");

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

	c.endGroup();
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
