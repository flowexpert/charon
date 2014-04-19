/*  Copyright (C) 2012 Jens-Malte Gottfried

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
/** @file CasedDummy.cpp
 *  CasedDummy plugin class for PluginManager tests
 *  \date 23.07.2012
 *  \author <a href="mailto:jmgottfried@web.de>Jens-Malte Gottfried</a>
 */

#include "CasedDummy.h"

/// Creates an instance of the plugin
extern "C" caseddummy_DLL_PUBLIC ParameteredObject* create(
		const std::string & name, ParameteredObject::template_type /*t*/) {
	return new CasedDummy(name);
}

/// Deletes an instance of the plugin
extern "C" caseddummy_DLL_PUBLIC void destroy(ParameteredObject * b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" caseddummy_DLL_PUBLIC
	ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}
