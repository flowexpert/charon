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
/** @file CGSolver.h
 *  Declaration of the parameter class CGSolver.
 *  @author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  @date 30.08.2011
 */

#ifndef _CGSOLVER_H_
#define _CGSOLVER_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef cgsolver_EXPORTS
/// dll import/export
#define cgsolver_DECLDIR __declspec(dllexport)
#else
#define cgsolver_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// no dll handling on gcc
#define cgsolver_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon/EnergyStencil.h>
#include <CImg.h>
#include <vector>

//  Conjugate Gradient method for minimization of differentiable multivariate functions.
/**
 */
template <typename T>
class cgsolver_DECLDIR CGSolver : public TemplatedParameteredObject<T>
{
public:
	InputSlot< EnergyStencil<T>* > energyStencils;
	InputSlot< cimg_library::CImgList<T> > initialParameters;
	OutputSlot< cimg_library::CImgList<T> > optimizedParameters;

	//  length parameter, see "minimize.m" for details
	Parameter< int > length;

	/// default constructor
	CGSolver(const std::string& name = "" /**[in] instance name*/);

	/// main function
	void execute();

	void minimize(
              std::vector<T> &X,
              std::vector<T> &startingPoint_X,
              typename std::set<AbstractSlot<EnergyStencil<T>*>*>::const_iterator &stencilsBegin,
              typename std::set<AbstractSlot<EnergyStencil<T>*>*>::const_iterator &stencilsEnd,
              int length
              );

	/// default destructor
	~CGSolver();
private:
  int _pSize;
  int _pWidth, _pHeight, _pDepth;
  int _pSpectrum;
};

#endif // _CGSOLVER_H_

