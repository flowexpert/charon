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

DropTabBar::DropTabBar(QWidget* parent) : QTabBar(parent)
{
	setAcceptDrops(true) ;
}

void DropTabBar::dropEvent(QDropEvent* event)
{
	if(event->source())
	{	DropTabBar* src = qobject_cast<DropTabBar*>(event->source()) ;
		int srcIndex = src->tabAt(src->_dragStartPos) ;
		int dstIndex = tabAt(event->pos()) ;
		emit tabMoved(src,srcIndex,this,dstIndex) ;
	}
}

void DropTabBar::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{	_dragStartPos = event->pos() ;}
	QTabBar::mousePressEvent(event) ;
}

void DropTabBar::mouseMoveEvent(QMouseEvent* event)
{
	if(!(event->buttons() == Qt::LeftButton))
	{	QTabBar::mouseMoveEvent(event) ;	}

	if((event->pos() - _dragStartPos).manhattanLength() < QApplication::startDragDistance())
	{	QTabBar::mouseMoveEvent(event) ;	}

	QDrag* drag = new QDrag(this) ;
	QMimeData* mime = new QMimeData ;
	mime->setData("action","tab-move") ;
	drag->setMimeData(mime) ;
	drag->exec() ;
}


void DropTabBar::dragEnterEvent(QDragEnterEvent* event)
{
	const QMimeData* m = event->mimeData() ;
	QStringList formats = m->formats() ;
	if (formats.contains("action") && (m->data("action") == "tab-move"))
	{	event->acceptProposedAction() ;	}
}



//-----------------------------------------------------------------------------

DropTabWidget::DropTabWidget(QWidget* parent) : QTabWidget(parent)
{
	DropTabBar* tabBar = new DropTabBar(this) ;
	setTabBar(tabBar) ;
	connect(tabBar, SIGNAL(tabMoved(DropTabBar*, int, DropTabBar*, int)),this,SLOT(moveTabs(DropTabBar*, int, DropTabBar*, int))) ;
	
	setAcceptDrops(true) ;
} 

void DropTabWidget::moveTabs(DropTabBar* source, int sourceIndex, DropTabBar* dest, int destIndex)
{
	DropTabWidget* srcTab = qobject_cast<DropTabWidget*>(source->parentWidget()) ;
	DropTabWidget* dstTab = qobject_cast<DropTabWidget*>(dest->parentWidget()) ;
	if(!srcTab || !dstTab)
		return ;
	if(sourceIndex < srcTab->count())
	{
		QWidget* widget = srcTab->widget(sourceIndex) ;
		QString text = srcTab->tabText(sourceIndex) ;
		srcTab->removeTab(sourceIndex) ;
		dstTab->insertTab(destIndex,widget,text) ;
	}
}

void DropTabWidget::dragEnterEvent(QDragEnterEvent* event)
{
	const QMimeData* m = event->mimeData() ;
	QStringList formats = m->formats() ;
	if (formats.contains("action") && (m->data("action") == "tab-move"))
	{	event->acceptProposedAction() ;	}
}


void DropTabWidget::dropEvent(QDropEvent *event)
{
	if(event->source())
	{	DropTabBar* src = qobject_cast<DropTabBar*>(event->source()) ;
		int srcIndex = src->tabAt(src->_dragStartPos) ;
		int dstIndex = src->tabAt(event->pos()) ;
		moveTabs(src,srcIndex,qobject_cast<DropTabBar*>(tabBar()),0) ;
	}

}

void DropTabWidget::enterEvent(QEvent* event)
{
	emit mouseEntered(this) ;
}

View::View(AbstractPixelInspector* i, RGBChannels mode) :
	channelMode(mode), inspector(i) {
}

//-----------------------------------------------------------------------------

