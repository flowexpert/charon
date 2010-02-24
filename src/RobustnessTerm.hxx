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
 *  @file RobustnessTerm.hxx
 *  @brief implementation of class RobustnessTerm
 *  @author <a href="mailto:techfreaq@web.de">Nina Hernitschek</a>
 *  @date 14.12.2009
 */

#ifndef ROBUSTNESSTERM_HXX_
#define ROBUSTNESSTERM_HXX_

#include "Solver.h"
#include "RobustnessTerm.h"


// standard set method for parameter e
template <typename T>
void RobustnessTerm<T>::setE(double e) 
//void RobustnessTerm::setE (double e)
{
	this->e = e;
}

// standard get function for parameter e
template <typename T>
double RobustnessTerm<T>::getE() 
//double RobustnessTerm::getE()
{
	return this->e;
}

#endif /* ROBUSTNESSTERM_HXX_ */
