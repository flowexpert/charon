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
/** \file WriteIntoList.hxx
 *  Implementation of the parameter class WriteIntoList.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 18.06.2013
 */

#ifndef _WRITEINTOLIST_HXX_
#define _WRITEINTOLIST_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "WriteIntoList.h"

template <typename T>
WriteIntoList<T>::WriteIntoList(const std::string& name) :
		TemplatedParameteredObject<T>(
			"WriteIntoList", name,
			"Module for (over-)writing (sub-)sequences of existing CImgList<T>s<br>"
			"Takes an input list of C dimension c and writes it either whole, or "
			"partially into the target list of C dimension c1, offset by the "
			"value of first. "
			"If first+c exceeds c1, the resulting list will be cut at c1-1, and "
			"the last element of out will be the last element of input. "
			"This can be usful to perform optical flow estimations successively, "
			"and write their results into a single list, which has to be allocated "
			"a-priori, by using the emptycimg module. "
			"Another application lies in estimation algorithms, which make use of "
			"the temporal consitency of the flow. "
			"Attention: The (temporal) index dimension is C or T, and NOT N, which "
			"is here reserved for the vector field dimension, e.g. in order to write "
			"multidimensional data, like resulting flow fields, into a result list."
		)
{
	ParameteredObject::_addInputSlot(
		list, "list",
		"image list input, field dimension is n, index dimension is c, so use ChannelConverter",
		"CImgList<T>");

	ParameteredObject::_addInputSlot(
		input, "input",
		"input slot for data that is written into list",
		"CImgList<T>");

	ParameteredObject::_addInputSlot(
		first, "first",
		"index of first frame to overwrite in list",
		"uint");

	ParameteredObject::_addOutputSlot(
		out, "out",
		"image output",
		"CImgList<T>");

}

template <typename T>
void WriteIntoList<T>::execute()
{
	const cimg_library::CImgList<T> &_list = list();
	int _size = _list.size();
	int _width = _list[0].width();
	int _height = _list[0].height();
	int _depth = _list[0].depth();

	const cimg_library::CImgList<T> &_input = input();
	int _count = _input[0].spectrum();

	int _first = first();

	cimg_library::CImgList<T> &_out = out();
	_out = _list;

	for (int nn=0; nn<_size; nn++)
	for (int xx=0; xx<_width; xx++)
	for (int yy=0; yy<_height; yy++)
	for (int zz=0; zz<_depth; zz++)
	for (int cc=0; cc<_count; cc++)
	{
		_out.atNXYZC( nn, xx, yy, zz, cc+_first ) = _input.atNXYZC( nn, xx, yy, zz, cc );
	}
}

#endif /* _WRITEINTOLIST_HXX_ */

