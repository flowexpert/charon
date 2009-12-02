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
/** @file SamplePsiSolver.h
 *  Implementation of class SamplePsiSolver.
 *  @author <a href="mailto:techfreaq@web.de">
 *      Nina Hernitschek</a>
 *  @date 23.11.2009
 */
//
//#ifndef _SAMPLEPSISOLVER_H_
//#define _SAMPLEPSISOLVER_H_
//
//#if defined(MSVC) && defined(HANDLE_DLL)
//#ifdef samplepsicsolver_EXPORTS
///// dll import/export
//#define samplepsicsolver_DECLDIR __declspec(dllexport)
//#else
//#define samplepsisolver_DECLDIR __declspec(dllimport)
//#endif /*Export or import*/
//#else /* No DLL handling or GCC */
///// no dll handling on gcc
//#define samplepsisolver_DECLDIR
//#endif
//
//#include "../PsiSolver.h"
//
//
//template <typename T>
//class samplepsisolver_DECLDIR SamplePsiSolver : public PsiSolver<T>
//{
//
//	static void CalcPsi();
//	static void CalcDPsi();
//
//public:
//	/// default constructor
//	SamplePsiSolver(const std::string& name = "" /**[in] instance name*/);
//
//	/// Petsc command line options
//	Parameter<std::string> commandLine;
//
//	/// encapsulated execute function 
//	int samplePsiSolverExecute();
//
//	/// main function
//	virtual void execute();
//
//	/// default destructor
//	virtual ~SamplePsiSolver();
//
//private:
//	/// command line argument counter
//	int _argc;
//	/// command line argument vector
//	char** _argv;
//	/// check if initialization has taken place
//	static bool _initialized;
//};
//
//#endif // _SAMPLEPSISOLVER_H_
