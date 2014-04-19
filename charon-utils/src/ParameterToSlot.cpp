/*  Copyright (C) 2013 Gerald Mwangi

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
/** \file ParameterToSlot.cpp
 *  This file is needed for class ParameterToSlot to work as a charon plugin.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 20.09.2013
 */

#include <charon-utils/ParameterToSlot.hxx>

/// Creates an instance of the plugin
extern "C" parametertoslot_DECLDIR ParameteredObject*
		create(const std::string & name, ParameteredObject::template_type t) {
	switch(t) {
	case ParameteredObject::TYPE_DOUBLE:
		return new ParameterToSlot<double>(name);
		break;
	case ParameteredObject::TYPE_FLOAT:
		return new ParameterToSlot<float>(name);
		break;
	case ParameteredObject::TYPE_INT:
		return new ParameterToSlot<int>(name);
		break;
	default:
		return 0;
		break;
	}
}

/// Deletes an instance of the plugin
extern "C" parametertoslot_DECLDIR void destroy(ParameteredObject * b) {
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
