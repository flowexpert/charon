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
/**	@file	HierarchyGraphView.cpp
 *	@brief	Implementation of class HierarchyGraphView
 *
 *	@date	21.11.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "HierarchyGraphView.h"
#include "HierarchyGraphBuilder.h"
#include "GraphModel.h"
#include "MetaData.h"
#include "imageManip.h"
#include "Qtiostream.h"
#include "FileManager.h"

#include <QMouseEvent>
#include <QApplication>
#include <QGraphicsSvgItem>
#include <QFile>
#include <graphviz/gvc.h>

#include <sstream>

#include "HierarchyGraphView.moc"

HierarchyGraphView::HierarchyGraphView(QWidget* myParent) :
    GraphView(myParent) {
    // set up graph creation
    _builder = new HierarchyGraphBuilder(_model);
    connect(_builder, SIGNAL(graphChanged()),
        this, SLOT(updateDisplay()));
}

HierarchyGraphView::~HierarchyGraphView() {
}

void HierarchyGraphView::_renderGraph(graph_t* graph) {
    Q_ASSERT(scene());

    scene()->clear();

    // stop rendering if graph not avaiable or empty.
    if(!graph || !agfstnode(graph))
        return;

    QList<QString> tempnames;

    // render graph and save it to some temporary files

    // avoid usage of decimal-"," in svg files
    char* localeBak = setlocale(LC_NUMERIC, "C");
    gvLayout(_gvc, graph, const_cast<char*> ("dot"));

    QString dotfile = FileManager::instance().tempFileName();

    Q_ASSERT(!QFile(dotfile).exists());
    gvRenderFilename(_gvc, graph, const_cast<char*> ("xdot"),
                        const_cast<char*> (dotfile.toAscii().constData()));
    tempnames.push_back(dotfile);
    Q_ASSERT(QFile(dotfile).exists());

    QString svgfile = FileManager::instance().tempFileName();

    Q_ASSERT(!QFile(svgfile).exists());
    gvRenderFilename(_gvc, graph, const_cast<char*>("svg"),
                        const_cast<char*> (svgfile.toAscii().constData()));
    tempnames.push_back(svgfile);
    Q_ASSERT(QFile(svgfile).exists());

    gvFreeLayout(_gvc, graph);

    // resore pevious locale
    setlocale(LC_NUMERIC, localeBak);

    // display graph
    QGraphicsSvgItem* svgImage = new QGraphicsSvgItem(svgfile);
    scene()->addItem(svgImage);
    QRectF imBB = svgImage->boundingRect();

    // find bounding rects of the nodes
    char* ret = 0;

    // get graph bounding box
    ret = agget(graph, const_cast<char*>("bb"));
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

    QString selected = _model->prefix();

    // iterate over all avaiable nodes
    for(node_t* cur  = agfstnode(graph); cur; cur = agnxtnode(graph, cur)) {
        ret = agget(cur, const_cast<char*>("rects"));
        Q_ASSERT(ret);
        std::istringstream rectsStrm(ret);
        QRectF bounding;

        // get rects
        QList<QRectF> rectList;
        QList<QRectF> inputSlots;
        QList<QRectF> outputSlots;

        // top rectangles (class name and object name)
        rectsStrm >> bounding;
        rectList  << bounding;
        rectsStrm >> bounding;
        rectList  << bounding;

        // rectangles for in- and output slots
        while(rectsStrm.good()) {
            rectsStrm   >> bounding;
            inputSlots  << bounding;
            Q_ASSERT(rectsStrm.good());
            rectsStrm   >> bounding;
            outputSlots << bounding;
        }

        // collect all rectangles
        rectList += outputSlots;
        rectList += inputSlots;

        // bounding box of all rectangles
        bounding = imageManip::boundingRect(rectList);

        QList<QAbstractGraphicsShapeItem*> rectItemList;

        // add mouse sensitive areas
        QGraphicsRectItem* rectItem = new QGraphicsRectItem(bounding);
        rectItem->setZValue(5);
        // store as node
        rectItem->setData(1, "main");
        rectItemList << rectItem;

        /**	Content of object bounding rects:
            *	- data(0): object name
            *	- data(1): "main"
            */

        std::string className = _model->getClass(cur->name);

        // add areas for input/output slots
        std::vector<std::string> inputs  =
            _model->metaInfo()->getInputs (className);
        std::vector<std::string> outputs =
            _model->metaInfo()->getOutputs(className);

        /**	Content of slot bounding rects:
            *	- data(0): object name
            *	- data(1): "input" / "output" (depends on slot type)
            *	- data(2): input/output slot name
            *	- data(3): type name
            */

        // inputs
        for(uint i=0; i < inputs.size(); i++) {
            QGraphicsRectItem* slotRect = new QGraphicsRectItem(inputSlots[i]);
            slotRect->setZValue(10);
            // store slot information
            slotRect->setData(1, "input");
            slotRect->setData(2, inputs[i].c_str());
            slotRect->setData(3, _model->metaInfo()->getType(
                std::string(cur->name) + "." + inputs[i], className).c_str());
            rectItemList << slotRect;
        }

        // outputs
        for(uint i=0; i < outputs.size(); i++) {
            QGraphicsRectItem* slotRect = new QGraphicsRectItem(outputSlots[i]);
            slotRect->setZValue(10);
            // store slot information
            slotRect->setData(1, "output");
            slotRect->setData(2, outputs[i].c_str());
            slotRect->setData(3, _model->metaInfo()->getType(
                std::string(cur->name) + "." + outputs[i], className).c_str());
            rectItemList << slotRect;
        }

        // common code
        QList<QAbstractGraphicsShapeItem*>::const_iterator rlIter;
        for(rlIter = rectItemList.begin(); rlIter != rectItemList.end();
                rlIter++) {
            // store node name
            (*rlIter)->setData(0, cur->name);
            (*rlIter)->setPen(QColor(Qt::transparent));
            (*rlIter)->setTransform(transformation);

            //	add to graphics scene
            scene()->addItem(*rlIter);
        }
    }

    // set new bounding rect
    scene()->setSceneRect(imBB);

    // remove temporary files
    bool res = false;
    QList<QString>::const_iterator curFile;
    for(curFile=tempnames.begin();curFile!=tempnames.end();curFile++) {
        QFile tempfile(*curFile);
        Q_ASSERT(tempfile.exists());
        res = tempfile.remove();
        Q_ASSERT(res);
    }
}

