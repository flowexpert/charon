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

#include "ParameteredObject.hxx"
#include "SquarerootRobustnessTerm.h"
#include <cmath>

template<typename T>
SquarerootRobustnessTerm<T>::SquarerootRobustnessTerm(const std::string& name) :
	RobustnessTerm<T> ("squarerootrobustnessterm", name,
		"please add robustness term class documentation")
{
	this->setE(0);
}

template <typename T>
double SquarerootRobustnessTerm<T>::Psi(double s) const {
	double psi = sqrt(pow(s, 2) + pow(this->e, 2));
	return psi;
}

template <typename T>
double SquarerootRobustnessTerm<T>::DPsi(double s, double ds) const {
	double dpsi = s*ds/(sqrt(pow(s, 2) + pow(this->e, 2)));
	return dpsi;
}


#endif // _SQUAREROOTROBUSTNESSTERM_HXX_
