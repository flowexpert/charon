/*  Copyright (C) 2014 Jens-Malte Gottfried

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
/** \file Int2Uint.cpp
 *  Implementation of parameter class Int2Uint.
 *  \author <a href="mailto:jmgottfried@web.de">
 *      Jens-Malte Gottfried</a>
 *  \date 16.01.2014
 */

#include <charon-core/ParameteredObject.hxx>
#include "../include/charon-utils/Int2Uint.h"

Int2Uint::Int2Uint(const std::string& name) :
		ParameteredObject(
			"Int2Uint", name,
			"<h2>Slot Converter</h2><br>converts slots from Int to Uint"
		)
{
	ParameteredObject::_setTags("charon-utils") ;

	ParameteredObject::_addInputSlot(
		in, "in",
		"int input",
		"int");

	ParameteredObject::_addOutputSlot(
		out, "out",
		"uint output",
		"uint");
}

void Int2Uint::execute() {
	out() = in();
}

// the following functions are needed
// for class Int2Uint to work as a charon plugin.
extern "C" int2uint_DECLDIR ParameteredObject*
		create(const std::string& name, ParameteredObject::template_type) {
	return new Int2Uint(name);
}

extern "C" int2uint_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" int2uint_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}
