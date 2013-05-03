/*  Copyright (C) 2013
                  University of Heidelberg (IWR/HCI)

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
/** \file WarpForward.hxx
 *  Implementation of the parameter class WarpForward.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 10.04.2013
 */

#ifndef _WARPFORWARD_HXX_
#define _WARPFORWARD_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "WarpForward.h"

template <typename T>
WarpForward<T>::WarpForward(const std::string& name) :
		TemplatedParameteredObject<T>(
			"WarpForward", name,
			"<h2>Module for forward warping</h2><br>"
			"Module for forward warping."
		)
{
	ParameteredObject::_addInputSlot(
		data, "data",
		"data input",
		"CImgList<T>");
	ParameteredObject::_addInputSlot(
		motion, "motion",
		"motion input",
		"CImgList<T>");

	ParameteredObject::_addOutputSlot(
		out, "out",
		"output",
		"CImgList<T>");

}

template <typename T>
void WarpForward<T>::execute()
{
	const cimg_library::CImgList<T> &_data = data();
	const cimg_library::CImgList<T> &_motion = motion();
	cimg_library::CImgList<T> &_out = out();

	int _size = _data.size();
	int _width = _data[0].width();
	int _height = _data[0].height();
	int _depth = _data[0].depth();
	int _spectrum = _data[0].spectrum();

	_out = cimg_library::CImgList<T>( _size, _width, _height, _depth, _spectrum, T(0) );

	T u, v;
	T dxx, dyy;
	cimg_forXYZC( _data[0], xx, yy, zz, cc )
	{
		u = _motion[0].atXYZC( xx, yy, zz, cc );
		v = _motion[1].atXYZC( xx, yy, zz, cc );
		dxx = xx+u;
		dyy = yy+v;
		if ((0 <= dxx) && (dxx <= _width-1) && (0 <= dyy) && (dyy <= _height-1) && (dxx == dxx) && (dyy == dyy))
		{
			cimglist_for( _data, nn )
			{
				_out.atNXYZC( nn, dxx, dyy, zz, cc ) = _data.atNXYZC( nn, xx, yy, zz, cc );
			}
		}
	}
}

#endif /* _WARPFORWARD_HXX_ */

