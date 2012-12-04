/*      Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

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
/** \file LayeredMedianFilter.hxx
 *  Implementation of the parameter class LayeredMedianFilter.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 01.02.2012
 */

#ifndef _LAYEREDMEDIANFILTER_HXX_
#define _LAYEREDMEDIANFILTER_HXX_

#include "LayeredMedianFilter.h"

template<typename T>
LayeredMedianFilter<T>::LayeredMedianFilter(const std::string& name) :
	TemplatedParameteredObject<T>(
			"LayeredMedianFilter", name,
			"calculate median of image windows<br><br>"
			"Use image windows of given size, calculate median within this "
			"window and use the result as new value for the window center "
			"pixel. This eliminates outliers and makes "
			"e.g. flow estimation more robust."),
	windowRadiusList("0;0;1;1;2;2")
{
	ParameteredObject::_setTags("charon-utils;CImg") ;

	this->_addInputSlot(in,    "in",    "image input",   "CImgList<T>");
	ParameteredObject::_addInputSlot(layer, "layer", "current layer");
	this->_addOutputSlot(out,  "out",   "image output",  "CImgList<T>");
	this->_addParameter(
			windowRadiusList, "windowRadiusList",
			"list of radius r of image windows (size is 2*r+1)",
			"T_list");
	ParameteredObject::_setTags("deprecated");
}

template<typename T>
void LayeredMedianFilter<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	const cimg_library::CImgList<T>& i = in();
	cimg_library::CImgList<T>& o = out();
	const unsigned int& r = windowRadiusList()[layer()];
	o = i;

	cimg_library::CImg<T> window;
	T cur;
	cimglist_for(o,kk) {
		cimg_forXYZC(o[kk],xx,yy,zz,tt) {
			window = i[kk].get_crop(xx-r,yy-r,zz,tt,xx+r,yy+r,zz,tt,true);
			cur = window.median();
			o(kk,xx,yy,zz,tt) = cur;
		}
	}
}

#endif /* _LAYEREDMEDIANFILTER_HXX_ */

