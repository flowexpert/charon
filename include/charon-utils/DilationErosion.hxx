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
/** \file DilationErosion.hxx
 *  Implementation of the parameter class DilationErosion.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 25.01.2011
 */

#ifndef _DILATION_EROSION_HXX_
#define _DILATION_EROSION_HXX_

#include "DilationErosion.h"
#include <vigra/multi_morphology.hxx>

template <typename T>
DilationErosion<T>::DilationErosion(const std::string& name) :
	TemplatedParameteredObject<T>("dilationerosion", name,
		"morphology operation to dilate or erode")
{
	ParameteredObject::_addParameter(
			radius, "radius",
			"dilation radius (pos: dilate, neg: erode)", 1);
	ParameteredObject::_addInputSlot (
			in, "in", "data input", "vigraArray5<T>");
	ParameteredObject::_addOutputSlot(
			out, "out", "data output", "vigraArray5<T>");
}

template <typename T>
void DilationErosion<T>::execute()
{
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	out().reshape(in().shape());

	int r = radius();
	vigra_precondition(r != 0, "radius has not to be zero");

	if ( r > 0 )
		vigra::multiBinaryDilation(
				vigra::srcMultiArrayRange(in()),
				vigra::destMultiArray(out()),
				r);
	else
		vigra::multiBinaryErosion(
				vigra::srcMultiArrayRange(in()),
				vigra::destMultiArray(out()),
				-r);
}

#endif /* _DILATION_EROSION_HXX_ */
