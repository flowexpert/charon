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
/** \file GroupObject.cpp
 *  Implementation of parameter class GroupObject.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 15.10.2012
 */

#include <charon-core/ParameteredObject.hxx>
#include "../include/charon-core/GroupObject.h"

GroupObject::GroupObject(const std::string& name) :
		ParameteredGroupObject(
			"GroupObject", name,
			"<h2>Loads a child workflow and executes it</h2><br>"
			"Loads a child workflow and executes it"
		)
{
	ParameteredObject::_setTags("charon-core") ;
}

void GroupObject::execute() {
	ParameteredGroupObject::execute();
}

// the following functions are needed
// for class GroupObject to work as a charon plugin.
extern "C" groupobject_DECLDIR ParameteredObject*
		create(const std::string& name, ParameteredObject::template_type) {
	return new GroupObject(name);
}

extern "C" groupobject_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" groupobject_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}

#if defined(__GNUC__) && defined (__ELF__)
#include <charon-core/configVersion.h>
/// charon-core version information
__attribute__ ((section (".charon-plugin"), used))
const unsigned char charon_core_version[] = {
	CHARON_CORE_VERSION_MAJOR,
	CHARON_CORE_VERSION_MINOR,
	CHARON_CORE_VERSION_PATCH};
#endif

