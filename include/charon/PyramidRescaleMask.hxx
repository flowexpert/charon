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
/** \file PyramidRescaleMask.hxx
 *  Implementation of the parameter class PyramidRescaleMask.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 21.01.2013
 */

#ifndef _PYRAMID_RESCALE_MASK_HXX_
#define _PYRAMID_RESCALE_MASK_HXX_

#include "PyramidRescaleMask.h"

#include <math.h>

template <typename T>
PyramidRescaleMask<T>::PyramidRescaleMask(const std::string& name) :
		TemplatedParameteredObject<T>("PyramidRescaleMask", name,
			"Pyramid Rescaling for binary masks.<br>"
			"<h2>deprecated</h2>"
		)
{
	ParameteredObject::_addInputSlot(
			mask, "mask", "input mask", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			level, "level", "level select (from small to larger scales)");
	ParameteredObject::_addOutputSlot(
			currentMask, "currentMask", "output current mask", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			previousMask, "previousMask", "output previous mask", "CImgList<T>");
	ParameteredObject::_addParameter (
			scaleFactor, "scaleFactor", "scale factor", 0.5);
	ParameteredObject::_addParameter (
			levels, "levels", "scale levels", 5u);

	ParameteredObject::_setTags("charon-flow;MultiscaleApproaches;CImg");
}

template <typename T>
void PyramidRescaleMask<T>::execute() {
	const cimg_library::CImgList<T>& _mask = mask();
	cimg_library::CImgList<T>& _currentMask = currentMask();
	cimg_library::CImgList<T>& _previousMask = previousMask();

	const unsigned int curL = level();
	const unsigned int endL = levels();
	const unsigned int maxL = endL-1;
	const unsigned int stepsDown = maxL-curL;

	if(curL > maxL) {
		std::ostringstream msg;
		msg << "current level too large: cur=" << curL << "; max=" << maxL;
		ParameteredObject::raise(msg.str());
	}

	// target sizes
	const double _scaleFactor = scaleFactor();
	const double _shrink = std::pow(_scaleFactor,(double)stepsDown);
	const double _shrinkOnceMore = std::pow(_scaleFactor,(double)(stepsDown+1));
	const double _shrinkInv = 1.0 / _shrink;
	const double _shrinkOnceMoreInv = 1.0 / _shrinkOnceMore;

	int lowerX, lowerY, higherX, higherY;
	double value;

	// rescale mask (this yields the current mask)
	const int _size = _mask.size();
	const int _width = _mask[0].width();
	const int _height = _mask[0].height();
	const int _depth = _mask[0].depth();
	const int _spectrum = _mask[0].spectrum();
	_currentMask = cimg_library::CImgList<T>( _size,
	                                          _width * _shrink, _height * _shrink,
	                                          _depth, _spectrum );
	_previousMask = cimg_library::CImgList<T>( _size,
	                                           _width * _shrink, _height * _shrink,
	                                           _depth, _spectrum );

	cimglist_for( _currentMask, kk )
	cimg_forXYZC( _currentMask[kk], x, y, z, c )
	{
		lowerX =  floor( double(x) * _shrinkInv );
		lowerY =  floor( double(y) * _shrinkInv );
		higherX = ceil( double(x+1) * _shrinkInv );
		higherY = ceil( double(y+1) * _shrinkInv );

		lowerX = (lowerX < 0) ? 0 : lowerX;
		lowerY = (lowerY < 0) ? 0 : lowerY;
		higherX = (higherX > _width) ? _width : higherX;
		higherY = (higherY > _height) ? _height : higherY;

		value = T(1.0);
		for (int j=lowerX; j<higherX; j++)
		for (int i=lowerY; i<higherY; i++)
		{
			value *= _mask[kk].atXYZC( j, i, z, c );
		}

		_currentMask[kk].atXYZC( x, y, z, c ) = value ? 255 : 0;
	}

	// rescale mask once more (this yields a temporary mask)
	cimg_library::CImgList<T> tmpMask;
	tmpMask = cimg_library::CImgList<T>( _size,
	                                     _width * _shrinkOnceMore, _height * _shrinkOnceMore,
	                                     _depth, _spectrum );

	cimglist_for( tmpMask, kk )
	cimg_forXYZC( tmpMask[kk], x, y, z, c )
	{
		lowerX =  floor( double(x) * _shrinkOnceMoreInv );
		lowerY =  floor( double(y) * _shrinkOnceMoreInv );
		higherX = ceil( double(x+1) * _shrinkOnceMoreInv );
		higherY = ceil( double(y+1) * _shrinkOnceMoreInv );

		lowerX = (lowerX < 0) ? 0 : lowerX;
		lowerY = (lowerY < 0) ? 0 : lowerY;
		higherX = (higherX > _width) ? _width : higherX;
		higherY = (higherY > _height) ? _height : higherY;

		value = T(1.0);
		for (int j=lowerX; j<higherX; j++)
		for (int i=lowerY; i<higherY; i++)
		{
			value *= _mask[kk].atXYZC( j, i, z, c );
		}

		tmpMask[kk].atXYZC( x, y, z, c ) = value ? 255 : 0;
	}

	// upscale temporary mask to previous mask
	int newX, newY;
	cimglist_for( _previousMask, kk )
	cimg_forXYZC( _previousMask[kk], x, y, z, c )
	{
		newX = int( double(x) * _scaleFactor );
		newY = int( double(y) * _scaleFactor );

		value = tmpMask[kk].atXYZC( newX, newY, z, c );

		_previousMask[kk].atXYZC( x, y, z, c ) = value ? 255 : 0;
	}
}

#endif /* _PYRAMID_RESCALE_MASK_HXX_ */

