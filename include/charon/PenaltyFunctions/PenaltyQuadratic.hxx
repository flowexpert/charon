/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

    This file is part of Charon.

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
/** \file PenaltyQuadratic.hxx
 *  Implementation of the parameter class PenaltyQuadratic.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 25.01.2012
 */

#ifndef _PENALTYQUADRATIC_HXX_
#define _PENALTYQUADRATIC_HXX_

#include "PenaltyQuadratic.h"

#include <charon/PenaltyFunction.hxx>

template <class T>
PenaltyQuadratic<T>::PenaltyQuadratic(const std::string& name) :
  PenaltyFunction<T>(
	     "PenaltyQuadratic", name,
	     "<h2>Implementation of the quadratic penalty function."
	     )
{
}

template <class T>
void PenaltyQuadratic<T>::execute() {
  PARAMETEREDOBJECT_AVOID_REEXECUTION;
  ParameteredObject::execute();

  _lamb = this->lambda();
}

template <class T>
T PenaltyQuadratic<T>::getPenalty( T diff )
{
	T penalty = diff * diff ;
	return T(this->_lamb * penalty);
}

template <class T>
T PenaltyQuadratic<T>::getPenaltyGradient( T diff )
{
	T penaltyGradient = 2 * diff ;
	return T(this->_lamb * penaltyGradient);
}

template <class T>
PenaltyQuadratic<T>::~PenaltyQuadratic()
{
}

#endif /* _PENALTYQUADRATIC_HXX_ */

