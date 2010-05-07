/*  Copyright (C) 2009 @Author@

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
/// @file @pluginName@.cpp
/// @author @Author@
/// @date @date@
/// Needed for class @pluginName@ to work as plugin.
#include "@pluginName@.h"

@pluginName@(const std::string& name = "") :
            ParameteredObject(
            "@pluginName@", name,
            "@PluginDocu@")
            @multi/optional@
			@ParameterListDefault@

{
	// parameters
	@addParameter@

	@addParameterList@


	// slots
	@add-In/Out@
}

void @pluginName@::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	// your code goes here :-)
}

extern "C" @pluginNameLower@_DECLDIR ParameteredObject*
		create(const std::string& name, template_type /*t*/) {
	return new @pluginName@(name);
}

extern "C" @pluginNameLower@_DECLDIR void destroy(ParameteredObject * b) {
	delete b;
}
