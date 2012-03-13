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
/** \file ViewStack.cpp
 *  Implementation of the ArgosDisplay ViewStack.
 *  \author Stephan Meister
 *  \date 29.11.2010
 */

#include <QtGui>
#include "ViewStack.hpp"
#include "vigraqt/qimageviewer.hxx"
#include "vigraqt/fimageviewer.hxx"
#include <charon-utils/ArgosDisplay.h>

using namespace ArgosDisplay ;

ViewStack::ViewStack(QWidget* p) : QWidget(p),
	_updatePending(false),
	_index(0)
{
	_tabWidget = new QTabWidget(this) ;
	_tabWidget->setUsesScrollButtons(true) ;

	QVBoxLayout* layout = new QVBoxLayout ;
		layout->addWidget(_tabWidget) ;
	this->setLayout(layout) ;
	this->setSizePolicy(
			QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

	connect(this, SIGNAL(imageLinked()), this, SLOT(_linkImages()), Qt::QueuedConnection) ;

	connect(_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(_currentChanged(int))) ;

	_createActions() ;
	
	this->setContextMenuPolicy(Qt::ActionsContextMenu) ;
}

ViewStack::~ViewStack() {
}

void ViewStack::_createActions()
{
	_switchColorModeAct = new QAction(QString("switch color mode"), this) ;
	_switchColorModeAct->setStatusTip(QString("switch between RGB and grayscale display")) ;
	connect(_switchColorModeAct, SIGNAL(triggered()), this, SLOT(_switchColorMode())) ;
	this->addAction(_switchColorModeAct) ;
	
	_saveCurrentViewAct = new QAction(QString("save current view"), this) ;
	_saveCurrentViewAct->setStatusTip(QString("save current view to image file")) ;
	connect(_saveCurrentViewAct, SIGNAL(triggered()), this, SLOT(_saveCurrentView())) ;
	this->addAction(_saveCurrentViewAct) ;

	_centerAndResetZoomAct = new QAction(QString("reset view"), this) ;
	_centerAndResetZoomAct->setStatusTip(QString("set zoom level to 0 and move view to center of window")) ;
	connect(_centerAndResetZoomAct, SIGNAL(triggered()), this, SLOT(_centerAndResetZoom())) ;
	this->addAction(_centerAndResetZoomAct) ;

	_switchLogModeAct = new QAction(QString("switch log mode"), this) ;
	_switchLogModeAct->setStatusTip(QString("switch floating-point display to logarithmic scaling")) ;
	connect(_switchLogModeAct, SIGNAL(triggered()), this, SLOT(_switchLogMode())) ;
	this->addAction(_switchLogModeAct) ;

}

void ViewStack::clear() {
	while(_tabWidget->count() > 0) {
		int c = _tabWidget->count() - 1;
		QWidget* cur = _tabWidget->widget(c);
		_tabWidget->removeTab(c);
		delete cur;
	}
	_inspectors.clear() ;
}

void ViewStack:: linkImage(AbstractPixelInspector* inspector)
{
	if(inspector == 0)
	{	return ;	}
	_inspectors.push_back(inspector) ;
	
	//TODO: make this thread safe?
	if(!_updatePending)
	{	_updatePending = true ;
		emit imageLinked() ;
	}
}


int ViewStack::currentIndex() const {
	return _tabWidget->currentIndex() ;
}

void ViewStack::setCurrentIndex(int index) {
	_index = index ;
	if(!_updatePending)
	{	_updatePending = true ;
		emit imageLinked() ;
	}
}

void ViewStack::_currentChanged(int index) {
	if(index < 0 || index >= (int)_inspectors.size()) {
		// fix bug on selection change
		return;
	}
	if(_inspectors[index] == 0) {
		return;
	}
	const std::vector<int>& dims = _inspectors[index]->dim();
	QString message = QString("%1 x %2 x %3 x %4 x %5")
			.arg(dims[0]).arg(dims[1]).arg(dims[2]).arg(dims[3]).arg(dims[4]);
	emit exportDimensionsMessage(message) ;
}

void ViewStack::_linkImages()
{
	//this->clear() ;
	for(size_t ii = 0 ; ii < _inspectors.size() ; ii++)
	{
		if(_inspectors[ii] == 0)
			continue ;
		if(_inspectors[ii]->isRGB()) //default: register image as RGB image
		{
			QImageViewer* viewer = new QImageViewer(0) ;
			_tabWidget->addTab(viewer, QString::fromStdString(_inspectors[ii]->name)) ;
			if(!_inspectors[ii]->isEmpty())
			{	try{
					viewer->setImage(_inspectors[ii]->getRGBImage().qImage()) ;
				}
				catch(std::exception&)
				{	;/*occurs if image has size zero*/	}
			}
			connect(
					viewer, SIGNAL(mouseOver(int, int)),
					this, SLOT(_processMouseMovement(int, int))) ;
		}
		else
		{
			FImageViewer* viewer = new FImageViewer(0) ;
			_tabWidget->addTab(viewer, QString::fromStdString(_inspectors[ii]->name)) ;
			if(!_inspectors[ii]->isEmpty())
			{	try{
					viewer->setImage(_inspectors[ii]->getFImage()) ;
				}
				catch(std::exception&)
				{	;/*occurs if image has size zero*/	}
			}
			connect(
					viewer->imageViewer(), SIGNAL(mouseOver(int, int)),
					this, SLOT(_processMouseMovement(int, int)));

		}
	}
	_tabWidget->setCurrentIndex(_index) ;
	_updatePending = false ;
	this->parentWidget()->show() ;
}

void ViewStack::_switchColorMode()
{
	int index = _tabWidget->currentIndex() ;
	if(index < 0 || _inspectors[index] ==0)
	{	return ;	}
	QString className = _tabWidget->currentWidget()->metaObject()->className() ;
	if(className == "FImageViewer")
	{
		_tabWidget->removeTab(index) ;
		QImageViewer* viewer = new QImageViewer(0) ;
		_tabWidget->insertTab(index, viewer, QString::fromStdString(_inspectors[index]->name)) ;
		viewer->setImage(_inspectors[index]->getRGBImage().qImage()) ;
		connect(
					viewer, SIGNAL(mouseOver(int, int)),
					this, SLOT(_processMouseMovement(int, int))) ;
	}
	else if(className == "QImageViewer")
	{
		_tabWidget->removeTab(index) ;
		FImageViewer* viewer = new FImageViewer(0) ;
		_tabWidget->insertTab(index, viewer, QString::fromStdString(_inspectors[index]->name)) ;
		viewer->setImage(_inspectors[index]->getFImage()) ;
		connect(
					viewer->imageViewer(), SIGNAL(mouseOver(int, int)),
					this, SLOT(_processMouseMovement(int, int))) ;
	}
	else //unknown tab type
	{
		//maybe use later when we implement OpenGL displays
		;
	}
	_tabWidget->setCurrentIndex(index) ;
}

void ViewStack::_switchLogMode() {
	int index = _tabWidget->currentIndex() ;
	if(index < 0 || _inspectors[index] ==0)
	{	return ;	}
	FImageViewer* viewer = qobject_cast<FImageViewer*>(_tabWidget->currentWidget()) ;
	if(!viewer)
		return ;
	viewer->setLogarithmicMode(!viewer->logarithmicMode()) ;
}


void ViewStack::_processMouseMovement(int x, int y) {
	QString message = QString("x : %1 y : %2  ").arg(x).arg(y) ;

	for(size_t ii = 0 ; ii < _inspectors.size() ; ii++)
	{
		message += QString::fromStdString(_inspectors[ii]->name) + QString(" {") ;
		std::vector<double> vals = _inspectors[ii]->operator()(x,y) ;
		for(size_t jj = 0 ; jj < vals.size() ; jj++)
		{
			message += QString("%1 ").arg(vals[jj]) ;
		}
		message += QString("} ") ;
	}

	emit exportStatusMessage(message) ;
}

void ViewStack::keyPressEvent(QKeyEvent* event) {
	int key = event->key() ;
	
	// assume the int values of Key_1 to Key_9 are in ascending order
	if(key >= Qt::Key_1 && key <= Qt::Key_9)
		// this will give 0 for Key_1, 1 for Key_2 ...
		_tabWidget->setCurrentIndex(key % Qt::Key_1) ;
	// QTabWidget will check if the range is valid (at least in Qt 4.6 it did)
	else if(key == Qt::Key_Left)
	{
		int index = _tabWidget->currentIndex() ;
		_tabWidget->setCurrentIndex((index - 1) % _tabWidget->count()) ;
	}
	else if(key == Qt::Key_Right)
	{
		int index = _tabWidget->currentIndex() ;
		_tabWidget->setCurrentIndex((index + 1) % _tabWidget->count()) ;
	}
	else
		// pass event to base class
		this->QWidget::keyPressEvent(event) ;
}

QImageViewer& ViewStack::_currentViewer() const
{
	int index = _tabWidget->currentIndex() ;
	if(index < 0 || _inspectors.size() <= size_t(index) || _inspectors[index] ==0)
	{	throw std::runtime_error("No active Viewer instance available!") ;	}
	
	QString className = _tabWidget->currentWidget()->metaObject()->className() ;
	if(className == "FImageViewer")
	{
		FImageViewer* fViewer = qobject_cast<FImageViewer*>(_tabWidget->currentWidget()) ;
		if(!fViewer)
		{	throw std::runtime_error("Unknown Tab Widget!") ;	}
		return *(fViewer->imageViewer()) ;
	}
	else if(className == "QImageViewer")
	{
		QImageViewer* viewer = qobject_cast<QImageViewer*>(_tabWidget->currentWidget()) ;
		if(!viewer)
		{	throw std::runtime_error("Unknown Tab Widget!") ;	}
		return *viewer ;
	}
	throw std::runtime_error("Unknown Tab Widget!") ;

}

void ViewStack::_saveCurrentView()
{
	static QString workingDir = QString() ;
	int index = _tabWidget->currentIndex() ;
	if(index < 0 || _inspectors.size() <= size_t(index) || _inspectors[index] ==0)
	{	return ;	}
	try
	{	const QImageViewer& viewer = _currentViewer() ;
		QString destination = QFileDialog::getSaveFileName(this, QString("Image Filename"),	workingDir) ;
		if(destination.isEmpty())
		{	return ;	}
		QDir prevDir(destination) ;
		if(prevDir.cdUp())
		{	workingDir = prevDir.path() ;	}
		if(!viewer.originalImage().save(destination))
		{
			QMessageBox::warning(this, QString("ViewStack::saveCurrentView()"),
				QString("Could not save current view")) ;
		}
	}
	catch(std::exception& err)
	{
		QMessageBox::warning(this, QString("ViewStack::saveCurrentView()"),
			QString("Could not save current view: \n") + QString::fromStdString(err.what())) ;
	}
}

void ViewStack::_centerAndResetZoom()
{
	try
	{	QImageViewer& viewer = _currentViewer() ;
		viewer.setZoomLevel(0) ;
		viewer.centerOn(QPoint(viewer.originalWidth() / 2,viewer.originalHeight() / 2)) ;
	}
	catch(std::exception&)
	{
		return ;
	}
}
