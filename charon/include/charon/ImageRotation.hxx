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
/// \file ImageRotation.hxx
/// Implementation of the parameter class ImageRotation.
/// \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
///   Michael Baron</a>
/// \date 11.02.2013

#ifndef IMAGEROTATION_HXX_
#define IMAGEROTATION_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "ImageRotation.h"

#include <math.h>  //  for sin and cos

template<typename T>
ImageRotation<T>::ImageRotation(const std::string& name) :
		TemplatedParameteredObject<T>("ImageRotation", name,
			"This module rotates the input image list within the X-Y plane "
			"and yields the corresponding two-dimensional ground-truth "
			"motion field. "
			"Here, the rotation's angle can be specified by the angle parameter, "
			"whereas boundaryCondition denotes the extrapolation behavior "
			"within the undefined regions and interpolationType the "
			"corresponding interpolation type. "
			)
{
	ParameteredObject::_setTags("charon;CImg") ;

	ParameteredObject::_addInputSlot(
			in, "in", "image input", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			out, "out", "image output", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			groundTruth, "groundTruth", "ground truth", "CImgList<T>");

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
	cimg_library::CImgList<T> &_groundTruth = groundTruth();

	_out = _in;

	int _width = _in[0].width();
	int _height = _in[0].height();

	double _rotationCenterX = (_width-1)/2;
	double _rotationCenterY = (_height-1)/2;

	_groundTruth = cimg_library::CImgList<T>( 2, _width, _height, 1, 1 );

	cimglist_for( _out, kk )
	{
		_out[kk].rotate( angle(), _rotationCenterX, _rotationCenterY, 1.0, boundaryCondition(), interpolationType() );
	}

	double _angleRad = (T(angle()) / 180.0) * M_PI;
	cimg_forXY( _groundTruth[0], x, y )
	{
		_groundTruth[0].atXY( x, y ) = T((cos(_angleRad)-1.0) * (x - _rotationCenterX)
		                             -  sin(_angleRad)      * (y - _rotationCenterY));
		_groundTruth[1].atXY( x, y ) =  T(sin(_angleRad)      * (x - _rotationCenterX)
		                             + (cos(_angleRad)-1.0) * (y - _rotationCenterY));
	}
}

#endif // IMAGEROTATION_HXX_

