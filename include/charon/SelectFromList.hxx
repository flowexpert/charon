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
			"<h2>Select an image from an image list</h2><br>"
			"Select an image from an image list."
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
		0, "uint");

}

template <typename T>
void SelectFromList<T>::execute()
{
	const cimg_library::CImgList<T> &_in = list();
	int _size = _in.size();
	int _width = _in[0].width();
	int _height = _in[0].height();
	int _depth = _in[0].depth();

	int _first = first();
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

