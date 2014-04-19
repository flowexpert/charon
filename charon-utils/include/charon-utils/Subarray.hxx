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
/** \file Subarray.hxx
 *  Implementation of the parameter class Subarray.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 25.01.2011
 */

#ifndef _SUBARRAY_HXX_
#define _SUBARRAY_HXX_

#include "Subarray.h"
#include "Roi.hxx"

template <typename T>
Subarray<T>::Subarray(const std::string& name) :
	TemplatedParameteredObject<T>("subarray", name,
		"vigra implementation of crop")
{
	ParameteredObject::_setTags("charon-utils;Vigra") ;

	ParameteredObject::_addInputSlot(
			in,"in","data input","vigraArray5<T>");
	ParameteredObject::_addOutputSlot(
			out,"out","data output","vigraArray5<T>");

	ParameteredObject::_addInputSlot(roi,"roi","crop region","Roi<int>*");

	ParameteredObject::_addParameter(x, "x", "crop in x direction", true);
	ParameteredObject::_addParameter(y, "y", "crop in y direction", true);
	ParameteredObject::_addParameter(z, "z", "crop in z direction", true);
	ParameteredObject::_addParameter(t, "t", "crop in t direction", true);
	ParameteredObject::_addParameter(v, "v", "crop in v direction", true);
}

template <typename T>
void Subarray<T>::execute() {
	const Roi<int>& r = *roi();

	typedef vigra::MultiArrayShape<5>::type Shape;
	Shape orig = in().shape();
	out() = in().subarray(
			Shape(r.xBegin,r.yBegin,r.zBegin,r.tBegin,r.vBegin),
			Shape(
					(x()&&(r.xEnd>r.xBegin))?r.xEnd():int(orig[0u]),
					(y()&&(r.yEnd>r.yBegin))?r.yEnd():int(orig[1u]),
					(z()&&(r.zEnd>r.zBegin))?r.zEnd():int(orig[2u]),
					(t()&&(r.tEnd>r.tBegin))?r.tEnd():int(orig[3u]),
					(v()&&(r.vEnd>r.vBegin))?r.vEnd():int(orig[4u])
			)
	);
}

#endif /* _SUBARRAY_HXX_ */
