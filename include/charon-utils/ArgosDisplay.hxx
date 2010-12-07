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
/** \file ArgosDisplay.hxx
 *  Implementation of the parameter class ArgosDisplay.
 *  \author Stephan Meister
 *  \date 17.06.2010
 */

#ifndef _ARGOSDISPLAY_HXX_
#define _ARGOSDISPLAY_HXX_

#include <ArgosDisplay.h>
#include <sstream>
#include <MainWindow.hpp>
#include <ViewStack.hpp>
#include <QApplication>

//using namespace cimg_library ;
using namespace ArgosDisplay ;


template <typename T>
ArgosDisplayPlugin<T>::ArgosDisplayPlugin(const std::string& name) :
		TemplatedParameteredObject<T>("ArgosDisplay", name,
			"Advanced Display Plugin"),
			in(false, true),
			_mainWindow(0)
{
	ParameteredObject::_addInputSlot(
			in, "in",
			"The vigra::MultiArray<5, T> input.",
			"vigraArray5<T>");

	if(!qApp)
	{
		sout << "ArgosDisplayPlugin::No QApplication found! " 
			<< "ArgosDisplay can only be used in a Qt GUI Application!" << std::endl ;
		return ;
	}

	_mainWindow = new MainWindow ;
}

template <typename T>
ArgosDisplayPlugin<T>::~ArgosDisplayPlugin()
{
	delete _mainWindow ;
}

template <typename T>
void ArgosDisplayPlugin<T>::execute() {
	//#pragma warning(push)
	//#pragma warning(disable : 4244)

	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();
	
	//exit if QApplication is not running (when opened with command line charon)
	if(!qApp)
	{	return ;	}
	
	_mainWindow->show() ;
	
	for(std::size_t ii = 0 ; ii < in.size() ; ii++)
	{	_mainWindow->viewStack().linkImage(in[ii]) ;	}
	//#pragma warning(pop)
}

#endif /* _ARGOSDISPLAY_HXX_ */



