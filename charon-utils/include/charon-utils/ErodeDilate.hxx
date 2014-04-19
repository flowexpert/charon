/*  Copyright (C) 2011 Daniel Kondermann

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
/** \file ErodeDilate.hxx
 *  Implementation of the parameter class ErodeDilate.
 *  \author <a href="mailto:daniel@kondermann.de">
 *      Daniel Kondermann</a>
 *  \date 19.12.2011
 */

#ifndef _ERODEDILATE_HXX_
#define _ERODEDILATE_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "ErodeDilate.h"
#include <exception>

using namespace std;

template <typename T>
ErodeDilate<T>::ErodeDilate(const std::string& name) :
		TemplatedParameteredObject<T>(
			"ErodeDilate", name,
			"<h2>Erodes/Dilates a binary or grayvalue image</h2><br>"
			"Wraps CImg-Function erode()/dilate()"
		)
{
	ParameteredObject::_setTags("charon-utils;CImg") ;

	ParameteredObject::_addInputSlot(
		in, "in",
		"Input Image",
		"CImgList<T>");

	ParameteredObject::_addOutputSlot(
		out, "out",
		"Result",
		"CImgList<T>");

	ParameteredObject::_addParameter< std::string >(
		method, "method",
		"Erode or Dilate",
		"erode", "{erode;dilate}");
	ParameteredObject::_addParameter< unsigned int >(
		size, "size",
		"Block width of box-shaped structuring element.",
		5, "uint");

}

template <typename T>
void ErodeDilate<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	out() =in();
	if(method() == "erode")
	{
		cimglist_for(in(), i)
			out()[i] =in()[i].get_erode(size());
	}
	else if(method() == "dilate")
	{
		cimglist_for(in(), i)
			out()[i] =in()[i].get_dilate(size());
	}
	else
		ParameteredObject::raise("Parameter method must be either 'erode' or 'dilate'");
}

#endif /* _ERODEDILATE_HXX_ */
