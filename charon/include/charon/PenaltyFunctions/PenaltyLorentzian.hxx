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

	ParameteredObject::_setTags("charon-flow;PenaltyFunctions;CImg;deprecated");
}

template <class T>
void PenaltyLorentzian<T>::execute() {
	PenaltyFunction<T>::execute();
	_lamb = this->lambda();
	_sigma = sigma();
}

template <class T>
T PenaltyLorentzian<T>::getPenalty( int, int, int, int, int, T sqrDiff )
{
	T penalty = log( double(1 + sqrDiff/(2*_sigma*_sigma)) ) ;
	return T(this->_lamb * penalty);
}

template <class T>
T PenaltyLorentzian<T>::getPenaltyGradient( int, int, int, int, int, T sqrDiff )
{
	T penaltyGradient = T(1.0) / (2*_sigma*_sigma + sqrDiff);
	return T(this->_lamb * penaltyGradient);
}

template <class T>
T PenaltyLorentzian<T>::getPenaltyHessian( int, int, int, int, int, T sqrDiff )
{
	T penaltyHessian = -pow(double(2.0*_sigma*_sigma + sqrDiff), -2.0);
	return T(this->_lamb * penaltyHessian);
}

#endif /* _PENALTYLORENTZIAN_HXX_ */

