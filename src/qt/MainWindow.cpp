/*  Copyright (C) 2010 Stephan Meister

    This file is part of Charon.

    Charon is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Charon is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file MainWindow.cpp
 *  Implementation of the ArgosDisplay's MainWindow.
 *  \author Stephan Meister
 *  \date 26.11.2010
 */

#include "MainWindow.hpp"
#include "ViewStack.hpp"
#include <QtGui>
using namespace ArgosDisplay ;
//using namespace vigraqt ;

MainWindow::MainWindow() : _viewStack(0)
{
	_viewStack = new ViewStack(this);
	this->setCentralWidget(_viewStack);
	this->setWindowTitle("ArgosDisplay");
	QObject::connect(
			_viewStack, SIGNAL(exportStatusMessage(QString)),
			this->statusBar(),SLOT(showMessage(QString)));
}

MainWindow::~MainWindow()
{
	;
}

ViewStack& MainWindow::viewStack()
{
	return *_viewStack ;
}

void MainWindow::addDockWidget(QWidget* widget)
{
	Q_ASSERT(widget);
	if (widget->parent() != 0)
		return;
	QDockWidget* dockWidget = qobject_cast<QDockWidget*>(widget);
	if(!dockWidget) {
		dockWidget = new QDockWidget(this);
		dockWidget->setWidget(widget);
	}
	Qt::DockWidgetArea area = Qt::LeftDockWidgetArea; // = 0x1
	// area values are 0x1, 0x2, 0x4, 0x8
	while(!dockWidget->isAreaAllowed(area) && area <= 0x8)
		area=(Qt::DockWidgetArea)((int)area<<1);
	if (!(area&Qt::AllDockWidgetAreas)) {
		qDebug("%s",tr("No usable DockArea found for %1, using left one.").arg(
				dockWidget->windowTitle()).toAscii().constData());
		area = Qt::LeftDockWidgetArea;
	}
	QMainWindow::addDockWidget(area, dockWidget);
	Q_ASSERT(widget->parent() != 0);
	Q_ASSERT(dockWidget->parent() != 0);
}
