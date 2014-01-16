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
/** \file RoiOffset.cpp
 *  This file is needed for class RoiOffset to work as a charon plugin.
 *  \author <a href="mailto:jmgottfried@web.de">
 *      Jens-Malte Gottfried</a>
 *  \date 16.01.2014
 */

#include "../include/charon-utils/RoiOffset.hxx"

/// Creates an instance of the plugin
extern "C" roioffset_DECLDIR ParameteredObject*
		create(const std::string & name, ParameteredObject::template_type t) {
	switch(t) {
	case ParameteredObject::TYPE_DOUBLE:
		return new RoiOffset<double>(name);
		break;
	case ParameteredObject::TYPE_FLOAT:
		return new RoiOffset<float>(name);
		break;
	case ParameteredObject::TYPE_INT:
		return new RoiOffset<int>(name);
		break;
	default:
		return 0;
		break;
	}
}

/// Deletes an instance of the plugin
extern "C" roioffset_DECLDIR void destroy(ParameteredObject * b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" roioffset_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}
