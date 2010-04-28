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
 *  @date 13.01.2010
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

#include <CImg.h>
#include "../RobustnessTerm.h"

/// class which uses the function \f$\Psi(s^2)=\sqrt{s^2+e^2}\f$ with
/// parameter compute the bcce-terms


class squarerootrobustnessterm_DECLDIR SquarerootRobustnessTerm:
		public RobustnessTerm
{
public:
	
	/// default constructor
	/// \param name instance name
	SquarerootRobustnessTerm(const std::string& name = "");

	/// calculates derivative of robustness term, DPsi
	virtual double DPsi(double s);

};

#endif
// _SQUAREROOTROBUSTNESSTERM_H_
