/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/// @file Constant.cpp
/// This file is needed for the Roi class to work as a plugin.
/// @author <a href="bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
/// @date 24.08.2009

///Class name of the plugin
#define TYPE BrightnessModels::Constant

#define brightnessmodel_EXPORTS
#include <charon-core/ParameteredObject.hxx>
#include <charon/BrightnessModels/Constant.hxx>

/// Creates an instance of the plugin
extern "C" brightnessmodels_constant_DECLDIR
ParameteredObject* create(const std::string & name, template_type t) {
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
extern "C" brightnessmodels_constant_DECLDIR
void destroy(ParameteredObject * b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" brightnessmodels_constant_DECLDIR
ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD ;
#else
	return ParameteredObject::RELEASE_BUILD ;
#endif
}
