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

View::View(AbstractPixelInspector* i, RGBChannels mode) :
	channelMode(mode), inspector(i) {
}

ViewStack::ViewStack(QWidget* p) : QWidget(p),
	_tabWidget(0),
	_switchColorModeAct(0),
	_saveCurrentViewAct(0),
	_centerAndResetZoomAct(0),
	_alignAndZoomAct(0),
	_updatePending(false),
	_index(0),
	_zoomLevel(0)

{
	_tabWidget = new QTabWidget(this) ;
		_tabWidget->setMovable(true) ;
		_tabWidget->setUsesScrollButtons(true) ;
		_tabWidget->setFocusPolicy(Qt::StrongFocus) ;

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
	this->clear() ;
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
	
	_alignAndZoomAct = new QAction(QString("align other views"), this);
	_alignAndZoomAct->setStatusTip(QString("align all other views of same size to this one")) ;
	connect(_alignAndZoomAct, SIGNAL(triggered()), this, SLOT(_alignAndZoom()));
	this->addAction(_alignAndZoomAct) ;
	
}

void ViewStack::clear() {
	while(_tabWidget->count() > 0) {
		int c = _tabWidget->count() - 1;
		QWidget* cur = _tabWidget->widget(c);
		_tabWidget->removeTab(c);
		delete cur;
	}
	_views.clear() ;
}

void ViewStack:: linkImage(AbstractPixelInspector* inspector)
{
	if(inspector == 0)
	{	return ;	}
	_views.push_back(View(inspector,NONE)) ;
	
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
		//emit imageLinked() ;
	}
}

void ViewStack::_currentChanged(int index) {
	if(index < 0 || index >= (int)_views.size()) {
		// fix bug on selection change
		return;
	}
	if(_views[index].inspector == 0) {
		return;
	}
	const std::vector<int>& dims = _views[index].inspector->dim();
	QString message = QString("%1 x %2 x %3 x %4 x %5")
			.arg(dims[0]).arg(dims[1]).arg(dims[2]).arg(dims[3]).arg(dims[4]);
	switch(_views[index].channelMode) {
		case(NONE) : {
			message += " [GRAY 1. channel]" ;
			break ;
		 }
		case(RGB4) : {
			message += " [RGB 4. dimension]" ;
			break ;
		}
		case(RGB3) : {
			message += " [RGB 3. dimension]" ;
			break ;
		}
	}

	emit exportDimensionsMessage(message) ;
}

void ViewStack::_linkImages()
{
	//this->clear() ;
	for(size_t ii = 0 ; ii < _views.size() ; ii++)
	{
		View& view = _views[ii] ;
		if(view.inspector == 0)
			continue ;
		view.channelMode = view.inspector->isRGB() ;
		
		if(view.channelMode == RGB4 || view.channelMode == RGB3) //default: register image as RGB image
		{
			QImageViewer* viewer = new QImageViewer(0) ;
			_tabWidget->addTab(viewer, QString::fromStdString(_views[ii].inspector->name)) ;
			if(!_views[ii].inspector->isEmpty())
			{	try{
					viewer->setImage(_views[ii].inspector->getRGBImage(view.channelMode).qImage()) ;
				}
				catch(std::exception&)
				{	;/*occurs if image has size zero*/	}
			}
			connect(
					viewer, SIGNAL(mouseOver(int, int)),
					this, SLOT(_processMouseMovement(int, int))) ;
			viewer->setZoomLevel(_zoomLevel);
		}
		else
		{
			FImageViewer* viewer = new FImageViewer(0) ;
			_tabWidget->addTab(viewer, QString::fromStdString(_views[ii].inspector->name)) ;
			_views[ii].channelMode = NONE ;
			if(!_views[ii].inspector->isEmpty())
			{	try{
					viewer->setImage(_views[ii].inspector->getFImage()) ;
				}
				catch(std::exception&)
				{	;/*occurs if image has size zero*/	}
			}
			connect(
					viewer->imageViewer(), SIGNAL(mouseOver(int, int)),
					this, SLOT(_processMouseMovement(int, int)));
			viewer->imageViewer()->setZoomLevel(_zoomLevel);
		}
	}
	_tabWidget->setCurrentIndex(_index) ;
	_updatePending = false ;
	_alignAndZoom() ;
	this->parentWidget()->show() ;
}

