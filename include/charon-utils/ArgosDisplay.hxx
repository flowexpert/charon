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

#include <QApplication>
#include <MainWindow.hpp>
#include <ViewStack.hpp>
#include <sstream>
#include <charon-utils/ArgosDisplay.h>


//using namespace cimg_library ;
using namespace ArgosDisplay ;


template <typename T>
ArgosDisplayPlugin<T>::ArgosDisplayPlugin(const std::string& name) :
		TemplatedParameteredObject<T>("ArgosDisplay", name,
			"Advanced Display Plugin<br>Allows inspection and display of vigra and cimg images as well as the use of connected QWidget instances<br><br>"
			"Images with a size greater than 2 in the last dimension (CImgList.size() or vigra::MultiArray.size(0)) will be interpreted as RGB images<br>"
			"(can be switched via the context menu)<br><br>"
			"It's safe to use this plugin in a non-gui workflow (e.g. for use with command-line charon)<br>"
			"Although it's execution will just be omitted"),
			_vigraIn(true, true),
			_cimgIn(true, true),
			_widgets(true, true),
			_mainWindow(0)
{
	ParameteredObject::_addInputSlot(
			_vigraIn, "vigraIn",
			"Multislot for input images.<br>"
			"Will display the first slice of an array "
			"(meaning all dimensions except 0(width) and 1(height) are set to 0)",
			"vigraArray5<T>");

	ParameteredObject::_addInputSlot(
			_cimgIn, "cimgIn",
			"Multislot for input images.<br>"
			"Will display the first slice of an array "
			"(meaning all dimensions except 0(width) and 1(height) are set to 0)",
			"CImgList<T>");


	ParameteredObject::_addInputSlot(
			_widgets, "widgets",
			"QWidgets to display in Dock areas.",
			"QWidget*") ;

	if (!qobject_cast<QApplication*>(qApp)) {
		sout << "(WW) ArgosDisplayPlugin::No QApplication found!\n"
			<< "(WW) ArgosDisplay can only be used in a Qt GUI Application!\n"
			"(WW) (e.g. using tuchulcha-run)" << std::endl ;
		return ;
	}

	_mainWindow = new MainWindow(this->getName());
}

template <typename T>
ArgosDisplayPlugin<T>::~ArgosDisplayPlugin()
{
	delete _mainWindow ;
}

template <typename T>
void ArgosDisplayPlugin<T>::execute() {
	// exit if QApplication is not running
	// (when opened with command line charon)
	if(!qApp)
	{	return ;	}
	
	//std::map<const Array* const, std::string> parentNames ;
	// get pointers to all OutputSlots of the _in Multislot to get the names
	// of the corresponding Plugin Instances
	typename std::set<AbstractSlot<vigra::MultiArray<5, T> >*>
			::const_iterator it = _vigraIn.begin() ;
	typename std::set<AbstractSlot<vigra::MultiArray<5, T> >*>
			::const_iterator end = _vigraIn.end() ;

	ViewStack& viewStack = _mainWindow->viewStack() ;
	
	//save current top view to reset it in case of reexecution
	int index = viewStack.currentIndex() ;
	viewStack.clear() ;

	//register connected vigra images
	for( ; it != end ; it++)
	{
		std::string name = (*it)->getParent().getName() + "." + (*it)->getName() ; 
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
		viewStack.linkImage(new VigraPixelInspector<T>(temp->operator ()(), name)) ;
	}

	typename std::set<AbstractSlot<cimg_library::CImgList<T> >*>
			::const_iterator cit;
	typename std::set<AbstractSlot<cimg_library::CImgList<T> >*>
			::const_iterator cend = _cimgIn.end() ;
	
	//register connected cimg images
	for(cit = _cimgIn.begin(); cit != cend ; cit++) {
		std::string name = (*cit)->getParent().getName() + "."
				+ (*cit)->getName() ;
		const OutputSlot<cimg_library::CImgList<T> >* temp =
				reinterpret_cast<
				const OutputSlot<cimg_library::CImgList<T> >*>(*cit);
		if(!temp) {
			throw std::runtime_error(
					this->getClassName() + " : " + this->getName()
						+ " : cast of cimg_library::CImgList failed! "
					"In/Output slot \"" + name + "\" may be invalid!");
		}
		viewStack.linkImage(new CImgPixelInspector<T>(
				temp->operator()(), name)) ;
	}

	viewStack.setCurrentIndex(index) ;
	
	for (std::size_t ii = 0 ; ii < _widgets.size() ; ii++) {
		_mainWindow->addDockWidget(_widgets[ii]) ;
	}
}


