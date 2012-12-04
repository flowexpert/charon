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
/** \file Demux2.hxx
 *  Implementation of the parameter class Demux2.
 *  \author Michael Baron
 *  \date 24.11.2011
 */

#ifndef _DEMUX2_HXX_
#define _DEMUX2_HXX_

#include "Demux2.h"

template<typename T>
Demux2<T>::Demux2(const std::string& name) :
	TemplatedParameteredObject<T> ("Demux2", name,
			"Generates an image sequence out of multiple images")
{
	ParameteredObject::_setTags("charon-utils;CImg") ;

	ParameteredObject::_addInputSlot(in, "in",
			"a CImgList<T>", "CImgList<T>");

	ParameteredObject::_addOutputSlot(out1, "out1",
			"multiplexed CImgList<T>", "CImgList<T>");

	ParameteredObject::_addOutputSlot(out2, "out2",
			"multiplexed CImgList<T>", "CImgList<T>");
	ParameteredObject::_setTags("charon-utils");
}

template<typename T>
void Demux2<T>::execute() {
	const cimg_library::CImgList<T> &_in = in();

	// Dimensions of the input image lists
	int size = _in.size();
	int width = _in[0].width();
	int height = _in[0].height();
	int depth = _in[0].depth();
	int spectrum = _in[0].spectrum();

	// Dim of output image list
	int halfSize = size/2;

	cimg_library::CImgList<T> _out1(halfSize, width, height, depth, spectrum);
	cimg_library::CImgList<T> _out2(halfSize, width, height, depth, spectrum);

	int n, x, y, z, c;
	for (n=0; n<halfSize; ++n)
	for (x=0; x<width; ++x)
	for (y=0; y<height; ++y)
	for (z=0; z<depth; ++z)
	for (c=0; c<spectrum; ++c)
	{
		_out1(n,x,y,z,c) = _in.atNXYZC(n,x,y,z,c);
	}

        for (n=0; n<halfSize; ++n)
        for (x=0; x<width; ++x)
        for (y=0; y<height; ++y)
        for (z=0; z<depth; ++z)
        for (c=0; c<spectrum; ++c)
        {
                _out2(n,x,y,z,c) = _in.atNXYZC(n+halfSize,x,y,z,c);
        }

	out1() = _out1;
	out2() = _out2;
}

#endif /* _DEMUX2_HXX_ */

