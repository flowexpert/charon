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

	connect(this, SIGNAL(imageLinked()), this, SLOT(linkImages()), Qt::QueuedConnection) ;
}

ViewStack::~ViewStack() {
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

void ViewStack::linkImages()
{
	//this->clear() ;
	for(size_t ii = 0 ; ii < _inspectors.size() ; ii++)
	{
		if(_inspectors[ii] == 0)
			continue ;
		if(_inspectors[ii]->isRGBA)
		{
			QImageViewer* viewer = new QImageViewer(0) ;
			_tabWidget->addTab(viewer, QString::fromStdString(_inspectors[ii]->name)) ;
			viewer->setImage(_inspectors[ii]->getRGBAImage().qImage()) ;
			connect(
					viewer, SIGNAL(mouseOver(int, int)),
					this, SLOT(processMouseMovement(int, int))) ;
		}
		else
		{
			FImageViewer* viewer = new FImageViewer(0) ;
			_tabWidget->addTab(viewer, QString::fromStdString(_inspectors[ii]->name)) ;
			viewer->setImage(_inspectors[ii]->getFImage()) ;
			connect(
					viewer->imageViewer(), SIGNAL(mouseOver(int, int)),
					this, SLOT(processMouseMovement(int, int)));

		}
	}
	_tabWidget->setCurrentIndex(_index) ;
	_updatePending = false ;
	this->parentWidget()->show() ;
}

void ViewStack::processMouseMovement(int x, int y) {
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
	// QTabWidget will check the range for us (at least in Qt 4.6 it did)
	else
		// pass event to base class
		this->QWidget::keyPressEvent(event) ;
}

