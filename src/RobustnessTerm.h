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
/**
 *  @file RobustnessTerm.h
 *  @brief declaration of class RobustnessTerm used by the IterativeSolver
 *  @author <a href="mailto:techfreaq@web.de">Nina Hernitschek</a>
 *  @date 14.12.2009
 */

#ifndef ROBUSTNESSTERM_H_
#define ROBUSTNESSTERM_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef robustnessterm_EXPORTS
/// Visual C++ specific code
#define robustnessterm_DECLDIR __declspec(dllexport)
#else
#define robustnessterm_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define robustnessterm_DECLDIR
#endif

#include <math.h>
#include "ParameteredObject.h"
/// class which compute the bcce-terms

class robustnessterm_DECLDIR RobustnessTerm
{
protected:
	double e;

public:
	/// constructor for derived classes
	RobustnessTerm();

	/// standard set method for parameter e
	void setE(double e);

	/// standard get function for parameter e
	double getE() const;

	/// calculates robustness term, Psi
	virtual double Psi(double s) const;

	/// calculates derivative of robustness term, DPsi
	virtual double DPsi(double s, double ds) const;
};

#endif /* ROBUSTNESSTERM_H_ */


