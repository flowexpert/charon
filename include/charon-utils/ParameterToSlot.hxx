/*  Copyright (C) 2013 Gerald Mwangi

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
/** \file ParameterToSlot.hxx
 *  Implementation of the parameter class ParameterToSlot.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 20.09.2013
 */

#ifndef _PARAMETERTOSLOT_HXX_
#define _PARAMETERTOSLOT_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "ParameterToSlot.h"

template <typename T>
ParameterToSlot<T>::ParameterToSlot(const std::string& name) :
		TemplatedParameteredObject<T>(
			"ParameterToSlot", name,
			"<h2>Copies a parameter to a slot</h2><br>"
			"Copies a parameter to a slot"
		)
{
	ParameteredObject::_addOutputSlot(
		value, "value",
		"value from parameter",
		"T");

	ParameteredObject::_addParameter< T >(
		param_val, "param_val",
		"param_val to be copied to slot",
		0, "T");
}

template <typename T>
void ParameterToSlot<T>::execute() {
	value=param_val;
}

#endif /* _PARAMETERTOSLOT_HXX_ */
