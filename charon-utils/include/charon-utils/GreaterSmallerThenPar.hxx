/*  Copyright (C) 2012 Gerald Mwangi

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
/** \file GreaterSmallerThenPar.hxx
 *  Implementation of the parameter class GreaterSmallerThenPar.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 17.02.2012
 */

#ifndef _GREATERSMALLERTHENPAR_HXX_
#define _GREATERSMALLERTHENPAR_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "GreaterSmallerThenPar.h"

template <typename T>
GreaterSmallerThenPar<T>::GreaterSmallerThenPar(const std::string& name) :
		TemplatedParameteredObject<T>(
			"GreaterSmallerThenPar", name,
			"<h2>Compares the input value to a threshold</h2><br>"
			"Compares the input value to a threshold"
		)
{
	ParameteredObject::_setTags("charon-utils") ;

	ParameteredObject::_addInputSlot(
		value, "value",
		"value",
		"T");

	ParameteredObject::_addOutputSlot(
		result, "result",
		"result",
		"bool");

	ParameteredObject::_addParameter< T >(
		threshold, "threshold",
		"value to be compared against",
		0, "T");
	ParameteredObject::_addParameter(
		greater, "greater",
		"val>threshold",
		"bool");
	ParameteredObject::_addParameter(
		smaller, "smaller",
		"val<threshold",
		"bool");
	ParameteredObject::_addParameter(
		equal, "equal",
		"val=threshold. Compatible with parameters greater and smaller",
		"bool");

}

template <typename T>
void GreaterSmallerThenPar<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	if(greater&&equal)
	{
	    result=value>=threshold;
	}
	else if(greater&&!equal)
	    result=value>threshold;
	else if(smaller&&equal)
	    result=value<=threshold;
	else if(smaller&&!equal)
	    result=value<smaller;
	else
	    throw std::string("Invalid parameter settings");

}

#endif /* _GREATERSMALLERTHENPAR_HXX_ */
