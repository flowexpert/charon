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
/**	@file	TemplateGraphView.cpp
 *	@brief	Implementation of class TemplateGraphView
 *
 *	@date	21.11.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "TemplateGraphView.h"
#include "TemplateGraphBuilder.h"
#include "GraphModel.h"
#include "MetaData.h"
#include "imageManip.h"
#include "Qtiostream.h"
#include "FileManager.h"

#include <QMouseEvent>
#include <QGraphicsSvgItem>
#include <QApplication>
#include <QFile>
#include <graphviz/gvc.h>

#include <sstream>

#include "TemplateGraphView.moc"

TemplateGraphView::TemplateGraphView(QWidget* myParent, QString classesFile) :
    GraphView(myParent, classesFile) {

    // set up graph creation
    _builder = new TemplateGraphBuilder(_model);
    connect(_builder, SIGNAL(graphChanged()),
        this, SLOT(updateDisplay()));

    scale(1.6, 1.6);
}

TemplateGraphView::~TemplateGraphView() {
}

void TemplateGraphView::_renderGraph(graph_t* graph) {
    Q_ASSERT(scene());

    scene()->clear();

    // stop rendering if graph not avaiable or empty.
    if(!graph || !agfstnode(graph))
        return;

    QStringList tempnames;

    // render graph and save it to some temporary files
    gvLayout(_gvc, graph, const_cast<char*> ("dot"));

    QString dotfile = FileManager::instance().tempFileName();

    Q_ASSERT(!QFile(dotfile).exists());
    std::string dotfileStr = dotfile.toAscii().constData();
    gvRenderFilename(_gvc, graph, const_cast<char*> ("xdot"),
        const_cast<char*> (dotfileStr.c_str()));
    Q_ASSERT(QFile(dotfile).exists());
    tempnames.push_back(dotfile);

    QString svgfile = FileManager::instance().tempFileName();

    Q_ASSERT(!QFile(svgfile).exists());
    std::string svgfileStr = svgfile.toAscii().constData();
    gvRenderFilename(_gvc, graph, const_cast<char*>("svg"),
        const_cast<char*>(svgfileStr.c_str()));
    Q_ASSERT(QFile(svgfile).exists());
    tempnames.push_back(svgfile);

    gvFreeLayout(_gvc, graph);

    // display graph
    QGraphicsSvgItem* svgImage = new QGraphicsSvgItem(svgfile);
    scene()->addItem(svgImage);
    QRectF imBB = svgImage->boundingRect();

    // get graph bounding box
    char* ret = agget(graph, const_cast<char*>("bb"));
    std::istringstream bbStrm(ret);
    QRectF bb;
    bbStrm >> bb;

    // calculate scaling
    double sx, sy;
    sx = imBB.width()/bb.width();
    sy = imBB.height()/bb.height();

    // calculate transformation matrix
    QTransform transformation;
    transformation.scale(sx, -sy);
    transformation.translate(0, -bb.height());

    // iterate over all avaiable nodes
    for(node_t* cur  = agfstnode(graph); cur; cur = agnxtnode(graph, cur)) {
        ret = agget(cur, const_cast<char*>("rects"));
        Q_ASSERT(ret);
        std::istringstream rectsStrm(ret);
        QRectF bounding;

        // get rects
        QList<QRectF> rectList;

        // all rectangles (name, class and slots)
        while(rectsStrm.good()) {
            rectsStrm >> bounding;
            rectList  << bounding;
        }

        // bounding box of all rectangles
        bounding = imageManip::boundingRect(rectList);

        // add mouse sensitive areas
        QGraphicsRectItem* rectItem = new QGraphicsRectItem(bounding);

        /**	Content of object template bounding rects:
         *	- data(0): className
         *	- data(1): "dummy"
         */
        QString className = _model->getClass(cur->name).c_str();

        // store node name
        rectItem->setData(0, className);
        rectItem->setData(1, "dummy");
        rectItem->setPen(QColor(Qt::transparent));
        rectItem->setTransform(transformation);
        rectItem->setZValue(5);

        //	add to graphics scene
        scene()->addItem(rectItem);
    }

    // set new bounding rect
    scene()->setSceneRect(imBB);

    // remove temporary files
    bool res = false;
    QStringList::const_iterator curFile;
    for(curFile = tempnames.begin(); curFile != tempnames.end(); curFile++) {
        QFile tempfile(*curFile);
        Q_ASSERT(tempfile.exists());
        res = tempfile.remove();
        Q_ASSERT(res);
    }
}

void TemplateGraphView::mousePressEvent(QMouseEvent* mEvent) {
    GraphView::mousePressEvent(mEvent);

    // show doc, if item has been clicked
    if (mEvent->button() == Qt::LeftButton) {
        QGraphicsRectItem* clicked =
                qgraphicsitem_cast<QGraphicsRectItem*>
                (itemAt(mEvent->pos()));
        if(clicked)
            emit(showClassDoc(clicked->data(0).toString()));
    }
}

void TemplateGraphView::mouseMoveEvent(QMouseEvent* mEvent) {
    if ((mEvent->buttons() & Qt::LeftButton) &&
            ((mEvent->pos()-_dragStartPosition).manhattanLength()
                >= QApplication::startDragDistance())) {

        QGraphicsRectItem* source =
            qgraphicsitem_cast<QGraphicsRectItem*>
            (itemAt(_dragStartPosition));

        if (!source) {
            QGraphicsView::mouseMoveEvent(mEvent);
            return;
        }

        // start drag
        QDrag* drag = new QDrag(this);
        QMimeData* pmime = new QMimeData;
        pmime->setData("application/x-paramnode",
            QByteArray::number(reinterpret_cast<quintptr>(source)));
        drag->setMimeData(pmime);

        // pixmap to display information during drag
        QPixmap pixmap(220, 50);
        pixmap.fill();
        QPainter painter(&pixmap);
        painter.setPen(QPen(QColor(128, 128, 255), 3, Qt::SolidLine,
            Qt::RoundCap, Qt::RoundJoin));
        painter.setRenderHint(QPainter::Antialiasing);
        painter.drawRect(3, 3, 214, 44);
        painter.setPen(Qt::black);
        painter.drawText(10,  5, 140, 20, Qt::AlignLeft | Qt::AlignVCenter,
            "Add a new node");
        painter.drawText(10, 25,  70, 20, Qt::AlignLeft | Qt::AlignVCenter,
            "class:");
        painter.setPen(Qt::blue);
        painter.drawText(80, 25, 140, 20, Qt::AlignLeft | Qt::AlignVCenter,
            source->data(0).toString());
        drag->setPixmap(pixmap);

        // colorize starting slot (blue)
        source->setBrush(QColor(0, 0, 255, 90));
        source->update();
        drag->exec();

        // items could now have been updated
        QAbstractGraphicsShapeItem* sourceN =
            qgraphicsitem_cast<QAbstractGraphicsShapeItem*>
            (itemAt(_dragStartPosition));

        if(source == sourceN) {
            source->setBrush(QColor(Qt::transparent));
            source->update();
        }
    }
    else
        QGraphicsView::mouseMoveEvent(mEvent);
}

void TemplateGraphView::mouseDoubleClickEvent(QMouseEvent* mEvent) {
    if (mEvent->button() == Qt::LeftButton) {

        // add this node to the active flow chart
        QGraphicsRectItem* item = qgraphicsitem_cast<QGraphicsRectItem*>
            (itemAt(mEvent->pos()));

        if (item) {
            emit addNode(item->data(0).toString());
        }
    }
}
