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
/** @file RobustnessTerm.h
 *  Implementation of class RobustnessTerm.
 *  @author <a href="mailto:techfreaq@web.de">Nina Hernitschek</a>
 *
 *  @date 15.12.2009
 */


#ifndef _SQUAREROOTROBUSTNESSTERM_H_
#define _SQUAREROOTROBUSTNESSTERM_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef squarerootrobustnessterm_EXPORTS
///Visual C++ specific code
#define squarerootrobustnessterm_DECLDIR __declspec(dllexport)
#else
#define squarerootrobustnessterm_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define squarerootrobustnessterm_DECLDIR
#endif

#include "../RobustnessTerm.h"

/// This is the SquareRootRobustnessTerm which is derived from class RobustnessTerm

class squarerootrobustnessterm_DECLDIR SquareRootRobustnessTerm : public RobustnessTerm<T>
{
	public:

		///calculates robustness term, Psi
		double Psi(double s);

		//calculates derivative of robustness term, DPsi
		double DPsi(double s);


};

#endif // _SQUAREROOTROBUSTNESSTERM_H_