ViewStack::ViewStack(QWidget* p) : QWidget(p),
	_switchViewModeActs(0),
	_layoutActions(0),
	_saveCurrentViewAct(0),
	_centerAndResetZoomAct(0),
	_alignAndZoomAct(0),
	_updatePending(false),
	_index(0),
	_zoomLevel(0),
	__currentTabWidget(0)
{
	
	DropTabWidget* tabWidget = new DropTabWidget(this) ;
		tabWidget->setMovable(true) ;
		tabWidget->setUsesScrollButtons(true) ;
		tabWidget->setFocusPolicy(Qt::StrongFocus) ;
		tabWidget->setContentsMargins(2,2,2,2) ;
	
			connect(tabWidget, SIGNAL(mouseEntered(DropTabWidget* )),
				this, SLOT(_changeCurrentTabWidget(DropTabWidget*))) ;


	_tabWidgets.push_back(tabWidget) ;
	__currentTabWidget = tabWidget ;

	QGridLayout* layout = new QGridLayout(this) ;
		layout->addWidget(tabWidget,0,0) ;
		layout->setContentsMargins(2,2,2,2) ;
	this->setLayout(layout) ;
	this->setSizePolicy(
			QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

	connect(this, SIGNAL(imageLinked()), this, SLOT(_linkImages()), Qt::QueuedConnection) ;

	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(_emitDimensionMessage())) ;

	_createActions() ;
	
	this->setContextMenuPolicy(Qt::ActionsContextMenu) ;
}

ViewStack::~ViewStack() {
	this->clear() ;
}


