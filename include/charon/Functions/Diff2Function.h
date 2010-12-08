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
/** \file Diff2Function.h
 *  Declaration of the abstract interface class Diff2Function.
 *  \author <a href="Jens-Malte.Gottfried@iwr.uni-heidelberg.de>
 *      Jens-Malte Gottfried </a>
 *  \date 16.11.2010
 */
#ifndef _DIFF2_FUNCTION_H_
#define _DIFF2_FUNCTION_H_

#include "DiffFunction.h"

/// Function interface providing first (partial) derivative(s)
class Diff2Function : public virtual DiffFunction {
public:
	/// Calculate second partial derivative wrt \f$x_i\f$ and \f$x_j\f$.
	/** For convenience and to simplify handling of 1-dim functions,
	 *  a default of i=j=0 is specified.
	 *  \param x    evaluation location
	 *  \param i,j  select arguments for partial derivatives
	 *  \returns    \f$\partial_{x_i}\partial_{x_j} f(\mathbf{x})\f$
	 */
	virtual double diff2(
			std::vector<double> x,
			std::vector<double>::size_type i = 0,
			std::vector<double>::size_type j = 0
	) const = 0;
};

#endif // _DIFF2_FUNCTION_H_
