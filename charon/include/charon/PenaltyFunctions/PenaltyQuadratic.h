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
/** \file PenaltyQuadratic.h
 *  Declaration of the parameter class PenaltyQuadratic.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 25.01.2012
 */

#ifndef _PENALTYQUADRATIC_H_
#define _PENALTYQUADRATIC_H_

#ifdef _MSC_VER
#ifdef penaltyquadratic_EXPORTS
/// Visual C++ specific code
#define penaltyquadratic_DECLDIR __declspec(dllexport)
#else
#define penaltyquadratic_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define penaltyquadratic_DECLDIR
#endif

#include <charon/PenaltyFunction.h>

/// Quadratic PenaltyFunction
/** Penalty function yielding the quadratic penalty.
 *
 *  It yields x ^ 2 wrt to its argument x
 *  and its gradient 2 * x .
 *
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T>
class penaltyquadratic_DECLDIR PenaltyQuadratic :
	public PenaltyFunction<T> {
public:
	/// default constructor
	/// \param name          Instance name
	PenaltyQuadratic(const std::string& name = "");

	/// sigma
	Parameter< T > sigma;

	/// truncation difference
	Parameter< T > maxDiff;

	/// penalty
	T getPenalty( int, int, int, int, int, T );

	/// penalty's 1st order derivative (gradient ;-)) wrt its argument
	T getPenaltyGradient( int, int, int, int, int, T );

	/// penalty's 2nd order derivative (Hessian ;-)) wrt its argument
	T getPenaltyHessian( int, int, int, int, int, T );

protected:
	/// main function
	virtual void execute();

private:
	/// \name cache members
	//\{
	T _lamb;
	T _sigma;
	T _maxDiff;
	//\}
};

#endif // _PENALTYQUADRATIC_H_

