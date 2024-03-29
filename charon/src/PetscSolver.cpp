/*  This file is part of Charon.

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
/** @file PetscSolver.cpp
 *  Implementation of class PetscSolver.
 *  This is the PETSc implementation of a solver. It has been designed to run on
 *  multiple instances simultaneously.
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */
#define TYPE PetscSolver

#if defined(MSVC) && defined (petscsolver_EXPORTS) 
	#define solver_EXPORTS
	#define roi_EXPORTS
	#define stencil_EXPORTS
#endif
#include <charon/Solvers/PetscSolver.hxx>
#include <charon-utils/DataManagerCImgList.hxx>

extern "C" petscsolver_DECLDIR ParameteredObject * create(
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

extern "C" petscsolver_DECLDIR void destroy(ParameteredObject * b) {
	delete b;
}
/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" petscsolver_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD ;
#else
	return ParameteredObject::RELEASE_BUILD ;
#endif
}
