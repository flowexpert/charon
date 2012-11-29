/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file PyramidRescaleMatlab.cpp
 *  This file is needed for class PyramidRescaleMatlab to work as a plugin.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 11.02.2011
 */

///Class name of the plugin
#define TYPE PyramidRescaleMatlab

#define roi_EXPORTS

#include <charon-utils/Convolution.hxx>
#include <charon/PyramidRescaleMatlab.hxx>
#include <charon-utils/DataManagerCImgList.hxx>

/// Creates an instance of the plugin
extern "C" pyramidrescalematlab_DECLDIR ParameteredObject* create(
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
extern "C" pyramidrescalematlab_DECLDIR void destroy(ParameteredObject * b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" pyramidrescalematlab_DECLDIR
ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD ;
#else
	return ParameteredObject::RELEASE_BUILD ;
#endif
}
