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
/** @file IterativeSolver.h
 *  Implementation of class IterativeSolver.
 *  @author <a href="mailto:techfreaq@web.de">
 *      Nina Hernitschek</a>
 *  @date 23.11.2009
 */

#ifndef _ITERATIVESOLVER_H_
#define _ITERATIVESOLVER_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef iterativesolver_EXPORTS
/// Visual C++ specific code
#define iterativesolver_DECLDIR __declspec(dllexport)
#else
#define iterativesolver_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define iterativesolver_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>
#include "RobustnessTerm.h"
#include "Interpolator.h"


template <typename T>
class iterativesolver_DECLDIR IterativeSolver :
		public TemplatedParameteredObject<T> {

private:
	/// forbid instantiation
	IterativeSolver();

protected:
	/// constructor for derived classes
	IterativeSolver(
		const std::string& classname,  ///< [in] class name
		const std::string& name,       ///< [in] instance name
		const std::string& doc         ///  [in] class docstring
	);

public:
	/// \name pointers used by the solver
	//  \{
	InputSlot<cimg_library::CImgList<T> > imgListIn;
	InputSlot<cimg_library::CImgList<T> > flowListIn;
	InputSlot<Interpolator<T>*> interpolator;
	InputSlot<RobustnessTerm<T>*> robustnessTerm;
	//  \}

	/// \name result
	//  \{
	OutputSlot<cimg_library::CImg<T> > flowOut;
	OutputSlot<cimg_library::CImgList<T> > imgListOut;
	//  \}
};


#endif // _ITERATIVESOLVER_H_
