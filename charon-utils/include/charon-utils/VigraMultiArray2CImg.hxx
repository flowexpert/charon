/*  Copyright (C) 2009 Daniel Kondermann

    This file is part of Charon.

    Charon is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Charon is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/// @file VigraMultiArray2CImg.hxx
/// Implementation of the parameter class VigraMultiArray2CImg.
/// @author Daniel Kondermann
/// @date 19.01.2010

#ifndef _VIGRAMULTIARRAY2CIMG_HXX_
#define _VIGRAMULTIARRAY2CIMG_HXX_

#include "VigraMultiArray2CImg.h"
#include <charon-core/SplitStream.h>

template <typename T>
VigraMultiArray2CImg<T>::VigraMultiArray2CImg(const std::string& name) :
		TemplatedParameteredObject<T>("VigraMultiArray2CImg", name,
			"Converts a vigra::MultiArray<5, T> to a CImgList-Object. "
			"The data is copied for compatibility reasons. Please note that "
			"this conversion is not always possible if the MultiArray is "
			"strided or has too many dimensions! Dimensions are mapped the "
			"following way: 0->X, 1->Y, 2->Z, 3->V, 4->ListIndex.")
{
	ParameteredObject::_setTags("charon-utils;CImg;Vigra") ;

	ParameteredObject::_addInputSlot(
			in, "in",
			"The vigra::MultiArray<5, T> object to be converted.",
			"vigraArray5<T>");
	ParameteredObject::_addOutputSlot(
			out, "out",
			"A copy of the same image in CImgList<T> data format.",
			"CImgList<T>");
}

template <typename T>
void VigraMultiArray2CImg<T>::execute() {
	// store references to input/output to avoid function calls (-> slow)
	const vigra::MultiArrayView<5, T>& i = in();
	cimg_library::CImgList<T>& o = out();

	sout << "\tData shape: " << i.size(0) << "x" << i.size(1) << "x"
		 << i.size(2) << "x" << i.size(3) << " x " << i.size(4) << std::endl;

	o.assign(i.size(4), i.size(0), i.size(1), i.size(2), i.size(3));
	cimglist_for(o, v) {
		cimg_forXYZC(o[v], x, y, z, t) {
			o(v, x, y, z, t) = i(x, y, z, t, v);
		}
	}
}

#endif /* _VIGRAMULTIARRAY2CIMG_HXX_ */



