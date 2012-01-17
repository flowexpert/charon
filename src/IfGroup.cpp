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
/** \file IfGroup.cpp
 *  Implementation of parameter class IfGroup.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 17.01.2012
 */

#include "../include/charon-core/ParameteredObject.hxx"
#include "../include/charon-core/IfGroup.h"

IfGroup::IfGroup(const std::string& name) :
		ParameteredObject(
			"IfGroup", name,
			"<h2>Execute a group of objects if a given statement is "
			"true</h2><br>"
			"Execute a group of objects if a given statement is true"
		)
{

	ParameteredObject::_addInputSlot(
		statement, "statement",
		"determines if group is executed",
		"bool");

}

void IfGroup::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	// your code goes here :-)
}

// the following functions are needed
// for class IfGroup to work as a charon plugin.
extern "C" ifgroup_DECLDIR ParameteredObject*
		create(const std::string& name, template_type) {
	return new IfGroup(name);
}

extern "C" ifgroup_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" ifgroup_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}
