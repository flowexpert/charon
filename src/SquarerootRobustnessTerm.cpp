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
/** @file SquarerootRobustnessTerm.cpp
 *  Implementation of class SquarerootRobustnessTerm.
 *  @author <a href="mailto:techfreaq@web.de">Nina Hernitschek</a>
 *
 *  @date 13.01.2010
 */
#define TYPE SquarerootRobustnessTerm

#include <charon/RobustnessTerms/SquarerootRobustnessTerm.h>
#include <cmath>

SquarerootRobustnessTerm::SquarerootRobustnessTerm(const std::string& name) :
		RobustnessTerm("SquarerootRobustnessTerm", name, 
			"This class is used for calculationg Robustness Term using the "
			"function Psi(s^2)=sqrt{s^2+e^2}")
{
}
	
// calculates derivative of robustness term
double SquarerootRobustnessTerm::DPsi(double s)  {
	double dpsi = s/(sqrt(pow(s, 2) + pow(this->epsilon(), 2)));
	return dpsi;
}

extern "C" squarerootrobustnessterm_DECLDIR ParameteredObject*
		create(const std::string &name, template_type /*t*/) {
	return new TYPE(name);
}

extern "C" squarerootrobustnessterm_DECLDIR
		void destroy(ParameteredObject * b) {
	delete b;
}
