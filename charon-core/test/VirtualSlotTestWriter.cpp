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
/** \file VirtualSlotTestWriter.cpp
 *  Implementation of parameter class VirtualSlotTestWriter.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 28.11.2012
 */

#include <charon-core/ParameteredObject.hxx>
#include "VirtualSlotTestWriter.h"

VirtualSlotTestWriter::VirtualSlotTestWriter(const std::string& name) :
		ParameteredObject(
			"VirtualSlotTestWriter", name,
			"<h2>Generates an integer and a CImg image to test the virtual "
			"slots</h2><br>"
			"Generates an integer and a CImg image to test the virtual slots"
		)
{
	ParameteredObject::_addOutputSlot(
		integer, "integer", "integer");
	ParameteredObject::_addOutputSlot(
		image, "image", "image");
}

void VirtualSlotTestWriter::execute() {
	image()="image";
	integer()=42;// The Answer, as you should know! ;)
}

// the following functions are needed
// for class VirtualSlotTestWriter to work as a charon plugin.
extern "C" virtualslottestwriter_DECLDIR ParameteredObject*
		create(const std::string& name, ParameteredObject::template_type) {
	return new VirtualSlotTestWriter(name);
}

extern "C" virtualslottestwriter_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" virtualslottestwriter_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}
