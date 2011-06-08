/*  Copyright (C) 2009 Jens-Malte Gottfried

	This file is part of Tuchulcha.

	Tuchulcha is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Tuchulcha is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file DocGenerator.cpp
 *  \brief Implementation of class DocGenerator
 *  \date   26.11.2008
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "DocGenerator.h"
#include <QUrl>
#include <QDir>
#include <QFile>
#include <QtDebug>
#include <QTextBrowser>

#include "ParameterFileModel.h"
#include "FileManager.h"
#include "MetaData.h"

DocGenerator::DocGenerator(QTextBrowser* viewer, QObject* myParent) :
		QObject(myParent),
		_viewer(viewer),
		_meta(0) {
	Q_ASSERT(_viewer);

	updateMetaData() ;

	// load stylesheet
	QFile stylesheetFile(":/help/styles.css");
	if (stylesheetFile.open(QIODevice::ReadOnly | QIODevice::Text))
		_stylesheet = stylesheetFile.readAll();
	else
		qWarning() << "Stylesheet could not be loaded.";
	stylesheetFile.close();


	// load footer
	QFile footerFile(":/help/footer.txt");
	if (footerFile.open(QIODevice::ReadOnly | QIODevice::Text))
		_footer = footerFile.readAll();
	else
		qWarning() << "Footer file could not be loaded.";
	footerFile.close();
}

DocGenerator::~DocGenerator() {
	delete _meta ;
}

void DocGenerator::showHelp() {
	showDocPage(":/help/help.txt");
}

void DocGenerator::showIntro() {
	showDocPage(":/help/start.txt");
}

void DocGenerator::showDocPage(const QString& fileName) {
	_helpDoc = "";

	// to be able to load files from the resource system,
	// we have to load it and pass to setHtml and not to use
	// _viewer->load()
	QFile docPage(fileName);
	if (docPage.open(QIODevice::ReadOnly | QIODevice::Text))
		_viewer->setHtml(QString(
		"<html><head>"
		"<meta name=\"qrichtext\" content=\"1\"/>"
		"<style type=\"text/css\">%1</style>"
		"</head><body>%2\n%3</body></html>")
		.arg(_stylesheet)
		.arg(QString(docPage.readAll())).arg(_footer));
	else
		qWarning() << tr("DocPage file %1 could not be loaded.")
			.arg(fileName);
	docPage.close();
}

void DocGenerator::showDocString(const QString& doc) {
	// check if content update needed
	if (doc == _helpDoc)
		return;

	// encapsulation into html document (using the stylesheet)
	QString html = QString(
		"<html><head>"
		"<meta name=\"qrichtext\" content=\"1\"/>"
		"<style type=\"text/css\">%1</style>"
		"</head><body><div><p>%2</p></div>\n%3</body></html>")
		.arg(_stylesheet).arg(_helpDoc=doc).arg(_footer);

	// display
	_viewer->setHtml(html);
}

void DocGenerator::updateMetaData() {
	delete _meta ;
	_meta = 0 ;
	_meta = new MetaData(FileManager::instance().classesFile()) ;
}

QString DocGenerator::_docList(QStringList parList,
	QString className, QString slotType) const {
	QString ret;
	QStringList::const_iterator parIter;

	if(parList.size()) {
		ret += "<p><ul>";
		for(parIter=parList.begin(); parIter!=parList.end(); parIter++) {
			ret += "<tr>";

			// show parameter name and type
			ret += "<td class=\"leftcol\"></td>";
			QString parType = _meta->getType(*parIter, className);
			if (parType.contains(QRegExp("^\\s*\\{\\s*\\w.*\\}\\s*$")))
				parType = "Selection";
			parType.replace("<", "&lt;").replace(">", "&gt;");
			ret += QString("<td class=\"dtype firstrow\">%1</td>")
				.arg(parType);
			ret += QString("<td class=\"firstrow\"><span class=\"parname\">"
				"%1</span></td>\n")
				.arg(*parIter);

			// show default value, if any
			QString def = _meta->getDefault(*parIter, className);
			ret += "<td class=\"firstrow\">";

			if(!def.isEmpty())
				ret += tr("<em>(default: \"%1\")</em>").arg(def);

			// show slot flags (if necessary)
			QString flags;
			if (slotType == "in") {
				if(_meta->isMultiSlot(*parIter, className))
					flags += "MultiSlot";
				if(_meta->isOptionalSlot(*parIter, className)) {
					if(!flags.isEmpty())
						flags += "; ";
					flags += "optional";
				}
			}

			if (slotType == "out") {
				if(!_meta->isMultiSlot(*parIter, className))
					flags += "SingleSlot";
				if(!_meta->isOptionalSlot(*parIter, className)) {
					if(!flags.isEmpty())
						flags += "; ";
					flags += "required";
				}
			}
			if(!flags.isEmpty())
				ret += tr("<em>(%1)</em>").arg(flags);

			ret += "</td></tr>";

			// show documentation
			ret += "<tr><td class=\"leftcol\"></td><td></td>";
			ret += QString("<td colspan=\"2\">%1</td></tr><tr></tr>")
				.arg(_meta->getDocString(*parIter, className));
		}
	}
	else
		ret += "<tr><td class=\"leftcol\"></td>"
			   "<td colspan=\"3\">" + tr("(none)") + "</td></tr>\n";
	return ret;
}

void DocGenerator::showClassDoc(const QString& className) {
	QString page =
		tr("<h1>ClassDocumentation: %1</h1>\n").arg(className)
		+ tr("<h2>Description</h2>\n")
		+ QString("<p>%1</p>\n")
		.arg(_meta->getDocString("", className));

	QString docFileName = _meta->getDocFile("", className);

	if(!docFileName.isEmpty()) {
		// load docfile and add content
		QFile docFile(QDir::toNativeSeparators(FileManager::instance()
				.configDir().absoluteFilePath(docFileName)));
		if (docFile.open(QIODevice::ReadOnly | QIODevice::Text))
			page += docFile.readAll();
		else
			qWarning() << tr("Documentation file %1 could not be loaded.")
				.arg(docFileName);
		docFile.close();
	}

	QStringList parList;

	page += "<table class=\"parlist\">";

	// input slots
	parList = _meta->getInputs(className);
	page += "<tr><td colspan=\"5\"><h2>" + tr("input slots")
		+ "</h2></td></tr>\n";
	page += _docList(parList, className, "in");

	// output slots
	parList = _meta->getOutputs(className);
	page += "<tr><td colspan=\"5\"><h2>" + tr("output slots")
		+ "</h2></td></tr>\n";
	page += _docList(parList, className, "out");

	// parameters
	parList = _meta->getParameters(className);
	page += "<tr><td colspan=\"5\"><h2>"+tr("parameters")+"</h2></td></tr>\n";
	page += _docList(parList, className);

	page += "</table>";

	showDocString(page);
}

#include "DocGenerator.moc"
