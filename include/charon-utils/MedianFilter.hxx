/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file MedianFilter.hxx
 *  Implementation of the parameter class MedianFilter.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 07.01.2011
 */

#ifndef _NORMALIZE_HXX_
#define _NORMALIZE_HXX_

#include "MedianFilter.h"

template<typename T>
MedianFilter<T>::MedianFilter(const std::string& name) :
	TemplatedParameteredObject<T>(
			"MedianFilter", name,
			"calculate median of image windows<br><br>"
			"Use image windows of given size, calculate median within this "
			"window and use the result as new value for the window center "
			"pixel. This eliminates outliers and makes "
			"e.g. flow estimation more robust.")
{
	this->_addInputSlot(in,    "in",    "image input",  "CImgList<T>");
	this->_addOutputSlot(out,  "out",   "image output", "CImgList<T>");
	this->_addParameter(
			windowRadius, "windowRadius",
			"radius r of image windows (size is 2*r+1)",  7u);
}

template<typename T>
void MedianFilter<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	const cimg_library::CImgList<T>& i = in();
	cimg_library::CImgList<T> o = out();
	const uint& r = windowRadius();
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

#endif /* _NORMALIZE_HXX_ */
