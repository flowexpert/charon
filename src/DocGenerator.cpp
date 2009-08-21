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
/** @file DocGenerator.cpp
 *  @brief Implementation of class DocGenerator
 *
 *  @date	26.11.2008
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "DocGenerator.h"
#include <QUrl>
#include <QDir>
#include <QFile>
#include <QtDebug>
#include <QWebView>

#include "ParameterFileModel.h"
#include "FileManager.h"
#include "MetaData.h"

#include "DocGenerator.moc"

DocGenerator::DocGenerator(QWebView* viewer, QObject* myParent) :
        QObject(myParent),
        _viewer(viewer),
        _model(0) {
    Q_ASSERT(_viewer);

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
}

void DocGenerator::setModel(ParameterFileModel* model) {
    _model = model;
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
        _viewer->setHtml(QString("<html><head><title>%1</title>"
        "<style type=\"text/css\">\n<!--\n%2\n-->\n</style>"
        "</head><body>%3\n%4</body></html>")
        .arg(fileName).arg(_stylesheet)
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
    QString html = QString("<html><head><title>short help</title>"
        "<style type=\"text/css\">\n<!--\n%1\n-->\n</style>"
        "</head><body><div><p>%2</p></div>\n%3</body></html>")
        .arg(_stylesheet).arg(_helpDoc = doc).arg(_footer);

    // display
    _viewer->setHtml(html);
}

QString DocGenerator::_docList(const std::vector<std::string>& parList,
        const std::string& className, const std::string& slotType) const {
    QString ret;
    std::vector<std::string>::const_iterator parIter;

    if(parList.size()) {
        ret += "<p><ul>";
        for(parIter=parList.begin(); parIter!=parList.end(); parIter++) {
            ret += "<tr>";

            // show parameter name and type
            ret += "<td class=\"leftcol\"></td>";
            ret	+= QString("<td class=\"dtype firstrow\">%1</td>")
                .arg(_model->metaInfo()->getType(*parIter, className).c_str());
            ret += QString("<td class=\"firstrow\"><span class=\"parname\">"
                "%1</span></td>\n")
                .arg(parIter->c_str());

            // show default value, if any
            QString def = _model->metaInfo()->
                getDefault(*parIter, className).c_str();
            ret += "<td class=\"firstrow\">";

            if(!def.isEmpty())
                ret += tr("<em>(default: \"%1\")</em>").arg(def);

            // show slot flags (if necessary)
            QString flags;
            if (slotType == "in") {
                if(_model->metaInfo()->isMultiSlot(*parIter, className))
                    flags += "MultiSlot";
                if(_model->metaInfo()->isOptionalSlot(*parIter, className)) {
                    if(!flags.isEmpty())
                        flags += "; ";
                    flags += "optional";
                }
            }

            if (slotType == "out") {
                if(!_model->metaInfo()->isMultiSlot(*parIter, className))
                    flags += "SingleSlot";
                if(!_model->metaInfo()->isOptionalSlot(*parIter, className)) {
                    if(!flags.isEmpty())
                        flags += "; ";
                    flags += "required";
                }
            }
            if(!flags.isEmpty())
                ret += tr("<em>(%1)</em>").arg(flags);

            ret += "</td>\n</tr>";

            // show documentation
            ret += "<tr><td class=\"leftcol\"></td><td></td>";
            ret += QString("<td colspan=\"2\">%1</td></tr>\n")
                .arg(_model->metaInfo()->getDocString(*parIter, className)
                    .c_str());
        }
    }
    else
        ret += "<tr><td class=\"leftcol\"></td>"
               "<td colspan=\"3\">" + tr("(none)") + "</td></tr>\n";

    return ret;
}

void DocGenerator::showClassDoc(const QString& className) {
    Q_ASSERT(_model);
    QString page =
        tr("<h1>ClassDocumentation: %1</h1>\n").arg(className)
        + tr("<h2>Description</h2>\n")
        + QString("<p>%1</p>\n")
        .arg(_model->metaInfo()->
            getDocString("", className.toAscii().constData()).c_str());

    QString docFileName =
        _model->metaInfo()->getDocFile("",
                                      className.toAscii().constData()).c_str();

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

    std::vector<std::string> parList;

    page += "<table class=\"parlist\">";

    // input slots
    parList = _model->metaInfo()->getInputs(className.toAscii().constData());
    page += "<tr><td colspan=\"5\"><h2>" + tr("input slots")
        + "</h2></td></tr>\n";
    page += _docList(parList, className.toAscii().constData(), "in");

    // output slots
    parList = _model->metaInfo()->getOutputs(className.toAscii().constData());
    page += "<tr><td colspan=\"5\"><h2>" + tr("output slots")
        + "</h2></td></tr>\n";
    page += _docList(parList, className.toAscii().constData(), "out");

    // parameters
    parList = _model->metaInfo()->getParameters(className.toAscii().constData());
    page += "<tr><td colspan=\"5\"><h2>"+tr("parameters")+"</h2></td></tr>\n";
    page += _docList(parList, className.toAscii().constData());

    page += "</table>";

    showDocString(page);
}
