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
 *  @date 13.01.2010
 */

#include "SquarerootRobustnessTerm.h"
//#include "../RobustnessTerm.cpp"
#include <cmath>


SquarerootRobustnessTerm::SquarerootRobustnessTerm(const std::string& name) :
		RobustnessTerm("SquarerootRobustnessTerm", name, 
			"This class is used for calculationg Robustness Term using the function \f$\Psi(s^2)=\sqrt{s^2+e^2}\f$")
{
}

// calculates derivative of robustness term
double SquarerootRobustnessTerm::DPsi(double s)  {
	double dpsi = s/(sqrt(pow(s, 2) + pow(epsilon(), 2)));
	return dpsi;
}
