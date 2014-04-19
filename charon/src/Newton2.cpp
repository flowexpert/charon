/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

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
/** @file Newton2.cpp
 *  Implementation of class Newton2.
 *  @author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *
 *  @date 06.02.2012
 */
#define TYPE Newton2

#if defined(MSVC) && defined (newton2_EXPORTS) 
#define solver_EXPORTS
#define roi_EXPORTS
#endif
#include <charon/Solvers/Newton2.hxx>

extern "C" newton2_DECLDIR ParameteredObject * create(
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

extern "C" newton2_DECLDIR void destroy(ParameteredObject * b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatible runtime libs
extern "C" newton2_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
        return ParameteredObject::DEBUG_BUILD ;
#else
        return ParameteredObject::RELEASE_BUILD ;
#endif
}

