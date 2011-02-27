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
			_vigraIn(false, true),
			_cimgIn(false, true),
			_widgets(true, true),
			_mainWindow(0)
{
	ParameteredObject::_addInputSlot(
			_vigraIn, "vigraIn",
			"Multislot for input images.<br>"
			"Will display the first slice of an array "
			"(meaning all dimensions except 0 and 1 are set to 0)",
			"vigraArray5<T>");

	ParameteredObject::_addInputSlot(
			_cimgIn, "cimgIn",
			"Multislot for input images.<br>"
			"Will display the first slice of an array "
			"(meaning all dimensions except 0 and 1 are set to 0)",
			"CImgList<T>");


	ParameteredObject::_addInputSlot(
			_widgets, "widgets",
			"QWidgets to display in Dock areas.",
			"QWidget*") ;

	ParameteredObject::_addParameter<bool>(
			_inputIsRGB, "inputIsRGB",
			"try to interpret input as 8bit (0-255) RGB images if last "
			"dimension is exaclty of size 3<br>"
			"(otherwise use grayscale)", false, "bool");


	if(!qApp)
	{
		sout << "ArgosDisplayPlugin::No QApplication found! " 
			<< "ArgosDisplay can only be used in a Qt GUI Application! "
			"(e.g. Tuchulcha)" << std::endl ;
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
	
	// exit if QApplication is not running
	// (when opened with command line charon)
	if(!qApp)
	{	return ;	}
	
	//std::map<const Array* const, std::string> parentNames ;
	// get pointers to all OutputSlots of the _in Multislot to get the names
	// of the corresponding Plugin Instances
	typename std::set<AbstractSlot<vigra::MultiArrayView<5, T> >*>
			::const_iterator it = _vigraIn.begin() ;
	typename std::set<AbstractSlot<vigra::MultiArrayView<5, T> >*>
			::const_iterator end = _vigraIn.end() ;

	ViewStack& viewStack = _mainWindow->viewStack() ;
	
	//save current top view to reset it in case of reexecution
	int index = viewStack.currentIndex() ;
	viewStack.clear() ;

	for( ; it != end ; it++)
	{
		std::string name = (*it)->getParent().getName() ;
		// dynamic_cast fails for unknown reasons, therefore this
		// horrible piece of code
		const OutputSlot<vigra::MultiArrayView<5, T> >* temp =
				reinterpret_cast<
				const OutputSlot<vigra::MultiArrayView<5, T> >*>(*it);
		if(!temp)
			vigra_fail(
					"cast of vigra::MultiArrayView failed! "
					"In/Output slot may be invalid!");

		// register all Arrays with the ViewStack
		viewStack.linkImage(new VigraPixelInspector<T>(temp->operator ()(), name, _inputIsRGB())) ;
	}
	typename std::set<AbstractSlot<cimg_library::CImgList<T> >*>
			::const_iterator cit = _cimgIn.begin() ;
	typename std::set<AbstractSlot<cimg_library::CImgList<T> >*>
			::const_iterator cend = _cimgIn.end() ;
	
	for( ; cit != cend ; cit++)
	{
		std::string name = (*cit)->getParent().getName() ;
		const OutputSlot<cimg_library::CImgList<T> >* temp =
				reinterpret_cast<
				const OutputSlot<cimg_library::CImgList<T> >*>(*cit);
		if(!temp)
			throw std::runtime_error("cast of cimg_library::CImgList failed! "
				"In/Output slot may be invalid!");
		viewStack.linkImage(new CImgPixelInspector<T>(temp->operator ()(), name, _inputIsRGB())) ;
	}

	viewStack.setCurrentIndex(index) ;
	
	for(std::size_t ii = 0 ; ii < _widgets.size() ; ii++)
	{
		_mainWindow->addDockWidget(_widgets[ii]) ;
	}

}


AbstractPixelInspector::AbstractPixelInspector(const std::string& name, bool rgb) :
	name(name),
	isRGBA(rgb)
{
	;
}

template <typename T>
VigraPixelInspector<T>::VigraPixelInspector (const vigra::MultiArrayView<5, T>& mArray, 
	const std::string& name, bool rgb) :
	AbstractPixelInspector(name, rgb),
	_mArray(mArray)
{
		;
}

template <typename T>
const std::vector<double> VigraPixelInspector<T>::operator()(int x, int y) const
{
	static int maxDimSize = 5 ; //don't return more values
	std::vector<double> result ;
	result.reserve(maxDimSize) ;
	if(x < 0 || y < 0 || _mArray.size() == 0 || x >= _mArray.size(0) || y >= _mArray.size(1))
		return result ;
	
	for(int i = 0 ; i < _mArray.size(4) && i < maxDimSize ; i++)
		result.push_back(double(_mArray(x,y,0,0,i))) ;
	return result ;

}

template <typename T>
const vigra::QRGBImage VigraPixelInspector<T>::getRGBAImage()
{
	if(isRGBA && _mArray.size(4) == 3)
	{	vigra::QRGBImage img(_mArray.size(0), _mArray.size(1)) ;
		for(int xx = 0 ; xx < _mArray.size(0) ; ++xx)
			for(int yy = 0 ; yy < _mArray.size(1) ; ++yy)
			{	img(xx,yy).red() = _mArray(xx,yy,0,0,0) ;
				img(xx,yy).green() = _mArray(xx,yy,0,0,1) ;
				img(xx,yy).blue() = _mArray(xx,yy,0,0,2) ;
			}
		return img ;
	}
	else
	{	return vigra::QRGBImage(1, 1) ;	}

}

template <typename T>
const vigra::FImage VigraPixelInspector<T>::getFImage()
{
	vigra::FImage img(_mArray.size(0), _mArray.size(1)) ;
	for(int xx = 0 ; xx < _mArray.size(0) ; ++xx)
		for(int yy = 0 ; yy < _mArray.size(1) ; ++yy)
		{	img(xx,yy) = _mArray(xx,yy,0,0,0) ;	}
	return img ;
}

template <typename T>
CImgPixelInspector<T>::CImgPixelInspector (const cimg_library::CImgList<T>& mArray, 
	const std::string& name, bool rgb) :
	AbstractPixelInspector(name, rgb),
	_mArray(mArray)
{
		;
}


template <typename T>
const std::vector<double> CImgPixelInspector<T>::operator()(int x, int y) const
{
	static int maxDimSize = 5 ; //don't return more values
	std::vector<double> result ;
	result.reserve(maxDimSize) ;
	if(x < 0 || y < 0 || _mArray.size() == 0 || x >= _mArray(0).width() || y >= _mArray(0).height())
		return result ;
	
	for(int i = 0 ; i < _mArray.width() && i < maxDimSize ; i++)
		result.push_back(double(_mArray(i,x,y,0,0))) ;
	return result ;

}

template <typename T>
const vigra::QRGBImage CImgPixelInspector<T>::getRGBAImage()
{
	if(isRGBA && _mArray.width() == 3)
	{	vigra::QRGBImage img(_mArray(0).width(), _mArray(0).height()) ;
		for(int xx = 0 ; xx < _mArray(0).width() ; ++xx)
			for(int yy = 0 ; yy < _mArray(0).height() ; ++yy)
			{	img(xx,yy).red() = _mArray(0,xx,yy,0,0) ;
				img(xx,yy).green() = _mArray(1,xx,yy,0,0) ;
				img(xx,yy).blue() = _mArray(2,xx,yy,0,0) ;
			}
		return img ;
	}
	else
	{	return vigra::QRGBImage(1, 1) ;	}

}

template <typename T>
const vigra::FImage CImgPixelInspector<T>::getFImage()
{
	const cimg_library::CImg<T>& cimg = _mArray(0) ;
	vigra::FImage img(cimg.width(), cimg.height()) ;
	for(int xx = 0 ; xx < cimg.width() ; ++xx)
		for(int yy = 0 ; yy < cimg.height() ; ++yy)
		{	img(xx,yy) = cimg(xx,yy,0,0) ;	}
	return img ;
}


#endif /* _ARGOSDISPLAY_HXX_ */
