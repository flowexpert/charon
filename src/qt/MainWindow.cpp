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

#include <QtGui>
#include "MainWindow.hpp"
#include "ViewStack.hpp"

using namespace ArgosDisplay ;
//using namespace vigraqt ;

MainWindow::MainWindow(const std::string& title) : _viewStack(0),
	_updatePending(false)
{
	_viewStack = new ViewStack(this);
	this->setCentralWidget(_viewStack);
	this->setWindowTitle("ArgosDisplay : " + QString::fromStdString(title));
	QLabel* dimBar = new QLabel(this) ;
	dimBar->setText("0 x 0 x 0 x 0 x 0") ;
	this->statusBar()->addPermanentWidget(dimBar, 0 ) ;
	QObject::connect(
			_viewStack, SIGNAL(exportStatusMessage(QString)),
			this->statusBar(),SLOT(showMessage(QString)));
	QObject::connect(
			_viewStack, SIGNAL(exportDimensionsMessage(QString)),
			dimBar, SLOT(setText(QString))) ;
	QObject::connect(this, SIGNAL(widgetAdded()), this, SLOT(_addDockWidgets()), Qt::QueuedConnection) ;

	_createMenus() ;
}

MainWindow::~MainWindow()
{
	//seize parentship of all connected dock widgets so they can be deleted in their respective Parametered Objects
	for(size_t ii = 0 ; ii < _widgets.size() ; ii++)
	{
		_widgets[ii]->setParent(0) ;
	}
}

void MainWindow::_createMenus()
{
	QMenuBar* menu = menuBar() ;
	QMenu* viewMenu = menu->addMenu(QString("View")) ;
	viewMenu->addActions(_viewStack->actions()) ;
}

ViewStack& MainWindow::viewStack()
{
	return *_viewStack ;
}

void MainWindow::addDockWidget(QWidget* widget)
{
	Q_ASSERT(widget);
	_widgets.push_back(widget) ;
	if(!_updatePending)
	{	_updatePending = true ;
		emit widgetAdded() ;
	}
}

void MainWindow::_addDockWidgets()
{
	if(!_updatePending)
		return ;
	
	for(size_t ii = 0 ; ii < _widgets.size() ; ii++)
	{
		QWidget* widget = _widgets[ii] ;
		Q_ASSERT(widget) ;
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
	_updatePending = false ;

}
