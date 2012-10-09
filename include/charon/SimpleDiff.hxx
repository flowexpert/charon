/*  Copyright (C) 2012 University of Heidelberg (IWR/HCI)

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
/** \file SimpleDiff.hxx
 *  Implementation of the parameter class SimpleDiff.
 *  \author Michael Baron
 *  \date 09.10.2012
 */

#ifndef _SIMPLEDIFF_HXX_
#define _SIMPLEDIFF_HXX_

#include "SimpleDiff.h"

template <typename T>
SimpleDiff<T>::SimpleDiff(const std::string& name) :
		TemplatedParameteredObject<T>("SimpleDiff", name,
			"Calculate 2D derivatives.<br><br>"
			"This plugin calculates two dimensional derivatives.")
{
	ParameteredObject::_addInputSlot(
			img, "img",
			"image input", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			dx, "dx", "derivative wrt x", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			dy, "dy", "derivative wrt y", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			dt, "dt", "derivative wrt t", "CImgList<T>");
}

template <typename T>
void SimpleDiff<T>::execute() {
	sout << "\tcalculating derivatives:" << std::endl;

	CImg_3x3( I, T );
	if(dx.connected()) {
		sout << "\t\twrt. dx" << std::endl;
		dx().assign(img());
		cimglist_for( img(),kk )
		cimg_for3x3( img()[kk], x, y, 0, 0, I, T )
		{
			dx().atNXYZC(kk,x,y,0,0) = (Inc - Ipc)/2.0;
		}
	}
	if(dy.connected()) {
		sout << "\t\twrt. dy" << std::endl;
		dy().assign(img());
		cimglist_for( img(),kk )
		cimg_for3x3( img()[kk], x, y, 0, 0, I, T )
		{
			dy().atNXYZC(kk,x,y,0,0) = (Icn - Icp)/2.0;
		}
	}
	if(dt.connected()) {
		sout << "\t\twrt. dt" << std::endl;
		dt().assign(img());
		cimglist_for(img(),kk)
		cimg_forXY( img()[kk], x, y )
		{
			dt().atNXYZC(kk,x,y,0,0) = img().atNXYZC(kk+1,x,y,0,0) - img().atNXYZC(kk,x,y,0,0);
		}
	}
}
#endif /* _SIMPLEDIFF_HXX_ */

