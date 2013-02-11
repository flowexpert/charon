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
/// \file ImageRotation.hxx
/// Implementation of the parameter class ImageRotation.
/// \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
///   Michael Baron</a>
/// \date 11.02.2013

#ifndef IMAGEROTATION_HXX_
#define IMAGEROTATION_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "ImageRotation.h"

template<typename T>
ImageRotation<T>::ImageRotation(const std::string& name) :
		TemplatedParameteredObject<T>("ImageRotation", name,
				"Image rotation and GT generation")
{
	ParameteredObject::_setTags("charon;CImg") ;

	ParameteredObject::_addInputSlot(
			in, "in", "image input", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			out, "out", "image output", "CImgList<T>");

	ParameteredObject::_addParameter< T >(
			angle, "angle", "rotation angle", 0.0 );
	ParameteredObject::_addParameter< unsigned int >(
			boundaryCondition, "boundaryCondition", "(0,1,2) == (dirichlet, neumann, cyclic)", 1 );
	ParameteredObject::_addParameter< unsigned int >(
			interpolationType, "interpolationType", "(0,1,2) == (nearest, linear, cubic)", 1 );
}

template<typename T>
void ImageRotation<T>::execute()
{
	const cimg_library::CImgList<T> &_in = in();
	cimg_library::CImgList<T> &_out = out();

	_out = _in;

	int _width = _in[0].width();
	int _height = _in[0].height();

	cimglist_for( _out, kk )
	{
		_out[kk].rotate( angle(), _width/2, _height/2, 1.0, boundaryCondition(), interpolationType() );
	}
}

#endif // IMAGEROTATION_HXX_

