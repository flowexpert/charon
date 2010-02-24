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

/** @file SampleIterativeSolver.cpp
 *  Implementation of class SampleIterativeSolver.
 *  @author <a href="mailto:techfreaq@web.de">
 *      Nina Hernitschek</a>
 *
 *  @date 23.11.2009
 */
#define TYPE SampleIterativeSolver

#if defined(MSVC) && defined (sampleiterativesolver_EXPORTS)
#define solver_EXPORTS
#define DECLDIR __declspec(dllexport)
#else
///Not needed with GCC
#define iterativesolver_DECLDIR
#endif

#include "SampleIterativeSolver.hxx"

using namespace std;

extern "C" iterativesolver_DECLDIR ParameteredObject* create(
		const std::string &name, template_type t) {
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

///Deletes an instance of the plugin
extern "C" iterativesolver_DECLDIR void destroy(ParameteredObject * b) {
	delete b;
}