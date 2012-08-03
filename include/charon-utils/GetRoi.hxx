/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

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
/** \file GetRoi.hxx
 *  Implementation of the parameter class GetRoi.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 03.08.2012
 */

#ifndef _GETROI_HXX_
#define _GETROI_HXX_

#include "GetRoi.h"

template <typename T>
GetRoi<T>::GetRoi(const std::string& name) :
		TemplatedParameteredObject<T>("GetRoi", name,
			"Returns the dimensions of the input sequence as a region of interest.")
{
	ParameteredObject::_addInputSlot(
			seqIn, "seqIn", "sequence input", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			size, "size", "current size", "Roi<int>*");
}

template <typename T>
void GetRoi<T>::execute() {
	size() = &_size;

	cimg_library::CImgList<T> si = seqIn();
	_size.vEnd() = si.size();
	_size.xEnd() = si[0].width();
	_size.yEnd() = si[0].height();
	_size.zEnd() = si[0].depth();
	_size.tEnd() = si[0].spectrum();

}
#endif /* _GETROI_HXX_ */

