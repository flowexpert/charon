/*  Copyright (C) 2010 Stephan Meister

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
/** \file Cut.hxx
 *  Implementation of the parameter class Cut.
 *  \author Stephan Meister
 *  \date 30.07.2010
 */

#ifndef _CUT_HXX_
#define _CUT_HXX_

#include "Cut.h"

template <typename T>
Cut<T>::Cut(const std::string& name) :
	TemplatedParameteredObject<T>(
		"Cut", name, "cutoff values<br>"
		"pixel values higher than upperCut will be set to upperCut, "
		"and vice versa for lowerCut<br>"
		"image dimensions remain unchanged")
{
	ParameteredObject::_setTags("charon-utils;CImg") ;

	ParameteredObject::_addParameter (
				_upperCut, "upperCut", "upper cutoff value", 1.0) ;
	ParameteredObject::_addParameter (
				_lowerCut, "lowerCut", "lower cutoff value", 0.0) ;

	ParameteredObject::_addParameter (
				_normalize, "normalize",
				"normalize, if set (overwrites first two values with cutoff values)", false) ;

	ParameteredObject::_addInputSlot(
				_in, "in", "input image", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
				_out, "out", "output image", "CImgList<T>");
}

template <typename T>
void Cut<T>::execute() {
	const cil::CImgList<T>& in = _in() ;
	_out().assign(in) ;
	cil::CImgList<T>& out = _out() ;
	cimglist_for(in, l)
	{
		out(l) = in(l).get_cut(T(_lowerCut()),T(_upperCut())) ;
		if (_normalize())
		{
			out(l).atXYZC(0,0,0,0) = _upperCut();
			out(l).atXYZC(1,0,0,0) = _lowerCut();
		}
	}
}

#endif /* _DIST2DEPTH_HXX_ */

