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

void GraphView::load(const QString& fileName) {
    _model->load(fileName);
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
    QString fileName = QFileDialog::getSaveFileName(0, tr("Save flowchart"),
            QDir::currentPath(), tr("graphic files (*.png *.svg)"));

    if(fileName.isEmpty())
        return;

    // avoid saving highlighted selection
    QString prefOld = _model->prefix();
    _model->setPrefix("");

    // render graph and save it to the selected file
    gvLayout(_gvc, _builder->graph(), const_cast<char*> ("dot"));
    if (fileName.indexOf("png") > 0)
        gvRenderFilename(_gvc, _builder->graph(), const_cast<char*>("png"),
                const_cast<char*> (fileName.toAscii().constData()));
    else
        gvRenderFilename(_gvc, _builder->graph(), const_cast<char*>("svg"),
                const_cast<char*> (fileName.toAscii().constData()));

    gvFreeLayout(_gvc, _builder->graph());

    // restore prefix
    _model->setPrefix(prefOld);

    // status message
    emit statusMessage(tr("Saved graphic to file %1").arg(fileName), 5000);
}
