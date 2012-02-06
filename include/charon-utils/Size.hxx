/*  Copyright (C) 2012 Stephan Meister

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
/** \file Size.hxx
 *  Implementation of the parameter class Size.
 *  \author <a href="mailto:stephan.meister@iwr.uni-heidelberg.de">
 *      Stephan Meister</a>
 *  \date 03.02.2012
 */

#ifndef _SIZE_HXX_
#define _SIZE_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "Size.h"

template <typename T>
Size<T>::Size(const std::string& name) :
		TemplatedParameteredObject<T>(
			"Size", name,
			"<h2>Size</h2><br>"
			"get Size of input object and export is as an ROI instance (e.g. "
			"for further use in the crop or resize plugins)<br><br>"
			"Usage example:<br>If you want to double the size of an image, "
			"connect it to the \"image input slot of this plugin\" "
			"and to the input of an \"resize plugin\".<br>"
			"Leave the offset slot unconnected,set resize factor to 2.0 "
			"and connect the roi output to the roi input of the resize plugin."
		),
		offset(true,false)
{
	ParameteredObject::_addInputSlot(
		image, "image",
		"input image"
		"The resulting ROI will have the same dimensions as this image",
		"CImgList<T>");
	ParameteredObject::_addInputSlot(
		offset, "offset",
		"offset ROI<br>"
		"If connected all starting values in this ROI will be added to the resulting ROI and<br>"
		"all end values will be subtraced from the resulting ROI.<br>"
		"The \"resize factor\" parameter is ignored when this slot is connected<br>"
		"Usefull in combination with the cut plugin: <br>"
		"To remove a fixed with border around an image set the values in offset to positive,<br>"
		"or to add a black border around an image set the values to negative.<br>"
		"BUG :Currently all borders must have the same size!",
		"Roi<int>*");

	ParameteredObject::_addOutputSlot(
		roi, "roi",
		"ROI(Region of Interest)",
		"Roi<int>*");
	roi() = 0 ;

	ParameteredObject::_addParameter< double >(
		resizeFactor, "resizeFactor",
		"The Size of the resulting ROI will be multiplied with this factor"
		"(when offset is not connected)<br>"
		"Usefull if you want to resize an image by a fixed factor",
		1.0, "double");
}

template <typename T>
void Size<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	roi() = &_roi ;

	const cimg_library::CImgList<T>& in = image ;

	_roi.xBegin() = _roi.yBegin() = 0 ;
	_roi.zBegin() = _roi.tBegin() = _roi.vBegin() = 0 ;
	_roi.xEnd() = _roi.yEnd() = _roi.zEnd() = _roi.tEnd() = _roi.vEnd() = 0 ;
	if(!offset.connected())
	{
		if(in.width())
		{	_roi.vEnd() = floor(in.width() * resizeFactor() + .5) ;
			_roi.xEnd() = floor(in[0].width() * resizeFactor() + .5) ;
			_roi.yEnd() = floor(in[0].height() * resizeFactor() + .5) ;
			_roi.zEnd() = floor(in[0].depth() * resizeFactor() + .5) ;
			_roi.tEnd() = floor(in[0].spectrum() * resizeFactor() + .5) ;
		}
		else
		{	sout << "Input has zero-width!" << std::endl ; }
	}
	else
	{
		if(offset() == 0)
			ParameteredObject::raise("Pointer to \"offset\" ROI is zero!"
				"Connect a valid object!") ;
		const Roi<int>& of = *offset() ;
		if(in.width())
		{	
			_roi.xBegin() = of.xBegin() ;
			_roi.yBegin() = of.yBegin() ;
			_roi.zBegin() = of.zBegin() ;
			_roi.tBegin() = of.tBegin() ;
			_roi.vBegin() = of.vBegin() ;
			_roi.xEnd() = in[0].width() - of.xEnd() ;
			_roi.yEnd() = in[0].height() - of.yEnd() ;
			_roi.zEnd() = in[0].depth() - of.zEnd() ;
			_roi.tEnd() = in[0].spectrum() - of.tEnd() ;
			_roi.vEnd() = in.width() - of.vEnd() ;
		}
		else
		{	sout << "Input has zero-width!" << std::endl ; }
	}
	sout << "Using ROI: " << _roi << std::endl ;
}

#endif /* _SIZE_HXX_ */
