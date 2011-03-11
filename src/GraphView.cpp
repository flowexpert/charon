/*	Copyright (C) 2009 Jens-Malte Gottfried

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
/**	@file	GraphView.cpp
 *	@brief	Implementation of class GraphView
 *
 *	@date	30.10.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "GraphView.h"
#include "GraphModel.h"
#include "GraphBuilder.h"
#include "FileManager.h"

#include <QMouseEvent>
#include <QDir>
#include <QFileDialog>
#include <QGraphicsItem>
#include <QMessageBox>

#include <graphviz/gvc.h>

#include "GraphView.moc"

GraphView::GraphView(QWidget* myparent, QString classesFile) :
        QGraphicsView(myparent),
        _model(0),
        _builder(0),
        _ownModel(true) {
    // init graphics scene
    setScene(new QGraphicsScene(this));

    // this enables scrolling by hand drag
    setDragMode(QGraphicsView::ScrollHandDrag);

    // init model
    if (classesFile.isEmpty())
        classesFile = FileManager::instance().classesFile();

    _model	= new GraphModel("", this, classesFile);

    // init graphviz
    _gvc = gvContext();
}

GraphView::~GraphView() {
    gvFreeContext(_gvc);
    if(_ownModel)
        delete _model;
}

bool GraphView::load(const QString& fileName) throw() {
	QString errorMSG;
	bool ok = true;
	try {
		ok = _model->load(fileName);
	}
	catch (const std::string& msg) {
		errorMSG = msg.c_str();
	}
	catch (const std::exception& err) {
		errorMSG = err.what();
	}
	catch (...) {
		errorMSG = tr("Got unhandled and unexpected exception during load!");
	}
	if (!errorMSG.isEmpty()) {
		if (errorMSG.indexOf("neither input nor output slot") >= 0) {
			errorMSG += tr(
				"<br><br>"
				"This is usually a hint "
				"that there is a mismatch of your parameter file "
				"and the actual plugin information. "
				"This may be caused by changed slots or typos in the "
				"parameter file.<br><br>"
				"Please update the plugin information and check if all "
				"plugins used in the parameter file to load have been "
				"successfully recognized.<br>"
				"Make sure that the path where the plugin is loaded from "
				"is the path you expect. Perhaps an older version of the "
				"plugin has been found in the global plugin path or "
				"something similar happened."
			);
		}
		QRegExp regex("^Parameter\\s(\\w*)\\.parameters\\snot\\sset\\s*$");
		if (errorMSG.indexOf(regex) >= 0) {
			errorMSG += tr(
				"<br><br>"
				"This is usually a hint that you are trying to use a "
				"plugin unknown to tuchulcha (<b>%1</b>).<br><br>"
				"Please check your path configuration and "
				"update the plugin information.<br>"
				"Check the update log and make sure the <em>%1</em> plugin "
				"has been successfully found and loaded."
			).arg(regex.cap(1));
		}
		QMessageBox::warning(
			this,"Error loading parameter file",
			tr("Message: %1").arg(errorMSG));
		return false;
	}
	return ok;
}

GraphModel* GraphView::model() {
    return _model;
}

const GraphModel* GraphView::model() const {
    return _model;
}

void GraphView::setModel(GraphModel* gModel) {
    if(gModel == _model)
        return;

    if(gModel) {
        // delete model if class has ownership
        if(_ownModel)
            delete _model;
        _ownModel = false;
    }
    else {
        // create empty model
        gModel = new GraphModel("", this,
            FileManager::instance().classesFile());
        // delete old model if class has ownership
        if(_ownModel)
            delete _model;
        _ownModel = true;
    }

    // commit changes
    _model = gModel;
    Q_ASSERT(_model);

    // update display and connections
    _builder->setModel(_model);

    emit modelChanged(_model);
}

void GraphView::updateDisplay() {
    _renderGraph(_builder->graph());
}

void GraphView::mousePressEvent(QMouseEvent* mouseEvent) {
    QGraphicsRectItem* source =
        qgraphicsitem_cast<QGraphicsRectItem*>(itemAt(mouseEvent->pos()));

    if (!source) {
        // original handling
        QGraphicsView::mousePressEvent(mouseEvent);
    }

    if ((mouseEvent->button() == Qt::LeftButton) &&
            (mouseEvent->buttons() & Qt::LeftButton)) {

        // save mouse position to detect drags
        _dragStartPosition = mouseEvent->pos();
    }
}

void GraphView::save() const {
	QString fileName = QFileDialog::getSaveFileName(
			0, tr("Save flowchart"), QDir::currentPath(),
			tr("graphic files (*.png *.svg *.pdf *.dot)"));

    if(fileName.isEmpty())
        return;

    // avoid saving highlighted selection
    QString prefOld = _model->prefix();
    _model->setPrefix("");

    // render graph and save it to the selected file
    gvLayout(_gvc, _builder->graph(), const_cast<char*> ("dot"));
	QStringList fileNameSplit = fileName.split(".");
	QString extension = fileNameSplit.last();
	extension.toLower();
	bool ok = true;
    if (extension == "png")
        gvRenderFilename(_gvc, _builder->graph(), const_cast<char*>("png"),
                const_cast<char*> (fileName.toAscii().constData()));
    else if (extension == "svg")
        gvRenderFilename(_gvc, _builder->graph(), const_cast<char*>("svg"),
                const_cast<char*> (fileName.toAscii().constData()));
	else if (extension == "pdf")
        gvRenderFilename(_gvc, _builder->graph(), const_cast<char*>("pdf"),
                const_cast<char*> (fileName.toAscii().constData()));
	else if (extension == "dot")
		gvRenderFilename(_gvc, _builder->graph(), const_cast<char*>("dot"),
				const_cast<char*> (fileName.toAscii().constData()));
	else {
		ok = false;
		QMessageBox::warning(
				0, tr("Error generating graphics file"),
				tr("Unknown file extension: %1<br>"
					"Supported: svg, png, pdf").arg(extension));
	}

    gvFreeLayout(_gvc, _builder->graph());

    // restore prefix
    _model->setPrefix(prefOld);

    // status message
	emit statusMessage(ok ? tr("Saved graphic to file %1").arg(fileName) :
		tr("Error generating file %1").arg(fileName), 5000);
}
