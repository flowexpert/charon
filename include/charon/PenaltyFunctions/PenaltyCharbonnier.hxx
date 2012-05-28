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
/** \file PenaltyCharbonnier.hxx
 *  Implementation of the parameter class PenaltyCharbonnier.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 25.01.2012
 */

#ifndef _PENALTYCHARBONNIER_HXX_
#define _PENALTYCHARBONNIER_HXX_

#include "PenaltyCharbonnier.h"

#include <charon/PenaltyFunction.hxx>
#include <cmath>

template <class T>
PenaltyCharbonnier<T>::PenaltyCharbonnier(const std::string& name) :
	PenaltyFunction<T>(
		"PenaltyCharbonnier", name,
		"<h2>Implementation of the Charbonnier penalty function."
	)
{
	this->_addParameter(maxDiff, "maxDiff", "truncation, if abs(diff) > maxDiff", T(127.0));
	this->_addParameter(a, "a", "parameter a", T(0.45));
	this->_addParameter(eps, "eps", "parameter eps", T(0.001));
}

template <class T>
void PenaltyCharbonnier<T>::execute() {
	PenaltyFunction<T>::execute();
	_lamb = this->lambda();
	_maxDiff = maxDiff();
	_a = a();
	_eps = eps();
}

template <class T>
T PenaltyCharbonnier<T>::getPenalty( T sqrDiff )
{
	T penalty;
	if (fabs(double(sqrDiff)) < _maxDiff*_maxDiff)
		penalty = pow( double(sqrDiff + _eps*_eps), double(_a)) ;
	else
		penalty = pow( double(_maxDiff*_maxDiff + _eps*_eps), double(_a)) ;
	return T(this->_lamb * penalty);
}

template <class T>
T PenaltyCharbonnier<T>::getPenaltyGradient( T sqrDiff )
{
	T penaltyGradient;
	if (fabs(double(sqrDiff)) < _maxDiff*_maxDiff)
		penaltyGradient = 2 * _a * pow(sqrDiff, 0.5) * pow( double(sqrDiff + _eps*_eps), double(_a-1) ) ;
	else
		penaltyGradient = T(0.0);
	return T(this->_lamb * penaltyGradient);
}

template <class T>
T PenaltyCharbonnier<T>::getPenaltyHessian( T sqrDiff )
{
	T penaltyHessian, tmp1, tmp2;
	if (fabs(double(sqrDiff)) < _maxDiff*_maxDiff) {
		tmp1 = pow( double(sqrDiff + _eps*_eps), double(_a-1) ) ;
		tmp2 = pow( double(sqrDiff + _eps*_eps), double(_a-2) ) ;
		penaltyHessian = 2 * _a * ( tmp1 + 2*(_a-1) * sqrDiff * tmp2 ) ;
	} else {
		penaltyHessian = T(0.0);
	}
	return T(this->_lamb * penaltyHessian);
}

#endif /* _PENALTYCHARBONNIER_HXX_ */