void HierarchyGraphView::_highlight(QAbstractGraphicsShapeItem* item, QColor colour) {
    colour.setAlphaF(0.5);
    item->setBrush(colour);
    item->update();
    _highlighted << item;
}

void HierarchyGraphView::_clearHighlight() {
    QList<QAbstractGraphicsShapeItem*>::iterator item;
    for(item = _highlighted.begin(); item != _highlighted.end(); item++) {
        (*item)->setBrush(QColor(Qt::transparent));
        (*item)->update();
    }
    _highlighted.clear();
}

void HierarchyGraphView::mousePressEvent(QMouseEvent* mEvent) {
    GraphView::mousePressEvent(mEvent);

    // handle selection
    if ((mEvent->button() == Qt::LeftButton) &&
        (mEvent->buttons() & Qt::LeftButton) && _model) {

        // select this item
        QGraphicsItem* item = itemAt(mEvent->pos());
        item = qgraphicsitem_cast<QGraphicsRectItem*>(item);

        if (item) {
            QString nodename = item->data(0).toString();
            _model->setPrefix(nodename);
            return;
        }
    }
}

void HierarchyGraphView::mouseDoubleClickEvent(QMouseEvent* mEvent) {
    if((mEvent->button() == Qt::LeftButton) && _model) {
        // rename this node
        QGraphicsRectItem* item = qgraphicsitem_cast<QGraphicsRectItem*>
            (itemAt(mEvent->pos()));

        if (item) {
            QString nodename = item->data(0).toString();
            if (!nodename.isEmpty())
                _model->renameNode(nodename);
        }
    }
}

