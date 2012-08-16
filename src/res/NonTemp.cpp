/*  Copyright (C) @Year@ @Author@

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
/** \file @ModuleName@.cpp
 *  Implementation of parameter class @ModuleName@.
 *  \author @AuthorEmail@
 *  \date @Date@
 */

#include <charon-core/ParameteredObject.hxx>
#include "@ModuleName@.h"

@ModuleName@::@ModuleName@(const std::string& name) :
		ParameteredObject(
			"@ModuleName@", name,
			"@PluginDoc@"
		)@ctorAdd@
{
@ctorCont@
}

void @ModuleName@::execute() {
	// your code goes here :-)
}

// the following functions are needed
// for class @ModuleName@ to work as a charon plugin.
extern "C" @modulename@_DECLDIR ParameteredObject*
		create(const std::string& name, ParameteredObject::template_type) {
	return new @ModuleName@(name);
}

extern "C" @modulename@_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" @modulename@_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}
