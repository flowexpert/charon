/*

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
/** \file CGSolverHelper.h
 *  Declaration of the parameter class CGSolverHelper.
 *  \author Michael Baron <michael.baron@iwr.uni-heidelberg.de>
 *  \date 22.11.2011
 */

#ifndef _CGSOLVER_HELPER_H_
#define _CGSOLVER_HELPER_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef cgsolverhelper_EXPORTS
/// Visual C++ specific code
#define cgsolverhelper_DECLDIR __declspec(dllexport)
#else
#define cgsolverhelper_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define cgsolverhelper_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// Helper for connecting a CGSolver.
/** This provides current flow/initial flow for further processing.
 *
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 *  \ingroup charon-iterators
 */
template <typename T>
class cgsolverhelper_DECLDIR CGSolverHelper :
		public TemplatedParameteredObject<T> {
public:
	/// original sequence input
	InputSlot < cimg_library::CImgList<T> > in;
	/// initial flow guess
	InputSlot < cimg_library::CImgList<T> > initFlow;
	/// number of flow components for initialization
	Parameter < unsigned int > flowDimensions;
	/// original sequence output
	OutputSlot < cimg_library::CImgList<T> > sequence;
	/// current flow solution
	OutputSlot < cimg_library::CImgList<T> > flow;
	/// self-pointer
	OutputSlot < CGSolverHelper<T>* > self;

	/// default constructor
	CGSolverHelper(const std::string& name = "" /** [in] Instance name*/);

protected:
	/// Update object.
	virtual void execute();
};

#endif // _CGSOLVER_HELPER_H_

