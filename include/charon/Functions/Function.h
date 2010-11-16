/*  Copyright (C) 2010 Jens-Malte Gottfried

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
/** \file Function.h
 *  Declaration of the abstract interface class SimpleFunction.
 *  \author <a href="Jens-Malte.Gottfried@iwr.uni-heidelberg.de>
 *      Jens-Malte Gottfried </a>
 *  \date 16.11.2010
 */
#ifndef _SIMPLE_FUNCTION_H_
#define _SIMPLE_FUNCTION_H_

#include <vector>

/// Abstract base class for arbitrary functions.
/** Provides function call method and operator().
 *  Function arguments are passed as std::vector<double> which may
 *  be one-dimensional or multidimensional, i.e. mappings
 *  \f$\mathcal{R^n}\to\mathcal{R}\f$ may be represented.
 *  The dimensionality is given as static member function.
 */
class Function {
public:
	/// calculate function value at location x
	/** \param x    evaluation location
	 */
	virtual double calculate(std::vector<double> x) const = 0;

	/// return number of dimensions accepted
	virtual std::vector<double>::size_type dims() = 0;

	/// easy calculation call
	/** \param x    evaluation location
	 *  \returns    calculate(x)
	 */
	inline double operator()(std::vector<double> x) const {
		return calculate(x);
	}

	virtual ~Function(){};
};

#endif // _SIMPLE_FUNCTION_H_
