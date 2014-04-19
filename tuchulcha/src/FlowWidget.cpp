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
/** @file FlowWidget.cpp
 *  @brief Implementation of class FlowWidget
 *
 *  @date	28.08.2008
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "FlowWidget.h"
#include <QGraphicsView>
#include <QMessageBox>
#include <QFileInfo>
#include <QWheelEvent>
#include <QDir>
#include "TuchulchaWindow.h"
#include "GraphModel.h"
#include "NodeHandler.h"

FlowWidget::FlowWidget(GraphModel* modelIn, QWidget* myParent) :
		QMdiSubWindow(myParent) {
	// init GUI
	_viewer = new QGraphicsView(this);
	_viewer->setRenderHints(QPainter::Antialiasing);
	_viewer->setAcceptDrops(true);
	_nodehandler = new NodeHandler(modelIn, _viewer);
	_viewer->setScene(_nodehandler);

	setWidget(_viewer);
	setAttribute(Qt::WA_DeleteOnClose, true);
	connect(model(), SIGNAL(fileNameChanged (QString)),
		SLOT(updateFileName(QString)));
	connect(_nodehandler, SIGNAL(statusMessage(QString)),
		SIGNAL(statusMessage(QString)));
	connect(_nodehandler, SIGNAL(nodeTypeSelected(QString)),
		SIGNAL(nodeTypeSelected(QString)));
	connect(model(), SIGNAL(modified(bool)), SLOT(modify(bool)));
	connect(model(), SIGNAL(commentChanged(QString)), _nodehandler,
		SLOT(updateTooltip(QString)));
	connect(this, SIGNAL(windowStateChanged(Qt::WindowStates, Qt::WindowStates)),
		SLOT(zoomFit(Qt::WindowStates, Qt::WindowStates)));
	updateFileName(model()->fileName());
}

void FlowWidget::updateFileName(const QString& fileName) {
	if (fileName.isEmpty() || fileName == QDir::homePath()) {
		setWindowTitle(tr("New file")+" [*]");
	}
	else {
		QFileInfo info(fileName);
		setWindowTitle(QString("%1 [*]").arg(info.baseName()));
	}
}

GraphModel* FlowWidget::model() {
	return _nodehandler->model();
}

const GraphModel* FlowWidget::model() const {
	return _nodehandler->model();
}

void FlowWidget::saveFlowChart() const {
	_nodehandler->saveFlowchart();
}

void FlowWidget::zoomIn() {
	_viewer->scale(1.2, 1.2);
}

void FlowWidget::zoomOut() {
	_viewer->scale(0.8, 0.8);
}

void FlowWidget::zoomFit() {
	_viewer->fitInView(_viewer->sceneRect(), Qt::KeepAspectRatio);
}

void FlowWidget::modify(bool val) {
	setWindowModified(val);
}

void FlowWidget::wheelEvent(QWheelEvent* ev) {
	if(ev->modifiers() & Qt::ControlModifier) {
		ev->accept();
		if(ev->delta() > 0)
			zoomIn();
		else
			zoomOut();
	}
	else
		QMdiSubWindow::wheelEvent(ev);
}

void FlowWidget::closeEvent(QCloseEvent* clEvent) {
	if (isWindowModified() && !model()->fileName().isEmpty()
		&& QMessageBox::question(
				this, tr("Save modified content?"),
				tr("Content has been modified."
					"Do you want to save to %1 before closing?")
				.arg(model()->fileName()),
				QMessageBox::Save|QMessageBox::Discard) == QMessageBox::Save)
		model()->save();
	QMdiSubWindow::closeEvent(clEvent);
}

void FlowWidget::zoomFit(Qt::WindowStates oldStates, Qt::WindowStates newStates) {
	// check if any flag except for Qt::WindowActive has changed
	if ((oldStates ^ newStates) == Qt::WindowActive)
		return;
	zoomFit();
}
