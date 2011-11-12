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
#include "TuchulchaWindow.h"
#include "GraphModel.h"
#include "NodeHandler.h"

#include "FlowWidget.moc"

FlowWidget::FlowWidget(QWidget* myParent) :
		QMdiSubWindow(myParent) {
	// init GUI
	_viewer = new QGraphicsView(this);
	_viewer->setRenderHints(QPainter::Antialiasing);
	_viewer->setAcceptDrops(true);
	_nodehandler = new NodeHandler(_viewer);
	connect(_nodehandler, SIGNAL(statusMessage(QString, int)), this, SIGNAL(statusMessage(QString, int))) ;
	_viewer->setScene(_nodehandler);

	setWidget(_viewer);
	setAttribute(Qt::WA_DeleteOnClose, true);
	connect(model(), SIGNAL(fileNameChanged (QString)),
		this, SLOT(updateFileName(QString)));
	TuchulchaWindow* main = qobject_cast<TuchulchaWindow*>(parent()->parent());
	Q_ASSERT(main);
	connect(model(), SIGNAL(fileNameChanged (QString)),
		main, SLOT(setCurrentFile(QString)));
	connect(model(), SIGNAL(modified(bool)),
		this, SLOT(modify(bool)));
	connect(model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)),
		this, SLOT(modify()));
	setWindowTitle(tr("New file [*]"));
}

FlowWidget::~FlowWidget() {
}

void FlowWidget::updateFileName(const QString& fileName) {
	setWindowTitle(QString("%1 [*]")
			.arg(QFileInfo(fileName).absoluteFilePath()));
}

void FlowWidget::load(const QString& fileName) {
	if (_nodehandler->load(fileName)) {
		setWindowTitle(QString("%1 [*]")
			.arg(QFileInfo(model()->fileName())
				.absoluteFilePath()));
		zoomFit();
	}
	else
		close();
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
