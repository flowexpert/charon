/*  Copyright (C) 2012 Daniel Kondermann

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
/** \file StrictResizeXY.hxx
 *  Implementation of the parameter class StrictResizeXY.
 *  \author <a href="mailto:daniel@kondermann.de">
 *      Daniel Kondermann</a>
 *  \date 02.01.2012
 */

#ifndef _STRICTRESIZEXY_HXX_
#define _STRICTRESIZEXY_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "StrictResizeXY.h"

template <typename T>
StrictResizeXY<T>::StrictResizeXY(const std::string& name) :
		TemplatedParameteredObject<T>(
			"StrictResizeXY", name,
			"<h2>Reduces the size of each frame by a given integer factor "
			"averaging exactly each n x x pixels</h2><br>"
			"This method is used to create no correlation in noise, for "
			"example to downsample reference data. Uneven XY-dimensions "
			"will be treated such that the final row/column are discarded."
		)
{
	ParameteredObject::_setTags("charon-utils;CImg") ;

	ParameteredObject::_addInputSlot(
		in, "in",
		"Input image sequence (all dimensions handled correctly)",
		"CImgList<T>");

	ParameteredObject::_addOutputSlot(
		out, "out",
		"Same sequence but with reduced XY-Dimensions",
		"CImgList<T>");

	ParameteredObject::_addParameter<int>(
		factor, "factor",
		"Resize factor: width and height of the source image will "
		"be divided by this value (round down)",
		2) ;

}

template <typename T>
void StrictResizeXY<T>::execute() {

	out().assign((unsigned int)(in().width()));
	
	if(factor() < 1) {
		ParameteredObject::raise("resize factor must be > 0") ;
	}
	
	cimglist_for(in(), i) {
		const cimg_library::CImg<T>& _in = in()[i] ;
		out()[i].assign(_in.width()/factor(), _in.height()/factor(), _in.depth(), _in.spectrum()); //discard final row/col if uneven XY-dimensions
		cimg_library::CImg<T>& _out = out()[i] ;
		cimg_forXYZC(_out, x, y, z, c) {
			T val = 0 ;
			for(int xx = 0 ; xx < factor() ; xx++)
				for(int yy = 0 ; yy < factor() ; yy++)
					val += _in(x * factor() + xx, y * factor() + yy, z, c) ;
			val /= (factor() * factor()) ;
			_out(x,y,z,c) = val ;
		}
	}
}

#endif /* _STRICTRESIZEXY_HXX_ */
