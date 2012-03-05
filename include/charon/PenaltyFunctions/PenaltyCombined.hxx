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
/** \file PenaltyCombined.hxx
 *  Implementation of the parameter class PenaltyCombined.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 05.03.2012
 */

#ifndef _PENALTYCOMBINED_HXX_
#define _PENALTYCOMBINED_HXX_

#include "PenaltyCombined.h"

#include <charon/PenaltyFunction.hxx>
#include <cmath>

template <class T>
PenaltyCombined<T>::PenaltyCombined(const std::string& name) :
  PenaltyFunction<T>(
	     "PenaltyCombined", name,
	     "<h2>Implementation of the Charbonnier penalty function."
	     )
{
	this->_addParameter(lambdaFst, "lambdaFst", "weights for 1st penalty function","ParameterList<T>");
	this->_addParameter(lambdaSnd, "lambdaSnd", "weights for 2nd penalty function","ParameterList<T>");
	this->_addInputSlot(fstPenaltyFunction, "fstPenaltyFunction", "first penalty function","PenaltyFunction<T>*");
	this->_addInputSlot(sndPenaltyFunction, "sndPenaltyFunction", "second penalty function","PenaltyFunction<T>*");
	this->_addInputSlot(level, "level", "level to select scaling weights from lists");
}

template <class T>
void PenaltyCombined<T>::execute() {
  PARAMETEREDOBJECT_AVOID_REEXECUTION;
  ParameteredObject::execute();

  unsigned int _level = level();
  _lambda1 = lambdaFst()[_level];
  _lambda2 = lambdaSnd()[_level];
//  sout << "(II) PenaltyCombined :: level, lambda[1,2] " << _level << ", " << _lambda1 << ", " << _lambda2 << std::endl;
}

template <class T>
T PenaltyCombined<T>::getPenalty( T diff )
{
	T penalty;
	penalty = _lambda1 * fstPenaltyFunction()->getPenalty( diff )
	        + _lambda2 * sndPenaltyFunction()->getPenalty( diff );
	return T(penalty);
}

template <class T>
T PenaltyCombined<T>::getPenaltyGradient( T diff )
{
	T penaltyGradient;
	penaltyGradient = _lambda1 * fstPenaltyFunction()->getPenaltyGradient( diff )
	                + _lambda2 * sndPenaltyFunction()->getPenaltyGradient( diff );
	return T(penaltyGradient);
}

template <class T>
T PenaltyCombined<T>::getPenaltyHessian( T diff )
{
	T penaltyHessian;
	penaltyHessian = _lambda1 * fstPenaltyFunction()->getPenaltyHessian( diff )
	               + _lambda2 * sndPenaltyFunction()->getPenaltyHessian( diff );
	return T(penaltyHessian);
}

template <class T>
PenaltyCombined<T>::~PenaltyCombined() {}

#endif /* _PENALTYCOMBINED_HXX_ */

