/*  Copyright (C) 2010 Jens-Malte Gottfried

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
/** \file Diff2Function.h
 *  Declaration of the abstract interface class Diff2Function.
 *  \author <a href="Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried </a>
 *  \date 18.01.2010
 */
#define TYPE DataConstant

#define stencil_EXPORTS
#include <charon/Stencils/DataConstant.hxx>
#include <charon-utils/DataManagerCImgList.hxx>


extern "C" dataconstant_DECLDIR ParameteredObject* create(
		const std::string &name, ParameteredObject::template_type t) {

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

extern "C" dataconstant_DECLDIR void destroy(ParameteredObject * b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" dataconstant_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD ;
#else
	return ParameteredObject::RELEASE_BUILD ;
#endif
}