void ViewStack::_createActions()
{
	QSignalMapper* switchViewModeMapper = new QSignalMapper(this) ;
	_switchViewModeActs = new QActionGroup(this) ;

		QAction* tableModeAct = new QAction(QString("Table View"), _switchViewModeActs) ;
			tableModeAct->setShortcut(QKeySequence(Qt::Key_T)) ;
			connect(tableModeAct, SIGNAL(triggered()), switchViewModeMapper, SLOT(map())) ;
			switchViewModeMapper->setMapping(tableModeAct, static_cast<int>(ViewStack::Table)) ;
		QAction* greyModeAct = new QAction(QString("Greyscale"), _switchViewModeActs) ;
			greyModeAct->setShortcut(QKeySequence(Qt::Key_G)) ;
			connect(greyModeAct, SIGNAL(triggered()), switchViewModeMapper, SLOT(map())) ;
			switchViewModeMapper->setMapping(greyModeAct, static_cast<int>(ViewStack::Grey)) ;
		QAction* rgb3ModeAct = new QAction(QString("4.Dim as RGB Channels"), _switchViewModeActs) ;
			rgb3ModeAct->setShortcut(QKeySequence(Qt::Key_C)) ;
			connect(rgb3ModeAct, SIGNAL(triggered()), switchViewModeMapper, SLOT(map())) ;
			switchViewModeMapper->setMapping(rgb3ModeAct, static_cast<int>(ViewStack::Rgb3)) ;
		QAction* rgb4ModeAct = new QAction(QString("5. Dim as RGB Channels"), _switchViewModeActs) ;
			rgb4ModeAct->setShortcut(QKeySequence(Qt::Key_V)) ;
			connect(rgb4ModeAct, SIGNAL(triggered()), switchViewModeMapper, SLOT(map())) ;
			switchViewModeMapper->setMapping(rgb4ModeAct, static_cast<int>(ViewStack::Rgb4)) ;

	connect(switchViewModeMapper, SIGNAL(mapped(int)), this, SLOT(_switchColorMode(int))) ;
	_switchViewModeActs->setExclusive(false) ;
	this->addActions(_switchViewModeActs->actions()) ;

	QAction* separator = new QAction(switchViewModeMapper) ;
		separator->setSeparator(true) ;
	this->addAction(separator) ;


	_saveCurrentViewAct = new QAction(QString("Save current view"), this) ;
	_saveCurrentViewAct->setStatusTip(QString("Save current view to image file")) ;
	_saveCurrentViewAct->setShortcut(QKeySequence::Save) ;
	connect(_saveCurrentViewAct, SIGNAL(triggered()), this, SLOT(_saveCurrentView())) ;
	this->addAction(_saveCurrentViewAct) ;

	_centerAndResetZoomAct = new QAction(QString("Reset view"), this) ;
	_centerAndResetZoomAct->setStatusTip(QString("Set zoom level to 0 and move view to center of window")) ;
	_centerAndResetZoomAct->setShortcut(QKeySequence(Qt::Key_R)) ;
	connect(_centerAndResetZoomAct, SIGNAL(triggered()), this, SLOT(_centerAndResetZoom())) ;
	this->addAction(_centerAndResetZoomAct) ;

	_switchLogModeAct = new QAction(QString("Switch log mode"), this) ;
	_switchLogModeAct->setStatusTip(QString("switch floating-point display to logarithmic scaling")) ;
	_switchLogModeAct->setShortcut(QKeySequence(Qt::Key_L)) ;
	connect(_switchLogModeAct, SIGNAL(triggered()), this, SLOT(_switchLogMode())) ;
	this->addAction(_switchLogModeAct) ;
	
	_alignAndZoomAct = new QAction(QString("Align other views"), this);
	_alignAndZoomAct->setStatusTip(QString("align all other views of same size to this one")) ;
	_alignAndZoomAct->setShortcut(QKeySequence(Qt::Key_Space)) ;
	connect(_alignAndZoomAct, SIGNAL(triggered()), this, SLOT(_alignAndZoom()));
	this->addAction(_alignAndZoomAct) ;
	
	_layoutActions = new QActionGroup(this) ;

	QSignalMapper* layoutMapper = new QSignalMapper(this) ;
	QAction* lA = new QAction("1 x 1", this) ;
		connect(lA,SIGNAL(triggered()), layoutMapper, SLOT(map())) ;
		layoutMapper->setMapping(lA,"1x1") ;
		_layoutActions->addAction(lA) ;
	lA = new QAction("1 x 2", this) ;
		connect(lA,SIGNAL(triggered()), layoutMapper, SLOT(map())) ;
		layoutMapper->setMapping(lA,"1x2") ;
		_layoutActions->addAction(lA) ;
	lA = new QAction("1 x 3", this) ;
		connect(lA,SIGNAL(triggered()), layoutMapper, SLOT(map())) ;
		layoutMapper->setMapping(lA,"1x3") ;
		_layoutActions->addAction(lA) ;
	lA = new QAction("1 x 4", this) ;
		connect(lA,SIGNAL(triggered()), layoutMapper, SLOT(map())) ;
		layoutMapper->setMapping(lA,"1x4") ;
		_layoutActions->addAction(lA) ;
	lA = new QAction("2 x 1", this) ;
		connect(lA,SIGNAL(triggered()), layoutMapper, SLOT(map())) ;
		layoutMapper->setMapping(lA,"2x1") ;
		_layoutActions->addAction(lA) ;
	lA = new QAction("3 x 1", this) ;
		connect(lA,SIGNAL(triggered()), layoutMapper, SLOT(map())) ;
		layoutMapper->setMapping(lA,"3x1") ;
		_layoutActions->addAction(lA) ;
	lA = new QAction("2 x 2", this) ;
		connect(lA,SIGNAL(triggered()), layoutMapper, SLOT(map())) ;
		layoutMapper->setMapping(lA,"2x2") ;
		_layoutActions->addAction(lA) ;
	lA = new QAction("2 x 3", this) ;
		connect(lA,SIGNAL(triggered()), layoutMapper, SLOT(map())) ;
		layoutMapper->setMapping(lA,"2x3") ;
		_layoutActions->addAction(lA) ;
	lA = new QAction("2 x 4", this) ;
		connect(lA,SIGNAL(triggered()), layoutMapper, SLOT(map())) ;
		layoutMapper->setMapping(lA,"2x4") ;
		_layoutActions->addAction(lA) ;
	lA = new QAction("3 x 3", this) ;
		connect(lA,SIGNAL(triggered()), layoutMapper, SLOT(map())) ;
		layoutMapper->setMapping(lA,"3x3") ;
		_layoutActions->addAction(lA) ;
	connect(layoutMapper, SIGNAL(mapped(QString)),this,SLOT(_switchToNamedLayout(QString))) ;
}

