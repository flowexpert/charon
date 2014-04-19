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
/** \file SimpleWarp.hxx
 *  Implementation of the parameter class SimpleWarp.
 *  \author Michael Baron
 *  \date 14.02.2013
 */

#ifndef _SIMPLEWARP_HXX_
#define _SIMPLEWARP_HXX_

#include "SimpleWarp.h"

template<typename T>
SimpleWarp<T>::SimpleWarp(const std::string& name) :
	TemplatedParameteredObject<T> (
			"SimpleWarp", name,
			"This module performs simple warping of image sequences.<br>"
			"<h2>deprecated</h2>"
			) {
	ParameteredObject::_setTags("charon-utils;CImg") ;

	ParameteredObject::_addInputSlot(seqInput, "seqInput",
			"Image sequence", "CImgList<T>");
	ParameteredObject::_addInputSlot(flowInput, "flowInput",
			"Image flow", "CImgList<T>");
	ParameteredObject::_addInputSlot(interpolator, "interpolator",
			"Interpolator", "Interpolator<T>*");
	ParameteredObject::_addOutputSlot(out, "out",
			"Warped Images", "CImgList<T>");
	ParameteredObject::_addOutputSlot(warpWithinBounds, "warpWithinBounds",
			"mask indicating, if warp is within bounds", "CImgList<T>");
}

template<typename T>
void SimpleWarp<T>::execute()
{
	// check sizes
	const cimg_library::CImgList<T>& seq = seqInput();
	const cimg_library::CImgList<T>& flow = flowInput();
	cimg_library::CImgList<T>& warped = out();
	cimg_library::CImgList<T>& withinBounds = warpWithinBounds();

	Interpolator<T>& interp = *interpolator();

	unsigned int dn = seq.size();
	unsigned int dx = seq[0].width();
	unsigned int dy = seq[0].height();
	unsigned int dz = seq[0].depth();
	unsigned int dc = seq[0].spectrum();

	warped = cimg_library::CImgList<T>( dn, dx, dy, dz, dc );
	withinBounds = cimg_library::CImgList<T>( dn, dx, dy, dz, dc );

	// continuous coordinates for interpolation (interpolator uses float)
	float xn, yn, zn ;
	T res ;
	cimglist_for(warped, n)   // for each color channel
	cimg_forC(warped[n], c) { // for each image in sequence
		cimg_forXYZ(warped[n],x,y,z) { // for each position within volume
			if(c == 0) {
				xn = (float)x;
				yn = (float)y;
				zn = (float)z;
			} else {
				xn = (float)x+flow[0].atXYZC(x,y,0,0);
				yn = (float)y+flow[1].atXYZC(x,y,0,0);
				zn = (float)z;
			}
			res = interp.interpolate(seq[n], xn, yn, zn, c);
			warped.atNXYZC(n,x,y,z,c) = res;

			if ((xn>=0) && (xn<=dx-1) && (yn>=0) && (yn<=dy-1)) {
				withinBounds.atNXYZC(n,x,y,z,c) = 255;
			} else {
				withinBounds.atNXYZC(n,x,y,z,c) = 0;
			}
		}
	}
}

#endif /* _SIMPLEWARP_HXX_ */

