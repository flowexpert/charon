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
	_viewStack = new ViewStack(this) ;
	this->setCentralWidget(_viewStack) ;
	
	this->statusBar() ;

}

MainWindow::~MainWindow()
{
	;
}

ViewStack& MainWindow::viewStack()
{
	return *_viewStack ;
}
