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

#include "ViewStack.hpp"
#include <QtGui>
#include "vigraqt/qimageviewer.hxx"
#include "vigraqt/fimageviewer.hxx"
#include <charon-utils/ArgosDisplay.h>

using namespace ArgosDisplay ;

ViewStack::ViewStack(QWidget* parent) : QWidget(parent)
{
	_tabWidget = new QTabWidget(this) ;
		_tabWidget->setUsesScrollButtons(true) ;

	_statusBar = new QStatusBar(this) ;

	QVBoxLayout* layout = new QVBoxLayout ;
		layout->addWidget(_tabWidget) ;
		layout->addWidget(_statusBar) ;
	this->setLayout(layout) ;
	QSizePolicy policy(QSizePolicy::Minimum, QSizePolicy::Minimum) ;
	this->setSizePolicy(policy) ;
}

ViewStack::~ViewStack()
{
	;
}

void ViewStack::linkFloatImage(const vigra::FImage& img, const std::string& name)
{
	FImageViewer* viewer = new FImageViewer(0) ;
	//_imgList.insert(std::pair<QWidget*, const vigra::MultiArrayView<5, T>*>(viewer, &mArray)) ;
	_tabWidget->addTab(viewer, QString::fromStdString(name)) ;
	viewer->setImage(img) ;
	connect(viewer->imageViewer(), SIGNAL(mouseOver(int, int)), this, SLOT(processMouseMovement(int, int))) ;
}

void ViewStack::linkRgbaImage(const vigra::QRGBImage& img, const std::string& name)
{
	QImageViewer* viewer = new QImageViewer(0) ;
	//_imgList.insert(std::pair<QWidget*, const vigra::MultiArrayView<5, T>*>(viewer, &mArray)) ;
	_tabWidget->addTab(viewer, QString::fromStdString(name)) ;
	viewer->setImage(img.qImage()) ;
	connect(viewer, SIGNAL(mouseOver(int, int)), this, SLOT(processMouseMovement(int, int))) ;
}



void ViewStack::processMouseMovement(int x, int y)
{
	QString message = QString("x : %1 y : %2").arg(x).arg(y) ;
	std::vector<std::pair<std::string, const VigraDoubleArray* > >::iterator dit = _doubleImgMap.begin() ;
	for(; dit != _doubleImgMap.end() ; dit++)
	{
		const VigraDoubleArray& array = *(dit->second) ;
		if(x < 0 || y < 0 || array.size() <= 0 || x >= array.size(0) || y >= array.size(1))
		{	continue ;	}
				//append name of parent node
				message += QString("  %1 : { ").arg(QString::fromStdString(dit->first)) ;

				//append pixel values
				for(int i = 0 ; i < array.size(4) ; i++)
		{	message += QString("%1 ").arg(array(x,y,0,0,i)) ;	}
		message += QString("}") ;
	}
	std::vector<std::pair<std::string, const VigraFloatArray* > >::iterator fit = _floatImgMap.begin() ;
	for(; fit != _floatImgMap.end() ; fit++)
	{
		const VigraFloatArray& array = *(fit->second) ;
		if(x < 0 || y < 0 || array.size() <= 0 || x >= array.size(0) || y >= array.size(1))
		{	continue ;	}
		message += QString("  %1 : { ").arg(QString::fromStdString(fit->first)) ;

				for(int i = 0 ; i < array.size(4) ; i++)
		{	message += QString("%1 ").arg(array(x,y,0,0,i)) ;	}
		message += QString("}") ;
	}
	std::vector<std::pair<std::string, const VigraIntArray* > >::iterator it = _intImgMap.begin() ;
	for(; it != _intImgMap.end() ; it++)
	{
		const VigraIntArray& array = *(it->second) ;
		if(x < 0 || y < 0 || array.size() <= 0 || x >= array.size(0) || y >= array.size(1))
		{	continue ;	}
		message += QString("  %1 : { ").arg(QString::fromStdString(it->first)) ;

				for(int i = 0 ; i < array.size(4) ; i++)
		{	message += QString("%1 ").arg(array(x,y,0,0,i)) ;	}
		message += QString("}") ;
	}


	_statusBar->showMessage(message) ;
}

void ViewStack::keyPressEvent(QKeyEvent * event )
{
	int key = event->key() ;
	
	//assume the int values of Key_1 to Key_9 are in ascending order
	if(key >= Qt::Key_1 && key <= Qt::Key_9)
	{	_tabWidget->setCurrentIndex(key % Qt::Key_1) ;	} //<- this will give 0 for Key_1, 1 for Key_2 ...
	//QTabWidget will check the range for us (at least in Qt 4.6 it did)
	else
	{	this->QWidget::keyPressEvent(event) ;	} //pass event to base class
}

