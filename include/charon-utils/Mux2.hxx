/*

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
/** \file Mux2.hxx
 *  Implementation of the parameter class Mux2.
 *  \author Michael Baron
 *  \date 24.11.2011
 */

#ifndef _MUX2_HXX_
#define _MUX2_HXX_

#include "Mux2.h"
#include <algorithm>

template<typename T>
Mux2<T>::Mux2(const std::string& name) :
	TemplatedParameteredObject<T> ("Mux2", name,
			"Generates an image sequence out of multiple images")
{
	ParameteredObject::_setTags("charon-utils;CImg") ;

	ParameteredObject::_addInputSlot(list1, "list1",
			"first CImgList<T>", "CImgList<T>");
	ParameteredObject::_addInputSlot(list2, "list2",
			"second CImgList<T>", "CImgList<T>");

	ParameteredObject::_addOutputSlot(out, "out",
			"multiplexed CImgList<T>", "CImgList<T>");

	ParameteredObject::_setTags("charon-utils");
}

template<typename T>
void Mux2<T>::execute() {
	const cimg_library::CImgList<T> &_list1 = list1();
	const cimg_library::CImgList<T> &_list2 = list2();

	// Dimensions of the input image lists
	int size1 = _list1.size();
	int size2 = _list2.size();
	int width = _list1[0].width();
	int height = _list1[0].height();
	int depth = _list1[0].depth();
	int spectrum = _list1[0].spectrum();

	// Dim of output image list
	int size = size1 + size2;

	cimg_library::CImgList<T> _out(size, width, height, depth, spectrum);

	int n, x, y, z, c;
	for (n=0; n<size1; ++n)
	for (x=0; x<width; ++x)
	for (y=0; y<height; ++y)
	for (z=0; z<depth; ++z)
	for (c=0; c<spectrum; ++c)
	{
		_out(n,x,y,z,c) = _list1.atNXYZC(n,x,y,z,c);
	}

        for (n=0; n<size2; ++n)
        for (x=0; x<width; ++x)
        for (y=0; y<height; ++y)
        for (z=0; z<depth; ++z)
        for (c=0; c<spectrum; ++c)
        {
                _out(n+size1,x,y,z,c) = _list2.atNXYZC(n,x,y,z,c);
        }

	out() = _out;
}

#endif /* _MUX2_HXX_ */

