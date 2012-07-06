/*  Copyright (C) 2011 Julian Coordts

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
/** \file EmptyCImg.hxx
 *  Implementation of the class EmptyCImg.
 *  \author Julian Coordts
 *  \date 13.01.2012
 */


#ifndef _EMPTYCIMG_HXX_
#define _EMPTYCIMG_HXX_

#include "EmptyCImg.h"
#include <math.h>
using namespace cimg_library ;

template <typename T>
EmptyCImg<T>::EmptyCImg(const std::string& name) :
		TemplatedParameteredObject<T>("emptycimg", name,
			"create an empty CImgList with given dimensions.<br>"
			"CImgList will have dimensions c X x X y X z X t<br>"
			"Individual CImg's will have dimension x X y X z X t."),
		roi(true, false)
{
	ParameteredObject::_addParameter (_sizeX, "sizex", 
		"size in x direction (width)", 1) ;
	
	ParameteredObject::_addParameter (_sizeY, "sizey", 
		"size in y direction(height)",1) ;
	
	ParameteredObject::_addParameter (_sizeZ, "sizez", 
		"size in z direction (depth)", 1) ;

	ParameteredObject::_addParameter (_sizeT, "sizet", 
		"size in t direction (time or spectrum)", 1) ;

	ParameteredObject::_addParameter (_sizeC, "sizec", 
		"size in c direction(width of the List, 3 for RGB images)",1);

	ParameteredObject::_addParameter (_value, "value",
		"value of all pixels in output image", T(0)) ;

	ParameteredObject::_addInputSlot(roi, "roi",
		"region of interest (to use this plugin within pyramids)", "Roi<int>*");

	ParameteredObject::_addOutputSlot(_output, "output", 
		"output image", "CImgList<T>"); 
}

template <typename T>
void EmptyCImg<T>::execute() {
	CImgList<T>& output = _output() ;
	if (roi.connected()) {
		const Roi<int> & _roi = *(this->roi());
		int size = _roi.vEnd - _roi.vBegin;
		int width = _roi.xEnd - _roi.xBegin; 
		int height = _roi.yEnd - _roi.yBegin;
		int depth = _roi.zEnd - _roi.zBegin;
		int spectrum = _roi.tEnd - _roi.tBegin;
		output.assign(size, width, height, depth, spectrum, _value());
	} else {
		output.assign(_sizeC(),_sizeX(),_sizeY(),_sizeZ(),_sizeT(), _value());
	}
}

#endif /* _EmptyCImg_HXX_ */

