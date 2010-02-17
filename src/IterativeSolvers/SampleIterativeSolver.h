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
/** @file SampleIterativeSolver.h
 *  Implementation of class SampleIterativeSolver.
 *  @author <a href="mailto:techfreaq@web.de">
 *      Nina Hernitschek</a>
 *  @date 23.11.2009
 */

#ifndef _SAMPLEITERATIVESOLVER_H_
#define _SAMPLEITERATIVESOLVER_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef sampleiterativecsolver_EXPORTS
/// dll import/export
#define sampleiterativecsolver_DECLDIR __declspec(dllexport)
#else
#define sampleiterativesolver_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// no dll handling on gcc
#define sampleiterativesolver_DECLDIR
#endif

#include "../IterativeSolver.h"
#include "../RobustnessTerm.h"

template <typename T>
class sampleiterativesolver_DECLDIR SampleIterativeSolver : public IterativeSolver<T>
{
public:
	/// default constructor
	SampleIterativeSolver(const std::string& name = "" /**[in] instance name*/);

	/// encapsulated execute function 
	int sampleIterativeSolverExecute();

	/// main function
	virtual void execute();

	/// default destructor
	virtual ~SampleIterativeSolver();

private:

	/// robustness term
	RobustnessTerm* psi;

	/// command line argument counter
	int _argc;
	/// command line argument vector
	char** _argv;
	/// check if initialization has taken place
	static bool _initialized;
};

#endif // _SAMPLEITERATIVESOLVER_H_