AbstractPixelInspector::AbstractPixelInspector(
		const std::string& name) :
		name(name) {
}

template <typename T>
VigraPixelInspector<T>::VigraPixelInspector (
	const vigra::MultiArrayView<5, T>& mArray,
	const std::string& name) :
		AbstractPixelInspector(name),
		_mArray(mArray)
{
}

template <typename T>
const std::vector<int>& VigraPixelInspector<T>::dim() const
{
	static size_t maxDims = 5 ;
	static std::vector<int> result = std::vector<int>(maxDims,0) ;
	if(_mArray.size() == 0)
	{	result.assign(maxDims,0) ;	}
	else
	{
		result[0] = _mArray.size(0) ;
		result[1] = _mArray.size(1) ;
		result[2] = _mArray.size(2) ;
		result[3] = _mArray.size(3) ;
		result[4] = _mArray.size(4) ;
	}
	return result ;
}

template <typename T>
const std::vector<double>& VigraPixelInspector<T>::operator()(
		int x, int y) const {
	static size_t maxDimSize = 5 ; //don't return more values
	static std::vector<double> result = std::vector<double>(maxDimSize, 0.0);
	if(x < 0 || y < 0 || _mArray.size() == 0
			|| x >= _mArray.size(0) || y >= _mArray.size(1))
	{
		result.clear();
		return result;
	}

	size_t width = _mArray.size(4);
	if (result.size() != width) {
		result.resize(width <= maxDimSize ? width : maxDimSize);
	}
	
	for(size_t i = 0; i<(size_t)_mArray.size(4) && i<maxDimSize; i++) {
		result[i] = (double(_mArray(x,y,0,0,i)));
	}
	return result;
}

template <typename T>
const vigra::QRGBImage VigraPixelInspector<T>::getRGBImage() const{
	vigra::QRGBImage img(_mArray.size(0), _mArray.size(1)) ;
	if(_mArray.size(4) >= 3) {
		for (int xx = 0 ; xx < _mArray.size(0) ; ++xx) {
			for (int yy = 0 ; yy < _mArray.size(1) ; ++yy) {
				vigra::VigraQImage<vigra::QRGBValue<uchar> >::reference
						pixel = img(xx,yy);
				pixel.red() = _mArray(xx,yy,0,0,0);
				pixel.green() = _mArray(xx,yy,0,0,1);
				pixel.blue() = _mArray(xx,yy,0,0,2);
			}
		}
	}
	else {
		for (int xx = 0 ; xx < _mArray.size(0) ; ++xx)
			for (int yy = 0 ; yy < _mArray.size(1) ; ++yy) {
				vigra::VigraQImage<vigra::QRGBValue<uchar> >::reference
						pixel = img(xx,yy) ;
				pixel.red() = pixel.green() = pixel.blue() =
						_mArray(xx,yy,0,0,0) ;
			}
	}
		return img ;

}

template <typename T>
const vigra::FImage VigraPixelInspector<T>::getFImage() const{
	vigra::FImage img(_mArray.size(0), _mArray.size(1)) ;
	for(int xx = 0 ; xx < _mArray.size(0) ; ++xx)
		for(int yy = 0 ; yy < _mArray.size(1) ; ++yy)
		{	img(xx,yy) = _mArray(xx,yy,0,0,0) ;	}
	return img ;
}