void ViewStack::_switchColorMode()
{
	int index = _tabWidget->currentIndex() ;
	View& view = _views[index] ;
	if(index < 0 || view.inspector ==0)
	{	return ;	}
	QString className = _tabWidget->currentWidget()->metaObject()->className() ;
	if(view.channelMode == NONE && className == "FImageViewer")
	{
		_tabWidget->removeTab(index) ;
		QImageViewer* viewer = new QImageViewer(0) ;
		_tabWidget->insertTab(index, viewer, QString::fromStdString(view.inspector->name)) ;
		viewer->setImage(view.inspector->getRGBImage(RGB4).qImage()) ;
		connect(
					viewer, SIGNAL(mouseOver(int, int)),
					this, SLOT(_processMouseMovement(int, int))) ;
		view.channelMode = RGB4 ;
	}
	else if(view.channelMode == RGB4 && className == "QImageViewer")
	{
		_tabWidget->removeTab(index) ;
		QImageViewer* viewer = new QImageViewer(0) ;
		_tabWidget->insertTab(index, viewer, QString::fromStdString(view.inspector->name)) ;
		viewer->setImage(view.inspector->getRGBImage(RGB3).qImage()) ;
		connect(
					viewer, SIGNAL(mouseOver(int, int)),
					this, SLOT(_processMouseMovement(int, int))) ;
		view.channelMode = RGB3 ;
	}
	else if(view.channelMode == RGB3 && className == "QImageViewer")
	{
		_tabWidget->removeTab(index) ;
		FImageViewer* viewer = new FImageViewer(0) ;
		_tabWidget->insertTab(index, viewer, QString::fromStdString(view.inspector->name)) ;
		viewer->setImage(view.inspector->getFImage()) ;
		connect(
					viewer->imageViewer(), SIGNAL(mouseOver(int, int)),
					this, SLOT(_processMouseMovement(int, int))) ;
		view.channelMode = NONE ;
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
	if(index < 0 || _views[index].inspector ==0)
	{	return ;	}
	FImageViewer* viewer = qobject_cast<FImageViewer*>(_tabWidget->currentWidget()) ;
	if(!viewer)
		return ;
	viewer->setLogarithmicMode(!viewer->logarithmicMode()) ;
}


void ViewStack::_processMouseMovement(int x, int y) {
	QString message = QString("x : %1 y : %2  ").arg(x).arg(y) ;

	for(size_t ii = 0 ; ii < _views.size() ; ii++)
	{
		message += QString::fromStdString(_views[ii].inspector->name) + QString(" {") ;
		std::vector<double> vals = _views[ii].inspector->operator()(x,y) ;
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
	if(index < 0 || _views.size() <= size_t(index) || _views[index].inspector ==0)
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
	if(index < 0 || _views.size() <= size_t(index) || _views[index].inspector ==0)
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
	try {
		QImageViewer& viewer = _currentViewer() ;
		viewer.setZoomLevel(0) ;
		viewer.centerOn(QPoint(
				viewer.originalWidth()/2, viewer.originalHeight()/2));
	}
	catch(std::exception&) {
		return ;
	}
}

void ViewStack::_alignAndZoom()
{
	if(this->_tabWidget->count() == 0)
		return ;
	//rember the index of the active tab
	int index = _tabWidget->currentIndex() ;
	
	int zoom = _currentViewer().zoomLevel() ;
	QPoint upperLeft = _currentViewer().upperLeft() ;
	int h = _currentViewer().originalHeight() ;
	int w = _currentViewer().originalWidth() ;
	for(int ii = 0 ; ii < _tabWidget->count() ; ii++)
	{
		if(ii == _tabWidget->currentIndex())
			continue ;
		QImageViewer* viewer = 0 ;
		QString className = _tabWidget->widget(ii)->metaObject()->className() ;
		if(className == "FImageViewer")
		{
			FImageViewer* fViewer = qobject_cast<FImageViewer*>(_tabWidget->widget(ii)) ;
			if(fViewer)
				viewer = fViewer->imageViewer() ;
		}
		else if(className == "QImageViewer")
		{
			QImageViewer* qviewer = qobject_cast<QImageViewer*>(_tabWidget->widget(ii)) ;
			viewer = qviewer ;
		}

		if(!viewer || viewer->originalHeight() != h || viewer->originalWidth() != w)
			continue ;
		//zooming and slide does not work correctly when the viewer tab is in the background
		//bring it to the front and reset the active index afterwards
		_tabWidget->setCurrentIndex(ii) ;
		viewer->setZoomLevel(zoom) ;
		viewer->slideBy(upperLeft - viewer->upperLeft()) ;
	}
	_tabWidget->setCurrentIndex(index) ;
}

void ViewStack::setZoomLevel(int level) {
	_zoomLevel = level;
}

int ViewStack::getZoomLevel() const {
	try {
		return _currentViewer().zoomLevel();
	}
	catch (...) {
		return _zoomLevel;
	}
}

QWidget* ViewStack::getCurrentViewer()
{
	int index = _tabWidget->currentIndex() ;
	if(index < 0 || _views.size() <= size_t(index) || _views[index].inspector ==0)
	{       throw std::runtime_error("No active Viewer instance available!") ;      }

	QString className = _tabWidget->currentWidget()->metaObject()->className() ;
	if(className == "FImageViewer")
	{
		FImageViewer* fViewer = qobject_cast<FImageViewer*>(_tabWidget->currentWidget()) ;
		if(!fViewer)
		{       throw std::runtime_error("Unknown Tab Widget!") ;       }
		return fViewer->imageViewer() ;
	}
	else if(className == "QImageViewer")
	{
		QImageViewer* viewer = qobject_cast<QImageViewer*>(_tabWidget->currentWidget()) ;
		if(!viewer)
		{       throw std::runtime_error("Unknown Tab Widget!") ;       }
		return viewer ;
	}
	throw std::runtime_error("Unknown Tab Widget!") ;
}

