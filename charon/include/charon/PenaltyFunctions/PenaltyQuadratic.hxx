/*  Copyright (C) 2012, 2013
                  Heidelberg Collaboratory for Image Processing

    This file is part of Charon.

    Charon is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Charon is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
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
#include <cmath>

template <class T>
PenaltyQuadratic<T>::PenaltyQuadratic(const std::string& name) :
	PenaltyFunction<T>(
		"PenaltyQuadratic", name,
		"Implementation of the quadratic penalty function.<br>"
		"Attention: It takes a squared difference (generally the residual) "
		"as an argument within its callbacks!<br> "
		"It is parameterized with a scaling parameter sigma, and truncated "
		"by the maxDiff parameter."
	)
{
	this->_addParameter(
		sigma, "sigma",
		"sigma",
		T(1.0) );
	this->_addParameter(
		maxDiff, "maxDiff",
		"parabola will be truncated, if abs(diff) > maxDiff",
		T(127.0) );

	ParameteredObject::_setTags("charon-flow;PenaltyFunctions;CImg");
}

template <class T>
void PenaltyQuadratic<T>::execute() {
	PenaltyFunction<T>::execute();
	_lamb = this->lambda();
	_sigma = sigma();
	_maxDiff = maxDiff();
}

template <class T>
T PenaltyQuadratic<T>::getPenalty( int, int, int, int, int, T sqrDiff )
{
	double penalty;
	if (sqrDiff < _maxDiff*_maxDiff)
		penalty = sqrDiff / pow(double(_sigma),2.0);
	else
		penalty = _maxDiff * _maxDiff / pow(double(_sigma),2.0);
	return T(this->_lamb * penalty);
}

template <class T>
T PenaltyQuadratic<T>::getPenaltyGradient( int, int, int, int, int, T sqrDiff )
{
	T penaltyGradient;
	if (sqrDiff < _maxDiff*_maxDiff)
		penaltyGradient = T(1.0) / pow(double(_sigma),2.0);
	else
		penaltyGradient = T(0.0);
	return T(this->_lamb * penaltyGradient);
}

template <class T>
T PenaltyQuadratic<T>::getPenaltyHessian( int, int, int, int, int, T )
{
	return T(0.0);
}

#endif /* _PENALTYQUADRATIC_HXX_ */

