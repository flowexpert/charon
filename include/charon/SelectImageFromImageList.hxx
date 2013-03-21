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
/** \file SelectImageFromImageList.hxx
 *  Implementation of the parameter class SelectImageFromImageList.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 21.03.2013
 */

#ifndef _SELECTIMAGEFROMIMAGELIST_HXX_
#define _SELECTIMAGEFROMIMAGELIST_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "SelectImageFromImageList.h"

template <typename T>
SelectImageFromImageList<T>::SelectImageFromImageList(const std::string& name) :
		TemplatedParameteredObject<T>(
			"SelectImageFromImageList", name,
			"<h2>Select an image from an image list</h2><br>"
			"Select an image from an image list."
		)
{
	ParameteredObject::_addInputSlot(
		in, "in",
		"image list input",
		"CImgList<T>");

	ParameteredObject::_addOutputSlot(
		out, "out",
		"image output",
		"CImgList<T>");

	ParameteredObject::_addParameter< unsigned int >(
		frameNumber, "frameNumber",
		"frame number",
		0, "uint");

}

template <typename T>
void SelectImageFromImageList<T>::execute()
{
	const cimg_library::CImgList<T> &_in = in();
	unsigned int width = _in[0].width();
	unsigned int height = _in[0].height();
	unsigned int depth = _in[0].depth();
	unsigned int spectrum = _in[0].spectrum();

	unsigned int fn = frameNumber();

	cimg_library::CImgList<T> &_out = out();
	_out = cimg_library::CImgList<T>( 1, width, height, depth, spectrum );

	cimg_forXYZC( _out[0], x, y, z, c )
	{
		_out.atNXYZC( 0, x, y, z, c ) = _in.atNXYZC( fn, x, y, z, c );
	}
}

#endif /* _SELECTIMAGEFROMIMAGELIST_HXX_ */

