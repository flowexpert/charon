/*  Copyright (C) 2012, 2013
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

/** \file SimpleDiff.hxx
 *  Implementation of the parameter class SimpleDiff.
 *  \author Michael Baron
 *  \date 09.10.2012 - 09.01.2013
 */

#ifndef _SIMPLEDIFF_HXX_
#define _SIMPLEDIFF_HXX_

#include "SimpleDiff.h"

#include <charon/Warper.hxx>

template <typename T>
SimpleDiff<T>::SimpleDiff(const std::string& name) :
		TemplatedParameteredObject<T>("SimpleDiff", name,
			"Calculate 2D derivatives.<br><br>"
			"This plugin calculates two dimensional derivatives.")
{
	ParameteredObject::_addInputSlot(
			img, "img",
			"image input", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			warper, "warper",
			"warper input", "Warper<T>*");

	ParameteredObject::_addOutputSlot(
			dx, "dx", "derivative wrt x", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			dy, "dy", "derivative wrt y", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			dt, "dt", "derivative wrt t", "CImgList<T>");

	ParameteredObject::_setTags("charon-flow;Differentiators;CImg");
}

template <typename T>
void SimpleDiff<T>::execute() {
	sout << "\tcalculating derivatives:" << std::endl;

	Warper<T> *_warper = warper();

	if(dx.connected()) {
		sout << "\t\twrt. dx" << std::endl;
		dx().assign(img());
		cimg_forXYZC( img()[0], x, y, z, c )
		{
			dx().atNXYZC(0,x,y,0,c) = (   img().atNXYZC(0, _warper->getX(c,x-2,y,x,y), _warper->getY(c,x,y,x,y), 0,c)
			                          - 8*img().atNXYZC(0, _warper->getX(c,x-1,y,x,y), _warper->getY(c,x,y,x,y), 0,c)
			                          + 8*img().atNXYZC(0, _warper->getX(c,x+1,y,x,y), _warper->getY(c,x,y,x,y), 0,c)
			                          -   img().atNXYZC(0, _warper->getX(c,x+2,y,x,y), _warper->getY(c,x,y,x,y), 0,c) ) / T(12.0);
		}
		cimg_forXY( img()[0], x, y )
		{
			dx().atNXYZC(0,x,y,0,0) += dx().atNXYZC(0,x,y,0,1) ;
			dx().atNXYZC(0,x,y,0,0) /= T(2.0) ;
			dx().atNXYZC(0,x,y,0,1) =  dx().atNXYZC(0,x,y,0,0) ;
		}
	}
	if(dy.connected()) {
		sout << "\t\twrt. dy" << std::endl;
		dy().assign(img());
		cimg_forXYZC( img()[0], x, y, z, c )
		{
			dy().atNXYZC(0,x,y,0,c) = (   img().atNXYZC(0, _warper->getX(c,x,y,x,y), _warper->getY(c,x,y-2,x,y), 0,c)
			                          - 8*img().atNXYZC(0, _warper->getX(c,x,y,x,y), _warper->getY(c,x,y-1,x,y), 0,c)
			                          + 8*img().atNXYZC(0, _warper->getX(c,x,y,x,y), _warper->getY(c,x,y+1,x,y), 0,c)
			                          -   img().atNXYZC(0, _warper->getX(c,x,y,x,y), _warper->getY(c,x,y+2,x,y), 0,c) ) / T(12.0);
		}
		cimg_forXY( img()[0], x, y )
		{
			dy().atNXYZC(0,x,y,0,0) += dy().atNXYZC(0,x,y,0,1) ;
			dy().atNXYZC(0,x,y,0,0) /= T(2.0) ;
			dy().atNXYZC(0,x,y,0,1) =  dy().atNXYZC(0,x,y,0,0) ;
		}
	}
	if(dt.connected()) {
		sout << "\t\twrt. dt" << std::endl;
		dt().assign(img());
		cimg_forXYZC( img()[0], x, y, z, c )
		{
			dt().atNXYZC(0,x,y,0,c) = img().atNXYZC(0, _warper->getX(c,x,y,x,y), _warper->getY(c,x,y,x,y), 0,1)
			                        - img().atNXYZC(0, _warper->getX(c,x,y,x,y), _warper->getY(c,x,y,x,y), 0,0);
		}
	}
}
#endif /* _SIMPLEDIFF_HXX_ */

