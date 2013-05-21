/*  Copyright (C) 2012, 2013
                  Heidelberg Collaboratory for Image Processing

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
/** \file PenaltyCombined.h
 *  Declaration of the parameter class PenaltyCombined.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 05.03.2012
 */

#ifndef _PENALTYCOMBINED_H_
#define _PENALTYCOMBINED_H_

#ifdef _MSC_VER
#ifdef penaltycombined_EXPORTS
/// Visual C++ specific code
#define penaltycombined_DECLDIR __declspec(dllexport)
#else
#define penaltycombined_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define penaltycombined_DECLDIR
#endif

#include <charon/PenaltyFunction.h>

/// Combined PenaltyFunction
/** Penalty function takes two penalty functions and yields a
 *  linear combination of both, whereas weighting depends on
 *  the current itertation number.
 *
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T>
class penaltycombined_DECLDIR PenaltyCombined :
	public PenaltyFunction<T> {
public:
	/// default constructor
	/// \param name          Instance name
	PenaltyCombined(const std::string& name = "");

	/// penalty
	T getPenalty( int n, int x, int y, int z, int c, T );
	/// penalty's 1st order derivative (gradient ;-)) wrt its argument
	T getPenaltyGradient( int n, int x, int y, int z, int c, T );
	/// penalty's 2nd order derivative (Hessian ;-)) wrt its argument
	T getPenaltyHessian( int n, int x, int y, int z, int c, T );

	/// parameter list containing weights for 1st penalty function
	/// wrt the current iteration number
	ParameterList< T > lambdaFst;
	/// parameter list containing weights for 2nd penalty function
	/// wrt the current iteration number
	ParameterList< T > lambdaSnd;

	/// input slot for 1st penalty function
	InputSlot< PenaltyFunction<T>* > fstPenaltyFunction;
	/// input slot for 1st penalty function
	InputSlot< PenaltyFunction<T>* > sndPenaltyFunction;

	/// input slot for current 'level'
	InputSlot< unsigned int > level;

protected:
	/// main function
	virtual void execute();

private:
	/// \name cache members
	//\{
	T _lambda1, _lambda2;
	//\}
};

#endif // _PENALTYCOMBINED_H_

