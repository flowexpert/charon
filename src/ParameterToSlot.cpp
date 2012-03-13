/*  Copyright (C) 2012 Gerald Mwangi

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
/** \file ParameterToSlot.cpp
 *  Implementation of parameter class ParameterToSlot.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 20.02.2012
 */

#include <charon-core/ParameteredObject.hxx>
#include "../include/charon-utils/ParameterToSlot.h"

ParameterToSlot::ParameterToSlot(const std::string& name) :
		ParameteredObject(
			"ParameterToSlot", name,
			"<h2>Copies a parameter to a slot</h2><br>"
			"Copies a parameter to a slot"
		)
{

	ParameteredObject::_addOutputSlot(
		value, "value",
		"value from parameter",
		"double");

	ParameteredObject::_addParameter< double >(
		param_val, "param_val",
		"param_val to be copied to slot",
		0, "double");

}

void ParameterToSlot::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	value=param_val;
}

// the following functions are needed
// for class ParameterToSlot to work as a charon plugin.
extern "C" parametertoslot_DECLDIR ParameteredObject* create(
		const std::string& name, ParameteredObject::template_type) {
	return new ParameterToSlot(name);
}

extern "C" parametertoslot_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" parametertoslot_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}
