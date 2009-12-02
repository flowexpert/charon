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
/** @file SamplePsiSolver.cpp
 *  Implementation of class SamplePsiSolver.
 *  This is the PETSc implementation of a solver. It has been designed to run on
 *  multiple instances simultaneously.
 *  @author <a href="mailto:techfreaq@web.de">
 *      Nina Hernitschek</a>
 *
 *  @date 23.11.2009
 *//*
#define TYPE SamplePsiSolver

#if defined(MSVC) && defined (samplepsisolver_EXPORTS)
#define psisolver_EXPORTS
#define DECLDIR __declspec(dllexport)
#else
///Not needed with GCC
#define DECLDIR
#endif

#include "SamplePsiSolver.hxx"

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
}*/