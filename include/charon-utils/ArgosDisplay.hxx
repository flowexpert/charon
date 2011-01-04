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

#include <charon-utils/ArgosDisplay.h>
#include <sstream>
#include <MainWindow.hpp>
#include <ViewStack.hpp>
#include <QApplication>

//using namespace cimg_library ;
using namespace ArgosDisplay ;


template <typename T>
ArgosDisplayPlugin<T>::ArgosDisplayPlugin(const std::string& name) :
		TemplatedParameteredObject<T>("ArgosDisplay", name,
			"Advanced Display Plugin<br>Allows inspection and display of "),
			_in(false, true),
			_widgets(true, true),
			_mainWindow(0)
{
	ParameteredObject::_addInputSlot(
			_in, "in",
			"Multislot for input images.<br>"
			"Will display the first slice of an array (meaning all dimensions except 0 and 1 are set to 0)",
			"vigraArray5<T>");

	ParameteredObject::_addInputSlot(
			_widgets, "widgets",
			"QWidgets to display in Dock areas.",
			"QWidget*") ;

	ParameteredObject::_addParameter<bool>(_inputIsRGB, "inputIsRGB",
		"try to interpret input as 8bit (0-255) RGB images if 3. dimension is exaclty of size 3,<br"
		"otherwise use grayscale", false, "bool");


	if(!qApp)
	{
		sout << "ArgosDisplayPlugin::No QApplication found! " 
			<< "ArgosDisplay can only be used in a Qt GUI Application! (e.g. Tuchulcha)" << std::endl ;
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

	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();
	
	//exit if QApplication is not running (when opened with command line charon)
	if(!qApp)
	{	return ;	}
	
	_mainWindow->show() ;
	
	//std::map<const Array* const, std::string> parentNames ;
	//get pointers to all OutputSlots of the _in Multislot to get the names of the corresponding Plugin Instances
        typename std::set<AbstractSlot<vigra::MultiArrayView<5, T> >*>::const_iterator it = _in.begin() ;
        typename std::set<AbstractSlot<vigra::MultiArrayView<5, T> >*>::const_iterator end = _in.end() ;

        for( ; it != end ; it++)
	{
		std::string name = (*it)->getParent().getName() ;
                OutputSlot<vigra::MultiArrayView<5, T> >* temp = dynamic_cast< OutputSlot<vigra::MultiArrayView<5, T> >*>(*it);
                if(!temp)
                {   throw std::runtime_error("cast of vigra::MultiArrayView failed! In/Output slot may be invalid!") ;  }

		//register all Arrays with the ViewStack
                _mainWindow->viewStack().linkImage(temp->operator ()(), temp->getType(), name, _inputIsRGB()) ;
        }
	
	for(std::size_t ii = 0 ; ii < _widgets.size() ; ii++)
	{	
		_mainWindow->addDockWidget(_widgets[ii]) ;
	}

}

#endif /* _ARGOSDISPLAY_HXX_ */



