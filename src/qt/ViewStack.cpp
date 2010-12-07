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

using namespace ArgosDisplay ;

ViewStack::ViewStack(QWidget* parent) : QWidget(parent)
{
	_tabWidget = new QTabWidget(this) ;

	QVBoxLayout* layout = new QVBoxLayout ;
		layout->addWidget(_tabWidget) ;
	this->setLayout(layout) ;
}

ViewStack::~ViewStack()
{
	;
}

void ViewStack::linkFloatImage(const vigra::FImage& img)
{
	FImageViewer* viewer = new FImageViewer(0) ;
	_tabWidget->addTab(viewer, QString("%1").arg(_tabWidget->count())) ;
	viewer->setImage(img) ;
}

void ViewStack::linkRgbaImage(const vigra::QRGBImage& img)
{
	QImageViewer* viewer = new QImageViewer(0) ;
	_tabWidget->addTab(viewer, QString("%1").arg(_tabWidget->count())) ;
	viewer->setImage(img.qImage()) ;
}