template <typename T>
bool VigraPixelInspector<T>::isRGB() const {
	return _mArray.size(4) >= 3 ;
}

template <typename T>
CImgPixelInspector<T>::CImgPixelInspector (
		const cimg_library::CImgList<T>& mArray,
		const std::string& name) :
	AbstractPixelInspector(name),
	_mArray(mArray)
{
}


template <typename T>
const std::vector<double>& CImgPixelInspector<T>::operator()(
		int x, int y) const {
	static size_t maxDimSize = 5 ; //don't return more values
	static std::vector<double> result = std::vector<double>(maxDimSize, 0.0);
	
	if(x < 0 || y < 0 || _mArray.size() == 0 ||
			x >= _mArray(0).width() || y >= _mArray(0).height())
	{
		result.clear();
		return result;
	}	
	size_t width = _mArray.width();
	if (result.size() != width)
	{
		result.resize(width <= maxDimSize ? width : maxDimSize);
	}
	for (size_t i = 0; i<(size_t)_mArray.width() && i<maxDimSize; i++) {
		result[i] = (double(_mArray(i,x,y,0,0)));
	}
	return result ;

}

template <typename T>
const std::vector<int>& CImgPixelInspector<T>::dim() const
{
	static size_t maxDims = 5 ;
	static std::vector<int> result = std::vector<int>(maxDims,0) ;
	if(_mArray.size() == 0)
	{	result.assign(maxDims, 0) ;	}
	else
	{	result[0] = _mArray(0).width() ; result[1] = _mArray(0).height() ;
		result[2] = _mArray(0).depth() ; result[3] = _mArray(0).spectrum() ;
		result[4] = _mArray.width() ;
	}
	return result ;
}

template <typename T>
const vigra::QRGBImage CImgPixelInspector<T>::getRGBImage() const
{
	if(_mArray.size() <= 0)
	{	return vigra::QRGBImage(0,0) ;	}
	vigra::QRGBImage img(_mArray(0).width(), _mArray(0).height()) ;
	if(_mArray.width() >= 3)
	{	
		for(int xx = 0 ; xx < _mArray(0).width() ; ++xx)
			for(int yy = 0 ; yy < _mArray(0).height() ; ++yy)
			{	vigra::VigraQImage<vigra::QRGBValue<uchar> >::reference pixel = img(xx,yy) ;
				pixel.red() = _mArray(0,xx,yy,0,0) ;
				pixel.green() = _mArray(1,xx,yy,0,0) ;
				pixel.blue() = _mArray(2,xx,yy,0,0) ;
			}
	}
	else
	{	
		for(int xx = 0 ; xx < _mArray(0).width() ; ++xx)
			for(int yy = 0 ; yy < _mArray(0).height() ; ++yy)
			{	vigra::VigraQImage<vigra::QRGBValue<uchar> >::reference pixel = img(xx,yy) ;
				pixel.red() = pixel.green() = pixel.blue() = _mArray(0,xx,yy,0,0) ;
			}
	}
	return img ;

}

template <typename T>
const vigra::FImage CImgPixelInspector<T>::getFImage() const
{
	if(_mArray.size() <= 0)
	{	return vigra::FImage(0, 0) ;	}
	const cimg_library::CImg<T>& cimg = _mArray(0) ;
	vigra::FImage img(cimg.width(), cimg.height()) ;
	for(int xx = 0 ; xx < cimg.width() ; ++xx)
		for(int yy = 0 ; yy < cimg.height() ; ++yy)
		{	img(xx,yy) = cimg(xx,yy,0,0) ;	}
	return img ;
}

template <typename T>
bool CImgPixelInspector<T>::isRGB() const
{
	return _mArray.width() >= 3 ;
}

#endif /* _ARGOSDISPLAY_HXX_ */
