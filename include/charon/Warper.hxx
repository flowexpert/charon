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
#include <ParameteredObject.hxx>

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
}

template <typename T>
void Warper<T>::execute()
{
        out = this;

	_motion = motion();

	_dimN = _motion.size();
	_dimX = _motion[0].width();
	_dimY = _motion[0].height();
	_dimZ = _motion[0].depth();
	_dimC = _motion[0].spectrum();
}

template <typename T>
unsigned int Warper<T>::getN( int N )
{
	if (N < 0) {
		return 0;
	} else if (N >= _dimN) {
		return _dimN-1;
	}
	return N;
}

template <typename T>
unsigned int Warper<T>::getX( int X )
{
	if (X < 0) {
		return 0;
	} else if (X >= _dimX) {
		return _dimX-1;
	}
	return X;
}

template <typename T>
unsigned int Warper<T>::getY( int Y )
{
	if (Y < 0) {
		return 0;
	} else if (Y >= _dimY) {
		return _dimY-1;
	}
	return Y;
}

template <typename T>
unsigned int Warper<T>::getZ( int Z )
{
	if (Z < 0) {
		return 0;
	} else if (Z >= _dimZ) {
		return _dimZ-1;
	}
	return Z;
}

template <typename T>
unsigned int Warper<T>::getC( int C )
{
	if (C < 0) {
		return 0;
	} else if (C >= _dimC) {
		return _dimC-1;
	}
	return C;
}

#endif

