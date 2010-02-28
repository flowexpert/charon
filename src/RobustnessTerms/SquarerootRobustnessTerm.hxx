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

//#include "../RobustnessTerm.h"
#include "ParameteredObject.hxx"
#include "SquarerootRobustnessTerm.h"
#include <cmath>

//namespace RobustnessTerms
//{

//constructor
/*
template <typename T>
SquarerootRobustnessTerm<T>::SquarerootRobustnessTerm(const std::string& name) : 
		RobustnessTerm<T>("SquarerootRobustnessTerm", name, "class for Squareroot Robustness Terms")
{
	e=0;
}*/

template<typename T>
SquarerootRobustnessTerm<T>::SquarerootRobustnessTerm(const std::string& name) :
	RobustnessTerm<T> ("squarerootrobustnessterm", name)//, functor()
{
	e=0;
	//	brightnessFunctor = &functor;
	//_addInputSlot(sequence, "sequence", "Sequence of Images", "CImgList");
	//this->setFunctorParams(0.02);
}

//calculates robustness term, Psi
template <typename T>
double SquarerootRobustnessTerm<T>::Psi(const double s) {

		double psi = sqrt(pow(s, 2) + pow(e, 2));

		return psi;
};

//calculates derivative of robustness term, DPsi
template <typename T>
double SquarerootRobustnessTerm<T>::DPsi(const double s, const double ds) {
	
		double dpsi = s*ds/(sqrt(pow(s, 2) + pow(e, 2)));

		return dpsi;
};


#endif // _SQUAREROOTROBUSTNESSTERM_HXX_
