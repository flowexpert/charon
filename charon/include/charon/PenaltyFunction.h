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
/** @file PenaltyFunction.h
 *  Declaration of the parameter class PenaltyFunction.
 *  @author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *
 *  @date 25.01.2012
 */

#ifndef _PENALTYFUNCTION_H_
#define _PENALTYFUNCTION_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef penaltyfunction_EXPORTS
///Visual C++ specific code
#define penaltyfunction_DECLDIR __declspec(dllexport)
#else
#define penaltyfunction_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define penaltyfunction_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>

/// Base class for PenaltyFunctions
/** A penalty function maps a difference to its residual value.
 */
template <class T>
class penaltyfunction_DECLDIR PenaltyFunction : public TemplatedParameteredObject<T> {
public:
	/// Lambda coefficient of the penalty function.
	Parameter<T> lambda;

	/// Output slot containing the this-pointer of the object
	OutputSlot<PenaltyFunction<T>*> out;

	/// default constructor
	PenaltyFunction(const std::string& classname /**[in] class name*/,
		const std::string& name /**[in] instance name*/,
		const std::string& doc /**[in] stencil documentation*/);

	/// function yielding penalty
	virtual T getPenalty( int n, int x, int y, int z, int c, T squaredDifference ) = 0;

	/// function yielding penalty gradient wrt its argument
	virtual T getPenaltyGradient( int n, int x, int y, int z, int c, T squaredDifference ) = 0;

	/// function yielding penalty Hessian wrt its argument
	virtual T getPenaltyHessian( int n, int x, int y, int z, int c, T squaredDifference ) = 0;

protected:
	/// \implements ParameteredObject::execute()
	virtual void execute();
};

#endif

