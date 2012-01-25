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
/** \file PenaltyLorentzian.hxx
 *  Implementation of the parameter class PenaltyLorentzian.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 25.01.2012
 */

#ifndef _PENALTYLORENTZIAN_HXX_
#define _PENALTYLORENTZIAN_HXX_

#include "PenaltyLorentzian.h"

#include <charon/PenaltyFunction.hxx>
#include <cmath>

template <class T>
PenaltyLorentzian<T>::PenaltyLorentzian(const std::string& name) :
  PenaltyFunction<T>(
	     "PenaltyLorentzian", name,
	     "<h2>Implementation of the Lorentzian penalty function."
	     )
{
	this->_addParameter(sigma, "sigma", "parameter sigma", T(0.03));
}

template <class T>
void PenaltyLorentzian<T>::execute() {
  PARAMETEREDOBJECT_AVOID_REEXECUTION;
  ParameteredObject::execute();

  _lamb = this->lambda();
  _sigma = sigma();
}

template <class T>
T PenaltyLorentzian<T>::getPenalty( T diff )
{
	T penalty = log( 1 + (diff*diff)/(2*_sigma*_sigma) ) ;
	return T(this->_lamb * penalty);
}

template <class T>
T PenaltyLorentzian<T>::getPenaltyGradient( T diff )
{
	T penaltyGradient = (diff / (_sigma*_sigma)) / ( 1 + (diff*diff)/(2*_sigma*_sigma) );
	return T(this->_lamb * penaltyGradient);
}

template <class T>
PenaltyLorentzian<T>::~PenaltyLorentzian()
{
}

#endif /* _PENALTYLORENTZIAN_HXX_ */

