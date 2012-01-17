/*  This file is part of Charon.

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
/** @file L2Norm.cpp
 *  Implementation of class Gbcce.
 *  This is the General Brightness Change Constraint Equation stencil which is
 *  derived from the stencil class.
 *  @see Stencil.cpp
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 14.09.2009
 */

#define TYPE L2Norm

#if defined(MSVC) && defined (l2norm_EXPORTS)
#define stencil_EXPORTS
#define DECLDIR __declspec(dllexport)
#else
///Not needed with GCC
#define DECLDIR
#endif

#include <charon/Stencils/L2Norm.hxx>
#include <charon-utils/DataManagerCImgList.hxx>

extern "C" DECLDIR ParameteredObject * create(const std::string &name, template_type t) {
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

extern "C" DECLDIR void destroy(ParameteredObject * b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD ;
#else
	return ParameteredObject::RELEASE_BUILD ;
#endif
}