void ViewStack::clear() {
	for(int ii = 0 ; ii < _tabWidgets.size() ; ii++)
	{
		while(_tabWidgets[ii]->count() > 0) {
			int c = _tabWidgets[ii]->count() - 1;
			QWidget* cur = _tabWidgets[ii]->widget(c);
			_tabWidgets[ii]->removeTab(c);
			delete cur;
		}
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
	return _currentTabWidget()->currentIndex() ;
}

void ViewStack::setCurrentIndex(int index) {
	_index = index ;
	if(!_updatePending)
	{	_updatePending = true ;
		//emit imageLinked() ;
	}
}

void ViewStack::_emitDimensionMessage() {
	DropTabWidget* tabWidget = _currentTabWidget() ;
	int index = tabWidget->currentIndex() ;
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
			message += " [RGB 5. dimension]" ;
			break ;
		}
		case(RGB3) : {
			message += " [RGB 4. dimension]" ;
			break ;
		}
	}
	int level = getZoomLevel() ;
	int zoom = 100 ;
	if(level < 0)
		zoom = 100 / (-level +1) ;
	else
		zoom = 100 * (level + 1) ;
	message += QString(" | Zoom %1%").arg(zoom) ;
	emit exportDimensionsMessage(message) ;
}

void ViewStack::_linkImages()
{
	DropTabWidget* tabWidget = _currentTabWidget() ;
	//this->clear() ;
	for(size_t ii = 0 ; ii < _views.size() ; ii++)
	{
		View& view = _views[ii] ;
		if(view.inspector == 0)
			continue ;
		view.channelMode = view.inspector->isRGB() ;
		DropTabWidget* tabWidget = _tabWidgets[0] ;
		if(view.inspector->dim()[0] < 8 && view.inspector->dim()[1] < 8)
		{
			tabWidget->addTab(_createImageTableView(view.inspector),QString::fromStdString(view.inspector->name)) ;
			continue ;
		}

		if(view.channelMode == RGB4 || view.channelMode == RGB3) //default: register image as RGB image
		{
			QImageViewer* viewer = new QImageViewer(0) ;
			tabWidget->addTab(viewer, QString::fromStdString(_views[ii].inspector->name)) ;
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
			connect(
					viewer, SIGNAL(zoomLevelChanged(int)),
					this, SLOT(_emitDimensionMessage())) ;
		}
		else
		{
			FImageViewer* viewer = new FImageViewer(0) ;
			tabWidget->addTab(viewer, QString::fromStdString(_views[ii].inspector->name)) ;
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
			connect(
					viewer->imageViewer(), SIGNAL(zoomLevelChanged(int)),
					this, SLOT(_emitDimensionMessage())) ;
		}
	}
	tabWidget->setCurrentIndex(_index) ;
	_updatePending = false ;
	_alignAndZoom() ;
	this->parentWidget()->show() ;
}

