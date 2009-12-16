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
/** @file Solver.h
 *  Implementation of class PsiSolver.
 *  @author <a href="mailto:techfreaq@web.de">
 *      Nina Hernitschek</a>
 *  @date 23.11.2009
 */



#ifndef _PSISOLVER_H_
#define _PSISOLVER_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef psisolver_EXPORTS
/// Visual C++ specific code
#define psisolver_DECLDIR __declspec(dllexport)
#else
#define psisolver_DECLDIR __declspec(dllimport)
#endif /*Export or import*/


#else /* No DLL handling or GCC */
/// Not needed with GCC
#define psisolver_DECLDIR
#endif

#include <iostream>
#include <cstdlib>
#include "ParameteredObject.hxx"

class sample_DECLDIR Sample : public ParameteredObject {

	/// CImgList containing the result.
	cimg_library::CImgList<T> result;

	public:
	/*// pointers, that the solver will use
	InputSlot< CImgList<T>* > imgin;
	*/
	InputSlot<cimg_library::CImgList<T> > imgin;

	/// result
	OutputSlot<cimg_library::CImgList<T> > imgout;

	/// default constructor
	PsiSolver(
		const std::string& classname,	///< [in] class name
		const std::string& name = ""	///< [in] instance name
	);

	
	/// encapsulated execute function 
	int PsiSolverExecute();

	/// main function
	virtual void execute();

	
	virtual ~PsiSolver();

};


#endif // _PSISOLVER_H_
