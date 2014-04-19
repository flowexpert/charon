/*  Copyright (C) 2013 Moritz Becker

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
/** \file DiscretizeByThreshold.hxx
 *  Implementation of the parameter class DiscretizeByThreshold.
 *  \author <a href="mailto:moritz.becker@iwr.uni-heidelberg.de">
 *      Moritz Becker</a>
 *  \date 17.03.2013
 */

#ifndef _DISCRETIZEBYTHRESHOLD_HXX_
#define _DISCRETIZEBYTHRESHOLD_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "DiscretizeByThreshold.h"

template <typename T>
DiscretizeByThreshold<T>::DiscretizeByThreshold(const std::string& name) :
		TemplatedParameteredObject<T>(
			"DiscretizeByThreshold", name,
			"<h2>Discretizes an image by a given threshold</h2><br>"
			"Discretizes an image by a given threshold. All pixels >= "
			"threshold will be set to 1. For display purposes normalize aftewards."
		)
{
	ParameteredObject::_addInputSlot(
		images, "images",
		"Images to discretize",
		"CImgList<T>");

	ParameteredObject::_addOutputSlot(
		imagesOut, "imagesOut",
		"Discretized images with maximum values of 1.",
		"CImgList<T>");

	ParameteredObject::_addParameter< float >(
		threshold, "threshold",
		"Threshold for pixel values",
		125, "float");

}

template <typename T>
void DiscretizeByThreshold<T>::execute() {

	for( size_t i = 0; i < images().size(); i++ )
	{
		imagesOut().push_back( images()[i].get_threshold( threshold() ) );
	}

}

#endif /* _DISCRETIZEBYTHRESHOLD_HXX_ */