void ViewStack::_switchColorMode(int mode)
{
	DropTabWidget* tabWidget = _currentTabWidget() ;
		
	int index = tabWidget->currentIndex() ;
	View& view = _views[index] ;
	if(index < 0 || view.inspector ==0)
	{	return ;	}
	ViewMode viewMode = static_cast<ViewMode>(mode) ;
	if(viewMode == ViewStack::Table && 
		(view.inspector->dim()[0] * view.inspector->dim()[1] > 1e5))
	{
		QMessageBox::warning(this, "ArgosDisplay", 
			"Table view mode is limited to images with less than 10.000 pixels") ;
		return ;
	}

	tabWidget->removeTab(index) ;
	if(viewMode == ViewStack::Rgb4)
	{
		QImageViewer* viewer = new QImageViewer(0) ;
		tabWidget->insertTab(index, viewer, QString::fromStdString(view.inspector->name)) ;
		viewer->setImage(view.inspector->getRGBImage(RGB4).qImage()) ;
		connect(
					viewer, SIGNAL(mouseOver(int, int)),
					this, SLOT(_processMouseMovement(int, int))) ;
		view.channelMode = RGB4 ;
	}
	else if(viewMode == ViewStack::Rgb3)
	{
		QImageViewer* viewer = new QImageViewer(0) ;
		tabWidget->insertTab(index, viewer, QString::fromStdString(view.inspector->name)) ;
		viewer->setImage(view.inspector->getRGBImage(RGB3).qImage()) ;
		connect(
					viewer, SIGNAL(mouseOver(int, int)),
					this, SLOT(_processMouseMovement(int, int))) ;
		view.channelMode = RGB3 ;
	}
	else if(viewMode == ViewStack::Table)
	{
		tabWidget->insertTab(index,_createImageTableView(view.inspector),
							QString::fromStdString(view.inspector->name)) ;
	}
	else if(viewMode == ViewStack::Grey)
	{
		FImageViewer* viewer = new FImageViewer(0) ;
		tabWidget->insertTab(index, viewer, QString::fromStdString(view.inspector->name)) ;
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
	tabWidget->setCurrentIndex(index) ;
}

void ViewStack::_switchLogMode() {
	DropTabWidget* tabWidget = _currentTabWidget() ;

	int index = tabWidget->currentIndex() ;
	if(index < 0 || _views[index].inspector ==0)
	{	return ;	}
	FImageViewer* viewer = qobject_cast<FImageViewer*>(tabWidget->currentWidget()) ;
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
	DropTabWidget* tabWidget = _currentTabWidget() ;

	// assume the int values of Key_1 to Key_9 are in ascending order
	if(key >= Qt::Key_1 && key <= Qt::Key_9)
		// this will give 0 for Key_1, 1 for Key_2 ...
		tabWidget->setCurrentIndex(key % Qt::Key_1) ;
	// DropTabWidget will check if the range is valid (at least in Qt 4.6 it did)
	else if(key == Qt::Key_Left)
	{
		int index = tabWidget->currentIndex() ;
		tabWidget->setCurrentIndex((index - 1) % tabWidget->count()) ;
	}
	else if(key == Qt::Key_Right)
	{
		int index = tabWidget->currentIndex() ;
		tabWidget->setCurrentIndex((index + 1) % tabWidget->count()) ;
	}
	else
		// pass event to base class
		this->QWidget::keyPressEvent(event) ;
}

QImageViewer& ViewStack::_currentViewer() const
{
	DropTabWidget* tabWidget = _currentTabWidget() ;

	int index = tabWidget->currentIndex() ;
	if(index < 0 || _views.size() <= size_t(index) || _views[index].inspector ==0)
	{	throw std::runtime_error("No active Viewer instance available!") ;	}
	

	QString className = tabWidget->currentWidget()->metaObject()->className() ;
	if(className == "FImageViewer")
	{
		FImageViewer* fViewer = qobject_cast<FImageViewer*>(tabWidget->currentWidget()) ;
		if(!fViewer)
		{	throw std::runtime_error("Unknown Tab Widget!") ;	}
		return *(fViewer->imageViewer()) ;
	}
	else if(className == "QImageViewer")
	{
		QImageViewer* viewer = qobject_cast<QImageViewer*>(tabWidget->currentWidget()) ;
		if(!viewer)
		{	throw std::runtime_error("Unknown Tab Widget!") ;	}
		return *viewer ;
	}
	throw std::runtime_error("Unknown Tab Widget!") ;

}

void ViewStack::_saveCurrentView()
{
	
	DropTabWidget* tabWidget = _currentTabWidget() ;
	static QString workingDir = QString() ;
	int index = tabWidget->currentIndex() ;
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
	catch(std::runtime_error& err) {
		return ;
	}
	_emitDimensionMessage() ;
}

void ViewStack::_alignAndZoom()
{
	DropTabWidget* tabWidget = _currentTabWidget() ;
	if(tabWidget->count() == 0)
		return ;
	//rember the index of the active tab
	int index = tabWidget->currentIndex() ;
	try
	{	_currentViewer() ;	}
	catch(std::runtime_error& err) //implies TableWidget view; do nothing
	{	return ;	}
	int zoom = _currentViewer().zoomLevel() ;
	QPoint upperLeft = _currentViewer().upperLeft() ;
	int h = _currentViewer().originalHeight() ;
	int w = _currentViewer().originalWidth() ;
	for(int ii = 0 ; ii < tabWidget->count() ; ii++)
	{
		if(ii == tabWidget->currentIndex())
			continue ;
		QImageViewer* viewer = 0 ;
		QString className = tabWidget->widget(ii)->metaObject()->className() ;
		if(className == "FImageViewer")
		{
			FImageViewer* fViewer = qobject_cast<FImageViewer*>(tabWidget->widget(ii)) ;
			if(fViewer)
				viewer = fViewer->imageViewer() ;
		}
		else if(className == "QImageViewer")
		{
			QImageViewer* qviewer = qobject_cast<QImageViewer*>(tabWidget->widget(ii)) ;
			viewer = qviewer ;
		}

		if(!viewer || viewer->originalHeight() != h || viewer->originalWidth() != w)
			continue ;
		//zooming and slide does not work correctly when the viewer tab is in the background
		//bring it to the front and reset the active index afterwards
		tabWidget->setCurrentIndex(ii) ;
		viewer->setZoomLevel(zoom) ;
		viewer->slideBy(upperLeft - viewer->upperLeft()) ;
	}
	tabWidget->setCurrentIndex(index) ;
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
	DropTabWidget* tabWidget = _currentTabWidget() ;

	int index = tabWidget->currentIndex() ;
	if(index < 0 || _views.size() <= size_t(index) || _views[index].inspector ==0)
	{       throw std::runtime_error("No active Viewer instance available!") ;      }

	QString className = tabWidget->currentWidget()->metaObject()->className() ;
	if(className == "FImageViewer")
	{
		FImageViewer* fViewer = qobject_cast<FImageViewer*>(tabWidget->currentWidget()) ;
		if(!fViewer)
		{       throw std::runtime_error("Unknown Tab Widget!") ;       }
		return fViewer->imageViewer() ;
	}
	else if(className == "QImageViewer")
	{
		QImageViewer* viewer = qobject_cast<QImageViewer*>(tabWidget->currentWidget()) ;
		if(!viewer)
		{       throw std::runtime_error("Unknown Tab Widget!") ;       }
		return viewer ;
	}
	throw std::runtime_error("Unknown Tab Widget!") ;
}

QWidget* ViewStack::_createImageTableView(AbstractPixelInspector* inspector)
{
	const AbstractPixelInspector& in = *inspector ;
	int width = in.dim()[0] ;
	int height = in.dim()[1] ;
	QTableWidget* table = new QTableWidget(	height,width,0) ;
		table->setSortingEnabled(false) ;
		table->setCornerButtonEnabled(false) ;
	QTableWidgetItem* prototype = new QTableWidgetItem ;
		prototype->setTextAlignment(Qt::AlignLeft) ;
		prototype->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled) ;
	table->setItemPrototype(prototype) ;
	QStringList vHeaders, hHeaders ;
	for(int yy = 0 ; yy < height ; yy++)
	{	
		vHeaders.append(QString("%1").arg(yy)) ;
		for(int xx = 0 ; xx <width ; xx++)
		{
			prototype->setText(QString("%1").arg(in(xx,yy)[0])) ;
			table->setItem(yy,xx, prototype->clone()) ;
		}
	}
	for(int xx = 0 ; xx <width ; xx++)
	{	hHeaders.append(QString("%1").arg(xx)) ;	}
	table->setHorizontalHeaderLabels(hHeaders) ;
	table->setVerticalHeaderLabels(vHeaders) ;


	table->resizeColumnsToContents() ;
	table->resizeRowsToContents() ;
	return table ;
}

