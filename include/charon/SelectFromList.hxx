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
		"image list input",
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
	unsigned int width = _in[0].width();
	unsigned int height = _in[0].height();
	unsigned int depth = _in[0].depth();
	unsigned int spectrum = _in[0].spectrum();

	int _first = first();
	int _count = count();
	int _last = _first + _count;

	cimg_library::CImgList<T> &_out = out();
	_out = cimg_library::CImgList<T>( _count, width, height, depth, spectrum );

	for ( int pos=_first, retpos=0; pos<_last; pos++, retpos++ )
	cimg_forXYZC( _out[0], x, y, z, c )
	{
		_out.atNXYZC( retpos, x, y, z, c ) = _in.atNXYZC( pos, x, y, z, c );
	}
}

#endif /* _SELECTFROMLIST_HXX_ */

