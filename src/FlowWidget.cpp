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
#include "GraphModel.h"
#include "NodeView.h"
#include <QVBoxLayout>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QPen>
#include <QColor>
#include <QFile>
#include <QFileInfo>
#include <QtDebug>
#include <string>
#include <stdexcept>
#include "HierarchyGraphView.h"
#include "../app/MainWindow.h"

#include "FlowWidget.moc"

FlowWidget::FlowWidget(QWidget* myParent) :
		QMdiSubWindow(myParent) {
	// init GUI
	_viewer = new NodeView(this);
	setWidget(_viewer);
	setAttribute(Qt::WA_DeleteOnClose, true);
	connect(_viewer->model(), SIGNAL(fileNameChanged (QString)),
		this, SLOT(updateFileName(QString)));
	MainWindow* main = qobject_cast<MainWindow*>(parent()->parent());
	Q_ASSERT(main);
	connect(_viewer->model(), SIGNAL(fileNameChanged (QString)),
		main, SLOT(setCurrentFile(QString)));
	connect(_viewer->model(), SIGNAL(modified(bool)),
		this, SLOT(modify(bool)));
	connect(_viewer->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)),
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
	if (_viewer->load(fileName)) {
		setWindowTitle(QString("%1 [*]")
			.arg(QFileInfo(_viewer->model()->fileName())
				.absoluteFilePath()));
	}
	else
		close();
}

GraphModel* FlowWidget::model() {
	return _viewer->model();
}

const GraphModel* FlowWidget::model() const {
	return _viewer->model();
}

void FlowWidget::saveFlowChart() const {
	_viewer->save();
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

void FlowWidget::closeEvent(QCloseEvent* clEvent) {
	if (isWindowModified() && !_viewer->model()->fileName().isEmpty()
		&& QMessageBox::question(
				this, tr("Save modified content?"),
				tr("Content has been modified."
					"Do you want to save to %1 before closing?")
				.arg(_viewer->model()->fileName()),
				QMessageBox::Save|QMessageBox::Discard) == QMessageBox::Save)
		_viewer->model()->save();
	QMdiSubWindow::closeEvent(clEvent);
}
