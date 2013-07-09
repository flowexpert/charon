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


using namespace ArgosDisplay ;

namespace ArgosDisplay {
	/// compare object to sort _inspectors
	class charon_core_LOCAL InspectorSort {
	public:
		/// comparison based on inspector name
		inline bool operator()(const AbstractPixelInspector*, const AbstractPixelInspector*) const;
	};
}

template <typename T>
ArgosDisplayPlugin<T>::ArgosDisplayPlugin(const std::string& name) :
		TemplatedParameteredObject<T>("ArgosDisplay", name,
			"Advanced Display Plugin<br>Allows inspection and display of "
			"vigra and cimg images as well as the use "
			"of connected QWidget instances<br><br>"
			"The window layout can be changed via the \"Layout\" menu. If multiple tabs are open, image views can moved between them by dragging and dropping them FROM the tab bar."
			"Context menu actions and key shortcuts always act upon the view the mouse is currently hovering over.<br><br>"
			"The following commands are available:<br><ul>"
			"<li><strong>Greyscale</strong>: Normalize and display image as greyscale. If the image has multiple dimensions/channels, only the first will be shown.</li>." 
			"<li><strong>Table</strong>: Display all values as a table."
			"If the image has multiple dimensions/channels, only the first will be shown."
			"This can be slow for images with more than 10000 pixels.</li>"
			"Images with a size greater than 2 in the last dimension "
			"<li><strong>4. Dim as RGB channels</strong>: If the t (4.) dimension of the image has at least size 3 "
			"interpret individual slices as RGB channels. Values must be between 0-255. Use the \"normalize\" or other visualization plugins such as \"CustomColorMask\" if not so.</li>"
			"<li><strong>5. Dim as RGB channels</strong>: If the v (5.) dimension of the image has at least size 3 "
			"interpret individual slices as RGB channels. Values must be between 0-255. Use the \"normalize\" or other visualization plugins such as \"CustomColorMask\" if not so.</li>"
			"</ul><ul>"
			"<li><strong>Save current view</strong>: Save the current view as seen on screen to image file. Supported formats are jpg, png and bmp.</li>"
			"<li><strong>Reset view</strong>: If not in Table mode, set the zoom level to 1 and center the image.</li>"
			"<li><strong>Switch log mode</strong>: If in greyscale mode, take logarithm of values before performing normalization. This is usefull to reduce the dynamic range of the display.</li>"
			"<li><strong>Align other views</strong>: Set the zoom level and image position of all images in the current view to to same values as the active image."
			"This performs a pixel exakt alignment of images, given they have the same width and height.</li>"
			"</ul>"
			"Additionaly, the current top image can be switched using the number keys."
			),
			_vigraIn(true, true),
			_cimgIn(true, true),
			_dockWidgets(true, true),
			_overlayWidgets(true, true),
			_mainWindow(0),
			_displayReloader(0)
{
	ParameteredObject::_setTags("charon-utils;Qt;CImg;Vigra") ;
	
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
			_dockWidgets, "dockWidgets",
			"QWidgets to display in Dock areas.",
			"QWidget*") ;
	ParameteredObject::_addInputSlot(
			_overlayWidgets, "overlayWidgets",
			"QWidgets to lay over displayed image.",
			"QWidget*") ;

	ParameteredObject::_addParameter(
			timeout, "timeout",
			"time interval for display reloading (0 disables)", 0);

	if (!qobject_cast<QApplication*>(qApp)) {
		sout << "(WW) ArgosDisplayPlugin::No QApplication found!\n"
			<< "(WW) ArgosDisplay can only be used in a Qt GUI Application!\n"
			"(WW) (e.g. using tuchulcha-run)" << std::endl ;
		return ;
	}

}

template <typename T>
ArgosDisplayPlugin<T>::~ArgosDisplayPlugin()
{
	delete _displayReloader;
	delete _mainWindow ;

	for(size_t ii = 0 ; ii < _inspectors.size() ; ii++)
	{
		delete _inspectors[ii] ;
	}
	_inspectors.clear() ;
}

bool ArgosDisplay::InspectorSort::operator ()(
		const AbstractPixelInspector* i1, const AbstractPixelInspector* i2) const {
	return (i1->name.compare(i2->name) < 0);
}

