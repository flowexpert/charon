/*  Copyright (C) 2013 Moritz Becker

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
/** \file Min.hxx
 *  Implementation of the parameter class Min.
 *  \author <a href="mailto:moritz.becker@iwr.uni-heidelberg.de">
 *      Moritz Becker</a>
 *  \date 14.03.2013
 */

#ifndef _MIN_HXX_
#define _MIN_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "Min.h"

template <typename T>
Min<T>::Min(const std::string& name) :
		TemplatedParameteredObject<T>(
			"Min", name,
			"<h2>Returns pointwise minimums of two images</h2><br>"
			"Returns piecewise minimums of two images"
		)
{
	ParameteredObject::_addInputSlot(
		image1, "image1",
		"Image 1",
		"CImgList<T>");
	ParameteredObject::_addInputSlot(
		image2, "image2",
		"Image 2",
		"CImgList<T>");

	ParameteredObject::_addOutputSlot(
		min, "min",
		"Minimum of both images",
		"CImgList<T>");

}

template <typename T>
void Min<T>::execute() {
	
	if( image1().size() != image2().size() )
		throw( std::range_error("Min<T>::execute: Different sizes of input lists.") );

	for( size_t i = 0; i < image1().size(); i++)
	{
		cimg_library::CImg<T> minimum = image1()[i].get_min( image2()[i] );
		this->min().push_back( minimum );
	}

}

#endif /* _MIN_HXX_ */
