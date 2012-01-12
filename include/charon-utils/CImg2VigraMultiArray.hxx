/*  Copyright (C) 2009 Daniel Kondermann

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
/// @file CImg2VigraMultiArray.hxx
/// Implementation of the parameter class CImg2VigraMultiArray.
/// @author Daniel Kondermann
/// @date 19.01.2010

#ifndef _CIMG2VIGRAMULTIARRAY_HXX_
#define _CIMG2VIGRAMULTIARRAY_HXX_

#include "CImg2VigraMultiArray.h"

template <typename T>
CImg2VigraMultiArray<T>::CImg2VigraMultiArray(const std::string& name) :
		TemplatedParameteredObject<T>("CImg2VigraMultiArray", name,
			"Converts a CImgList-Object to a vigra::MultiArray<5, T>. "
			"The data is copied.")
{
	ParameteredObject::_addInputSlot(
			in, "in",
			"The CImg object to be converted", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			out, "out",
			"A copy of the image stored as vigra::MultiArray<5, T>",
			"vigraArray5<T>");
}

template <typename T>
void CImg2VigraMultiArray<T>::execute() {
	// store reference to input list to avoid function calls (-> slow)
	const cimg_library::CImgList<T>& i = in();
	vigra::MultiArray<5, T>& o = out();

	if(i.size() <= 0) {
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__ << ":\n\t";
		msg << "CImg2VigraMultiArray: Specified in-image list is empty!";
		throw std::runtime_error(msg.str());
	}

	const int dx = i[0].width();
	const int dy = i[0].height();
	const int dz = i[0].depth();
	const int dt = i[0].spectrum();
	const int dv = i.size();

	o.reshape(typename vigra::MultiArray<5, T>::difference_type(
			dx, dy, dz, dt, dv));

	// all output elements will be overwritten since CImgList does not
	// support images of different dimensions
	// (see e.g. header of some CImg file)

	cimglist_for(i, v) {
		// to make sure...
		assert(i[v].width()    == dx);
		assert(i[v].height()   == dy);
		assert(i[v].depth()    == dz);
		assert(i[v].spectrum() == dt);

		cimg_forXYZC(i[v], x, y, z, t) {
			o(x, y, z, t, v) = i(v, x, y, z, t);
		}
	}
}

#endif /* _CIMG2VIGRAMULTIARRAY_HXX_ */