template <typename T>
void ArgosDisplayPlugin<T>::execute() {
	// exit if QApplication is not running
	// (when opened with command line charon without GUI)
	if(!qApp){
		return ;
	}
	if(!_mainWindow)
	{	_mainWindow = new MainWindow(this->getName());
		//move window to top left corner so that the maximize/minimize buttons
		//are not hidden for big images
		_mainWindow->move(10,10) ;
	}

	if(!_displayReloader)
		_displayReloader = new ArgosDisplayReloader(this);

	// setup timer for display reloading
	_displayReloader->setTimeout( timeout() );

	//std::map<const Array* const, std::string> parentNames ;
	// get pointers to all OutputSlots of the _in Multislot to get the names
	// of the corresponding Plugin Instances
	typename std::set<AbstractSlot<vigra::MultiArray<5, T> >*>
			::const_iterator it = _vigraIn.begin() ;
	typename std::set<AbstractSlot<vigra::MultiArray<5, T> >*>
			::const_iterator end = _vigraIn.end() ;

	ViewStack& viewStack = _mainWindow->viewStack() ;
	
	// save current top view to reset it in case of reexecution
	int index = viewStack.currentIndex();
	// save current zoom level to restore it on reload
	viewStack.setZoomLevel(viewStack.getZoomLevel());
	viewStack.clear() ;

	for(size_t ii = 0 ; ii < _inspectors.size() ; ii++)
	{
		delete _inspectors[ii] ;
	}
	_inspectors.clear() ;

	// register connected vigra images
	for( ; it != end ; it++) {
		std::string name = (*it)->getParent().getName() + "." + (*it)->getName() ;
		// register all Arrays with the ViewStack
		AbstractPixelInspector* inspector = new VigraPixelInspector<T>(
					_vigraIn.getDataFromOutputSlot(*it), name);
		if (inspector) {
			_inspectors.push_back(inspector) ;
		}
	}

	typename std::set<AbstractSlot<cimg_library::CImgList<T> >*>
			::const_iterator cit;
	typename std::set<AbstractSlot<cimg_library::CImgList<T> >*>
			::const_iterator cend = _cimgIn.end() ;
	
	// register connected cimg images
	for(cit = _cimgIn.begin(); cit != cend ; cit++) {
		std::string name = (*cit)->getParent().getName() + "."
				+ (*cit)->getName() ;
		AbstractPixelInspector* inspector = new CImgPixelInspector<T>(
				_cimgIn.getDataFromOutputSlot(*cit), name) ;
		if(inspector) {
			_inspectors.push_back(inspector) ;
		}
	}

	// sort connected images by their slot name
	std::sort(_inspectors.begin(), _inspectors.end(), InspectorSort());
	for (std::vector<AbstractPixelInspector*>::const_iterator iIter =_inspectors.begin();
				iIter != _inspectors.end(); iIter++) {
		viewStack.linkImage(*iIter);
	}

	viewStack.setCurrentIndex(index) ;

	
	for (std::size_t ii = 0 ; ii < _dockWidgets.size() ; ii++) {
		_mainWindow->addDockWidget(_dockWidgets[ii]) ;
	}
	for (std::size_t ii = 0 ; ii < _overlayWidgets.size() ; ii++) {
		_mainWindow->addOverlayWidget(_overlayWidgets[ii]) ;
	}
	
}

template<typename T>
void ArgosDisplayPlugin<T>::run() {
	ParameteredObject::run();
	// start timer at execution end to avoid flooding of the event loop
	_displayReloader->start();
	QApplication::processEvents() ;
	/*
	for (std::size_t ii = 0 ; ii < _overlayWidgets.size() ; ii++) {
		_mainWindow->addOverlayWidget(_overlayWidgets[ii]) ;
	}*/
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
const vigra::QRGBImage VigraPixelInspector<T>::getRGBImage(RGBChannels mode) const{
	vigra::QRGBImage img(_mArray.size(0), _mArray.size(1)) ;
	if(mode == RGB4 && _mArray.size(4) >= 3) {
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
	else if(mode == RGB3 && _mArray.size(3) >= 3) {
		for (int xx = 0 ; xx < _mArray.size(0) ; ++xx) {
			for (int yy = 0 ; yy < _mArray.size(1) ; ++yy) {
				vigra::VigraQImage<vigra::QRGBValue<uchar> >::reference
						pixel = img(xx,yy);
				pixel.red() = _mArray(xx,yy,0,0,0);
				pixel.green() = _mArray(xx,yy,0,1,0);
				pixel.blue() = _mArray(xx,yy,0,2,0);
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
RGBChannels VigraPixelInspector<T>::isRGB() const {
	if(_mArray.size(4) >= 3)
		return RGB4 ;
	if(_mArray.size(3) >= 3)
		return RGB3 ;
	return NONE ;

}

template <typename T>
bool VigraPixelInspector<T>::isEmpty() const {
	return _mArray.size() <= 0 ;
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
const vigra::QRGBImage CImgPixelInspector<T>::getRGBImage(RGBChannels mode) const
{
	if(_mArray.size() <= 0)
	{	return vigra::QRGBImage(0,0) ;	}
	vigra::QRGBImage img(_mArray(0).width(), _mArray(0).height()) ;
	if(mode == RGB4 &&_mArray.width() >= 3)
	{	
		for(int xx = 0 ; xx < _mArray(0).width() ; ++xx)
			for(int yy = 0 ; yy < _mArray(0).height() ; ++yy)
			{	vigra::VigraQImage<vigra::QRGBValue<uchar> >::reference pixel = img(xx,yy) ;
				pixel.red() = _mArray(0,xx,yy,0,0) ;
				pixel.green() = _mArray(1,xx,yy,0,0) ;
				pixel.blue() = _mArray(2,xx,yy,0,0) ;
			}
	}
	else if(mode == RGB3 && _mArray(0).spectrum() >= 3)
	{	
		for(int xx = 0 ; xx < _mArray(0).width() ; ++xx)
			for(int yy = 0 ; yy < _mArray(0).height() ; ++yy)
			{	vigra::VigraQImage<vigra::QRGBValue<uchar> >::reference pixel = img(xx,yy) ;
				pixel.red() = _mArray(0,xx,yy,0,0) ;
				pixel.green() = _mArray(0,xx,yy,0,1) ;
				pixel.blue() = _mArray(0,xx,yy,0,2) ;
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
RGBChannels CImgPixelInspector<T>::isRGB() const
{
	if(_mArray.width() >= 3)
		return RGB4 ;
	if(_mArray.width() > 0 && _mArray(0).spectrum() >= 3)
		return RGB3 ;
	return NONE ;
}

template <typename T>
bool CImgPixelInspector<T>::isEmpty() const
{
	return _mArray.size() <= 0 || _mArray[0].size() <= 0 ;
}


#endif /* _ARGOSDISPLAY_HXX_ */
