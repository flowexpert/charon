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

template <typename T>
PyramidRescaleMask<T>::PyramidRescaleMask(const std::string& name) :
		TemplatedParameteredObject<T>("PyramidRescaleMask", name,
			"Pyramid Rescaling for binary masks.")
{
	ParameteredObject::_addInputSlot(
			maskIn, "maskIn", "mask input", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			level, "level", "level select (from small to larger scales)");
	ParameteredObject::_addOutputSlot(
			maskOut, "maskOut", "mask output", "CImgList<T>");
	ParameteredObject::_addParameter (
			scaleFactor, "scaleFactor", "scale factor", 0.5);
	ParameteredObject::_addParameter (
			levels, "levels", "scale levels", 5u);

	ParameteredObject::_setTags("charon-flow;MultiscaleApproaches;CImg");
}

template <typename T>
void PyramidRescaleMask<T>::execute() {
	const cimg_library::CImgList<T>& si = maskIn();
	cimg_library::CImgList<T>& so = maskOut();

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
	const double _shrinkInv = 1.0 / _shrink;

	int lowerX, lowerY, higherX, higherY;
	double value;

	// rescale sequence
	const int _width = si[0].width();
	const int _height = si[0].height();
	so = cimg_library::CImgList<T>( si.size(),
	                                _width * _shrink, _height * _shrink,
	                                si[0].depth(), si[0].spectrum() );

	cimglist_for( so, kk )
	cimg_forXYZC( so[kk], x, y, z, c )
	{
		lowerX =  int( double(x) * _shrinkInv );
		lowerY =  int( double(y) * _shrinkInv );
		higherX = int( double(x) * _shrinkInv ) + 1;
		higherY = int( double(y) * _shrinkInv ) + 1;

		value = T(1.0);
		for (int j=lowerX; j<higherX; j++)
		for (int i=lowerY; i<higherY; i++)
		{
			value *= si[kk].atXYZC( j, i, z, c );
		}

		so[kk].atXYZC( x, y, z, c ) = value;
	}
}

#endif /* _PYRAMID_RESCALE_MASK_HXX_ */

