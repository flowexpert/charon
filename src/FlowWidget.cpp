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
#include <QVBoxLayout>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPen>
#include <QColor>
#include <QFile>
#include <QtDebug>
#include <string>
#include "HierarchyGraphView.h"
#include "../app/MainWindow.h"

#include "FlowWidget.moc"

FlowWidget::FlowWidget(QWidget* myParent) :
		QMdiSubWindow(myParent) {
	// init GUI
	_viewer = new HierarchyGraphView(this);
	setWidget(_viewer);
	setAttribute(Qt::WA_DeleteOnClose, true);
	setWindowTitle(tr("new file"));
	connect(_viewer->model(), SIGNAL(fileNameChanged (QString)),
		this, SLOT(setWindowTitle(QString)));
	MainWindow* main = qobject_cast<MainWindow*>(parent()->parent());
	Q_ASSERT(main);
	connect(_viewer->model(), SIGNAL(fileNameChanged (QString)),
		main, SLOT(setCurrentFile(QString)));
}

FlowWidget::~FlowWidget() {
}

void FlowWidget::load(const QString& fileName) {
	try {
		_viewer->load(fileName);
	}
	catch (const std::string& msg) {
		qWarning("%s", msg.c_str());
		close();
	}
	catch (const std::exception& err) {
		qWarning("%s", err.what());
		close();
	}
	catch (...) {
		qWarning("Got exception during load!");
		close();
	}
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