void HierarchyGraphView::mouseMoveEvent(QMouseEvent* mEvent) {
    if ((mEvent->buttons() & Qt::LeftButton) &&
            ((mEvent->pos()-_dragStartPosition).manhattanLength()
                >= QApplication::startDragDistance())) {

        QAbstractGraphicsShapeItem* source =
            qgraphicsitem_cast<QAbstractGraphicsShapeItem*>
            (itemAt(_dragStartPosition));

        if (!source ||
                !((source->data(1) == "input") ||
                    (source->data(1) == "output"))) {
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
        QPixmap pixmap(220, 70);
        pixmap.fill();
        QPainter painter(&pixmap);
        painter.setPen(QPen(QColor(128, 128, 255), 3, Qt::SolidLine,
            Qt::RoundCap, Qt::RoundJoin));
        painter.setRenderHint(QPainter::Antialiasing);
        painter.drawRect(3, 3, 214, 64);
        painter.setPen(Qt::black);
        painter.drawText(10,  5,  70, 20, Qt::AlignLeft | Qt::AlignVCenter,
            "source:");
        painter.drawText(10, 25,  70, 20, Qt::AlignLeft | Qt::AlignVCenter,
            "slot:");
        painter.drawText(10, 45,  70, 20, Qt::AlignLeft | Qt::AlignVCenter,
            "type:");
        painter.setPen(Qt::blue);
        painter.drawText(80,  5, 140, 20, Qt::AlignLeft | Qt::AlignVCenter,
            source->data(0).toString() + "." + source->data(2).toString());
        painter.drawText(80, 25, 140, 20, Qt::AlignLeft | Qt::AlignVCenter,
            source->data(1).toString());
        painter.drawText(80, 45, 140, 20, Qt::AlignLeft | Qt::AlignVCenter,
            source->data(3).toString());
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

bool HierarchyGraphView::_match(QDropEvent* mEvent) const {
    // only input/output slot can be connected
    // check mime-data type
    if (!mEvent->mimeData()->hasFormat("application/x-paramnode"))
        return false;

    QAbstractGraphicsShapeItem* source =
        reinterpret_cast<QAbstractGraphicsShapeItem*>
        (mEvent->mimeData()->data("application/x-paramnode").toULong());
    Q_ASSERT(source);

    // template instances cannot be connected to slots
    if(source->data(1) == "dummy")
        return false;

    QAbstractGraphicsShapeItem* target =
        qgraphicsitem_cast<QAbstractGraphicsShapeItem*>
        (itemAt(mEvent->pos()));
    if(!target)
        return false;

    // self connections are not allowed
    if (source->data(0) == target->data(0))
        return false;

    // only connections input <-> output slots are allowed
    if (source->data(1) == target->data(1))
        return false;

    // source and target type have to be identical
    if (source->data(3) != target->data(3))
        return false;

    return true;
}


void HierarchyGraphView::dragEnterEvent(QDragEnterEvent* mEvent) {
    // accept only paramnodes
    if (!mEvent->mimeData()->hasFormat("application/x-paramnode")) {
        mEvent->ignore();
        return;
    }

    mEvent->accept();

    if (_match(mEvent)) {
        QAbstractGraphicsShapeItem* target =
            qgraphicsitem_cast<QAbstractGraphicsShapeItem*>
            (itemAt(mEvent->pos()));
        if(!target)
            return;

        // green color to signalize success
        _highlight(target, Qt::green);
    }
}

void HierarchyGraphView::dragMoveEvent(QDragMoveEvent* mEvent) {
    // accept only paramnodes
    if (!mEvent->mimeData()->hasFormat("application/x-paramnode")) {
        mEvent->ignore();
        return;
    }

    mEvent->accept();

    QAbstractGraphicsShapeItem* target =
        qgraphicsitem_cast<QAbstractGraphicsShapeItem*>
        (itemAt(mEvent->pos()));

    if(_highlighted.indexOf(target) >= 0)
        return;

    _clearHighlight();

    if (target && _match(mEvent)) {
        _highlight(target, Qt::green);
    }
}

void HierarchyGraphView::dragLeaveEvent(QDragLeaveEvent* mEvent) {
    _clearHighlight();
    mEvent->accept();
}

void HierarchyGraphView::dropEvent(QDropEvent* mEvent) {
    // accept only paramnodes
    if (!mEvent->mimeData()->hasFormat("application/x-paramnode")) {
        mEvent->ignore();
        return;
    }

    mEvent->accept();

    _clearHighlight();

    if (!_model)
        return;

    // find drag source
    QAbstractGraphicsShapeItem* dragSource =
        reinterpret_cast<QAbstractGraphicsShapeItem*>
        (mEvent->mimeData()->data("application/x-paramnode").toULong());
    Q_ASSERT(dragSource);

    // handle new object creation request
    if (dragSource->data(1) == "dummy") {
        _model->addNode(dragSource->data(0).toString());
        return;
    }

    // else, handle attempts to connect slots
    if (_match(mEvent)) {
        // drag target
        QAbstractGraphicsShapeItem* dragTarget =
            qgraphicsitem_cast<QAbstractGraphicsShapeItem*>
            (itemAt(mEvent->pos()));
        if(!dragTarget)
            return;

        // collect slot information
        QString source = dragSource->data(0).toString() + "."
            + dragSource->data(2).toString();
        QString target = dragTarget->data(0).toString() + "."
            + dragTarget->data(2).toString();

        // request slot connection
        _model->connectSlot(source, target);
    }
}

void HierarchyGraphView::keyPressEvent(QKeyEvent* mEvent) {
    if (_model) {

        switch(mEvent->key()) {
            case Qt::Key_Right:
                _model->selectNext(false);
                break;
            case Qt::Key_Left:
                _model->selectNext(true);
                break;
            case Qt::Key_Delete:
            case Qt::Key_Backspace:
                _model->deleteNode(_model->prefix());
                break;
            case Qt::Key_F2:
                _model->renameNode(_model->prefix());
                break;
        }
    }
}
