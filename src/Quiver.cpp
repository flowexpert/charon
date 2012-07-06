/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

    This file is part of charon-free.

    charon-free is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    charon-free is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with charon-free.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file Quiver.cpp
 *  This file is needed for class Quiver to work as a plugin.
 *  \author Michael Baron
 *  \date 23.04.2012
 */

/// Class name of the plugin
#define TYPE Quiver

#include <charon-utils/Quiver.hxx>

/// Creates an instance of the plugin
extern "C" quiver_DECLDIR ParameteredObject* create(
		const std::string & name, ParameteredObject::template_type t) {
	switch(t) {
	case ParameteredObject::TYPE_DOUBLE:
		return new TYPE<double>(name);
		break;
	case ParameteredObject::TYPE_FLOAT:
		return new TYPE<float>(name);
		break;
	case ParameteredObject::TYPE_INT:
		return new TYPE<int>(name);
		break;
	default:
		return new TYPE<int>(name);
		break;
	}
}

/// Deletes an instance of the plugin
extern "C" quiver_DECLDIR void destroy(ParameteredObject * b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" quiver_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}

