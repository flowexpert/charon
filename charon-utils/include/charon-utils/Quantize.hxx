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
/** \file Quantize.hxx
 *  Implementation of the parameter class Quantize.
 *  \author <a href="mailto:moritz.becker@iwr.uni-heidelberg.de">
 *      Moritz Becker</a>
 *  \date 17.03.2013
 */

#ifndef _QUANTIZE_HXX_
#define _QUANTIZE_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "Quantize.h"

template <typename T>
Quantize<T>::Quantize(const std::string& name) :
		TemplatedParameteredObject<T>(
			"Quantize", name,
			"<h2>Applies quantize function of CImg library</h2><br>"
			"Applies quantize function of CImg library"
		)
{
	ParameteredObject::_addInputSlot(
		image, "image",
		"Input images",
		"CImgList<T>");

	ParameteredObject::_addOutputSlot(
		imagesOut, "imagesOut",
		"Output images",
		"CImgList<T>");

	ParameteredObject::_addParameter< int >(
		levels, "levels",
		"Number of quantize levels",
		2, "int");

}

template <typename T>
void Quantize<T>::execute() {
	
	if( levels() <= 1 )
		throw( std::runtime_error("Quantize<T>::execute(): Levels !> 1") );

	for( size_t i = 0; i < this->image().size(); i++ )
	{
		this->imagesOut().push_back( image()[i].get_quantize( levels() ) );
	}

}

#endif /* _QUANTIZE_HXX_ */
