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
/** \file ReSample.hxx
 *  Implementation of the parameter class ReSample.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 02.09.2013
 */

#ifndef _RESAMPLE_HXX_
#define _RESAMPLE_HXX_

#include "ReSample.h"
#include <charon-utils/Interpolator.hxx>
#include <charon-utils/Roi.hxx>

template <typename T>
ReSample<T>::ReSample(const std::string& name) :
		TemplatedParameteredObject<T>("ReSample", name,
			"Module for resampling of data.<br>"
			"Input data in is being resampled by pow(scaleFactor(), level()) "
			"utilizing the connected interpolator module. "
			"If scaleData == true, the data is being rescaled "
			"accordingly (which is desirable for flow fields). "
			"Additionally, a levelOffset can be specified, which "
			"can be usful within multiscale approaches. "
		)
{
	ParameteredObject::_addInputSlot(
			in, "in", "data input", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			level, "level", "level select (from smaller to larger scales)");
	ParameteredObject::_addInputSlot(
			interpolator, "interpolator", "interpolator", "Interpolator<T>*");
	ParameteredObject::_addOutputSlot(
			out, "out", "data output", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			size, "size", "current data size", "Roi<int>*");
	ParameteredObject::_addParameter (
			scaleFactor, "scaleFactor", "scale factor", 0.5);
	ParameteredObject::_addParameter (
			scaleData, "scaleData", "scale data values, if set", false);
	ParameteredObject::_addParameter (
			levels, "levels", "scale levels", 5u);
	ParameteredObject::_addParameter (
			levelOffset, "levelOffset", "level offset", 0);

	ParameteredObject::_setTags("charon-flow;MultiscaleApproaches;CImg");
}

template <typename T>
void ReSample<T>::execute()
{
	Interpolator<T> *_interpolator = interpolator();

	const unsigned int _level = level();
	const unsigned int _levels = levels();

	const cimg_library::CImgList<T>& _in = in();
	const unsigned int _sz = _in.size();
	const unsigned int _width = _in[0].width();
	const unsigned int _height = _in[0].height();
	const unsigned int _depth = _in[0].depth();
	const unsigned int _spectrum = _in[0].spectrum();
	cimg_library::CImgList<T>& _out = out();

	// compute target size (width, height)
	const unsigned int stepsDown = _levels-1-_level-levelOffset();
	const double shrink = std::pow(scaleFactor(),(double)stepsDown);
	unsigned int _twidth = _width * shrink;
	unsigned int _theight = _height * shrink;

	_out = cimg_library::CImgList<T>( _sz, _twidth, _theight, _depth, _spectrum, T(0) );
	_size.xEnd = _twidth;
	_size.yEnd = _theight;
	size() = &_size;

	const double _scaling = scaleData() ? shrink : 1.0;
	cimglist_for( _out, nn )
	cimg_forXYZC( _out[nn], xx, yy, zz, cc )
	{
		_out[nn].atXYZC(xx, yy, zz, cc) = _scaling * _interpolator->interpolate(_in[nn], xx/shrink, yy/shrink, zz, cc);
	}
	
}

#endif /* _PYRAMID_RESCALE_HXX_ */