void ViewStack::switchLayout(int rows, int columns)
{
	QGridLayout* l = qobject_cast<QGridLayout*>(this->layout()) ;
	if((rows * columns <= 0) ||
		(rows * columns == _tabWidgets.size() &&
		rows == l->rowCount() &&
		columns == l->columnCount()))
	{
		//do nothing if we get illegal values or the status quo
		return ;
	}
	if(rows * columns > _tabWidgets.size())
	{
		DropTabWidget* firstTab = _tabWidgets[0] ;
		for(int ii = _tabWidgets.size() ; ii < rows * columns ; ii++)
		{
			DropTabWidget* tabWidget = new DropTabWidget(this) ;
			tabWidget->setMovable(true) ;
			tabWidget->setUsesScrollButtons(true) ;
			tabWidget->setContentsMargins(2,2,2,2);
			//tabWidget->setFocusPolicy(Qt::StrongFocus) ;
			tabWidget->setContentsMargins(2,2,2,2) ;
			connect(tabWidget, SIGNAL(mouseEntered(DropTabWidget*)),
				this, SLOT(_changeCurrentTabWidget(DropTabWidget*))) ;
			_tabWidgets.push_back(tabWidget) ;

			//move tabs from first tabWidget to new tabWidgets
			if(firstTab->count() > 1)
			{	QWidget* last = firstTab->widget(firstTab->count() -1) ;
				QString text = firstTab->tabText(firstTab->count() -1) ;
				firstTab->removeTab(firstTab->count() -1) ;
				_tabWidgets.last()->addTab(last,text) ;
			}
		}
	}
	else if(rows * columns < _tabWidgets.size())
	{
		DropTabWidget* firstTab = _tabWidgets[0] ;
		//move all tabs from the to-be deleted tabWidgets to the first
		while(_tabWidgets.size() > rows * columns)
		{
			DropTabWidget* tW = _tabWidgets.last() ;
			while(tW->count() > 0)
			{	QWidget* last = tW->widget(0) ;
				QString text = tW->tabText(0) ;
				tW->removeTab(0) ;
				firstTab->addTab(last, text) ;
			}
			delete tW ;
			_tabWidgets.pop_back() ;
		}
	}
	//set new arrangement
	int index = 0 ;
	delete l ;
	l = new QGridLayout(this) ;
	l->setContentsMargins(2,2,2,2) ;
	this->setSizePolicy(
			QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
	for(int row = 0 ; row < rows ; row++)
		for(int column = 0 ; column < columns ; column++,index++)
		{
			l->addWidget(_tabWidgets[index],row,column) ;
		}

}

void ViewStack::_switchToNamedLayout(const QString& layout)
{
	//i'm innocent! They forced me to do it!
	if(layout == "1x1")
		switchLayout(1,1) ;
	else if(layout == "1x2")
		switchLayout(1,2) ;
	else if(layout == "1x3")
		switchLayout(1,3) ;
	else if(layout == "1x4")
		switchLayout(1,4) ;
	else if(layout == "2x1")
		switchLayout(2,1) ;
	else if(layout == "3x1")
		switchLayout(3,1) ;
	else if(layout == "2x2")
		switchLayout(2,2) ;
	else if(layout == "2x3")
		switchLayout(2,3) ;
	else if(layout == "2x4")
		switchLayout(2,4) ;
	else if(layout == "3x3")
		switchLayout(3,3) ;
}

const QList<QAction*> ViewStack::layoutActions() const
{
	return _layoutActions->actions() ;
}

DropTabWidget* ViewStack::_currentTabWidget() const
{
	return __currentTabWidget ;
}

void ViewStack::_changeCurrentTabWidget(DropTabWidget* widget)
{
	if(widget)
		__currentTabWidget = widget ;
	else
		__currentTabWidget = _tabWidgets[0] ;
	_emitDimensionMessage() ;
}
