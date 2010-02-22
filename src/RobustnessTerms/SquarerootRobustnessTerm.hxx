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
/** @file SquarerootRobustnessTerm.hxx
 *  Implementation of class SquarerootRobustnessTerm.
 *  @author <a href="mailto:techfreaq@web.de">Nina Hernitschek</a>
 *
 *  @date 14.12.2009
 */


#ifndef _SQUAREROOTROBUSTNESSTERM_HXX_
#define _SQUAREROOTROBUSTNESSTERM_HXX_

#include "../RobustnessTerm.hxx"
#include "SquarerootRobustnessTerm.h"
#include <math.h>

//namespace RobustnessTerms
//{

//calculates robustness term, Psi
double Psi(double s) {
		
		double psi = sqrt(pow(s, 2) + pow(epsilon, 2));
		
		return psi;
};

//calculates derivative of robustness term, DPsi
	
double DPsi(double s, double ds) {
	
		double dpsi = s*ds/(sqrt(pow(s, 2) + pow(epsilon, 2)));

		return dpsi;
};



//}





#endif // _SQUAREROOTROBUSTNESSTERM_HXX_
