/*  Copyright (C) 2014 Jens-Malte Gottfried

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
/** \file Uint2Int.cpp
 *  Implementation of parameter class Uint2Int.
 *  \author <a href="mailto:jmgottfried@web.de">
 *      Jens-Malte Gottfried</a>
 *  \date 16.01.2014
 */

#include <charon-core/ParameteredObject.hxx>
#include "../include/charon-utils/Uint2Int.h"

Uint2Int::Uint2Int(const std::string& name) :
		ParameteredObject(
			"Uint2Int", name,
			"<h2>Slot Converter</h2><br>converts from uint to int"
		)
{
	ParameteredObject::_setTags("charon-utils") ;

	ParameteredObject::_addInputSlot(
		in, "in",
		"uint input",
		"uint");

	ParameteredObject::_addOutputSlot(
		out, "out",
		"int output",
		"int");

}

void Uint2Int::execute() {
	out() = in();
}

// the following functions are needed
// for class Uint2Int to work as a charon plugin.
extern "C" uint2int_DECLDIR ParameteredObject*
		create(const std::string& name, ParameteredObject::template_type) {
	return new Uint2Int(name);
}

extern "C" uint2int_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" uint2int_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}
