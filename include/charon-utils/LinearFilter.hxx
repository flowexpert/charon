/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/// \file LinearFilter.hxx
/// Implementation of the parameter class LinFilter
/// \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// \date 02.10.2009

#ifndef LINEAR_FILTER_HXX_
#define LINEAR_FILTER_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "LinearFilter.h"
#include "Convolution.hxx"
#include <cassert>

template <typename T>
LinearFilter<T>::LinearFilter(const std::string& name) :
		TemplatedParameteredObject<T>(
				"LinearFilter", name,
				"Linear filtering by convolution"),
		masks(false, true)
{
	ParameteredObject::_addInputSlot(
			masks, "masks", "filter mask input", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			in, "in", "image input", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			out, "out", "result output", "CImgList<T>");
}

template <typename T>
void LinearFilter<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();
	assert(masks[0].size() == 1);
	assert(in().size() >= 1);
	out().assign(
			in().size(),
			in()[0].width(),
			in()[0].height(),
			in()[0].width(),
			in()[0].spectrum(),
			T(0));
	assert(out().size() == in().size());
	cimglist_for(in(), ii)
		Convolution::convolve(masks[0][0], in()[ii], out()[ii]);

	// continue convolutions if more than one filter mask given
	std::set<Slot*>::size_type tsize = masks.getTargets().size();
	for(unsigned int ii=1; ii < tsize; ii++) {
		assert(masks[ii].size() == 1);
		cimglist_for(out(), jj)
			Convolution::convolve(masks[ii][0], out()[jj], out()[jj]);
	}
}

#endif // LINEAR_FILTER_HXX_

