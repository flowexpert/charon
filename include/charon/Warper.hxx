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

/** @file Warper.hxx
 *  Implementation of class Warper.
 *  @author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *
 *  @date 09.01.2013
 */

#ifndef _WARPER_HXX_
#define _WARPER_HXX_

#include "Warper.h"
#include <charon-core/ParameteredObject.hxx>

#define ONLY_CHECK_BOUNDS -1

template <typename T>
Warper<T>::Warper(const std::string& name) :
	TemplatedParameteredObject<T>("Warper",name,
	"Warper<br><br>"
	"A Warper translates simple positions (e.g. out-of-bounds positions) "
	"into effective (within-bounds) positions, that is it performs "
	"boundary handling. "
	"The base class Warper performs radial interpolation."
	)
{
	this->_addInputSlot(motion,"motion","motion used to warp", "CImgList<T>");
	this->_addOutputSlot(out,"this","Pointer to itself", "Warper<T>*");

	this->_addParameter(mirror, "mirror", "mirroring boundary behavior, if set", false);
}

template <typename T>
void Warper<T>::execute()
{
	out() = this;

	_motion = motion();

	_dimX = _motion[0].width();
	_dimY = _motion[0].height();
}

template <typename T>
T Warper<T>::getX( T c, T x, T, T xMotion, T yMotion )
{
	// warp x position with horizontal motion
	// obtained at xMotion,yMotion
	// (which always should be the center of mask
	//  and thus in bounds within the calling function)
	//
	// do not warp the first image (c==0)
	if (c) {
		x += _motion[0].atXYZC(xMotion,yMotion,0,0);
	}

	// asymmetric behavior needed to emulate DS_s implementation  // (!!)
	if (c == ONLY_CHECK_BOUNDS) {                                 // (!!)
		if (x >= _dimX-1) return 0;                           // (!!)
	}                                                             // (!!)

	// mirroring at image boundaries, if new x is out of bounds
	if (mirror())
	{
		if (x < 0) {
			if (c == ONLY_CHECK_BOUNDS) return 0;
			return -x -1 ;
		} else if (x >= _dimX) {
			if (c == ONLY_CHECK_BOUNDS) return 0;
			return 2*_dimX -x -1 ;
		}
		if (c == ONLY_CHECK_BOUNDS) return 1;
		return x;
	}

	// radial extrapolation, if new x is out of bounds
	if (x < 0) {
		if (c == ONLY_CHECK_BOUNDS) return 0;
		return 0;
	} else if (x >= _dimX) {
		if (c == ONLY_CHECK_BOUNDS) return 0;
		return _dimX-1;
	}
	if (c == ONLY_CHECK_BOUNDS) return 1;
	return x;
}

template <typename T>
T Warper<T>::getY( T c, T, T y, T xMotion, T yMotion )
{
	// add vertical motion
	if (c) {
		y += _motion[1].atXYZC(xMotion,yMotion,0,0);
	}

	// asymmetric behavior needed to emulate DS_s implementation  // (!!)
	if (c == ONLY_CHECK_BOUNDS) {                                 // (!!)
		if (y >= _dimY-1) return 0;                           // (!!)
	}                                                             // (!!)

	// boundary handling (mirror)
	if (mirror())
	{
		if (y < 0) {
			if (c == ONLY_CHECK_BOUNDS) return 0;
			return -y -1 ;
		} else if (y >= _dimY) {
			if (c == ONLY_CHECK_BOUNDS) return 0;
			return 2*_dimY -y -1 ;
		}
		if (c == ONLY_CHECK_BOUNDS) return 1;
		return y;
	}

	// boundary handling (radial)
	if (y < 0) {
		if (c == ONLY_CHECK_BOUNDS) return 0;
		return 0;
	} else if (y >= _dimY) {
		if (c == ONLY_CHECK_BOUNDS) return 0;
		return _dimY-1;
	}
	if (c == ONLY_CHECK_BOUNDS) return 1;
	return y;
}

#endif

