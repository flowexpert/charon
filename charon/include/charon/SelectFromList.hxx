/*  Copyright (C) 2013
                  University of Heidelberg (IWR/HCI)

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
/** \file SelectFromList.hxx
 *  Implementation of the parameter class SelectFromList.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 21.03.2013
 */

#ifndef _SELECTFROMLIST_HXX_
#define _SELECTFROMLIST_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "SelectFromList.h"

template <typename T>
SelectFromList<T>::SelectFromList(const std::string& name) :
		TemplatedParameteredObject<T>(
			"SelectFromList", name,
			"Selects data from a list</h2><br>"
			"The input slot first can be connected to a Value module or a Count output slot "
			"of an IteratorHelper, such that first+offset denotes the first position in list. "
			"The parameter count defaults to 1 and denotes the count of frames. "
			"Attention: All list indexes are w.r.t. the C or T dimension of that list, "
			"and NOT the N dimension, which is here reserved for the vector field components! "
		)
{
	ParameteredObject::_addInputSlot(
		list, "list",
		"image list input, field dimension is n, index dimension is c, so use ChannelConverter",
		"CImgList<T>");

	ParameteredObject::_addInputSlot(
		first, "first",
		"index of first frame to return",
		"uint");

	ParameteredObject::_addOutputSlot(
		out, "out",
		"image output",
		"CImgList<T>");

	ParameteredObject::_addParameter< unsigned int >(
		count, "count",
		"count of frames to return",
		1, "uint");

	ParameteredObject::_addParameter< int >(
		offset, "offset",
		"offset",
		0, "int");
}

template <typename T>
void SelectFromList<T>::execute()
{
	const cimg_library::CImgList<T> &_in = list();
	int _size = _in.size();
	int _width = _in[0].width();
	int _height = _in[0].height();
	int _depth = _in[0].depth();

	int _first = first() + offset();
	int _count = count();
	int _last = _first + _count;

	cimg_library::CImgList<T> &_out = out();
	_out = cimg_library::CImgList<T>( _size, _width, _height, _depth, _count );

	for (int nn=0; nn<_size; nn++)
	for (int xx=0; xx<_width; xx++)
	for (int yy=0; yy<_height; yy++)
	for (int zz=0; zz<_depth; zz++)
	for (int pos=_first, retpos=0; pos<_last; pos++, retpos++)
	{
		_out.atNXYZC( nn, xx, yy, zz, retpos ) = _in.atNXYZC( nn, xx, yy, zz, pos );
	}
}

#endif /* _SELECTFROMLIST_HXX_ */

