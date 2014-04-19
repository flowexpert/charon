/*  Copyright (C) 2010 Jens-Malte Gottfried

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
/** \file Diff2LinFunction.h
 *  Declaration of the abstract interface class Diff2LinFunction.
 *  \author <a href="Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried </a>
 *  \date 17.11.2010
 */
#ifndef _DIFF2_LIN_FUNCTION_H_
#define _DIFF2_LIN_FUNCTION_H_

#include "DiffFunction.h"

/// Function interface providing linearization of the first (partial)
/// derivative(s)
class Diff2LinFunction : public virtual DiffFunction {
public:
	/// Calculate "linearization" of first derivative as a replacement
	/// for usage of the second derivative in tailor expansion.
	/** This has been proposed as trick by Bruhn et al 2005 (CLG) and
	 *  avoids to use the second derivative.
	 *
	 *  \f[\tilde f(\mathbf{x})=\frac{\tilde f(\mathbf{x})}{x_j}\cdot x_j
		\approx\frac{\tilde f(\mathbf{x^0})}{x^0_j}\cdot x_j\f]
	 *  where \f$\mathbf{x^0}\f$ are the initial guess values for
	 *  \f$\mathbf{x}\f$ and \f$\tilde f = \partial_{x_i}f\f$ is the first
	 *  derivative of \f$f\f$ by \f$x_i\f$.
	 *
	 *  \param x     \f$x\f$
	 *  \param i,j   Select dimensions (if multidimensional function)
	 *  \returns
	 *      \f$\frac{1}{x_j}\cdot\frac{\partial}{\partial x_i}f(\mathbf{x})\f$
	 */
	virtual double diff2Linearized (
			std::vector<double> x,
			std::vector<double>::size_type i = 0,
			std::vector<double>::size_type j = 0) const {
		return diff(x, i)/x[j];
	}
};

#endif // _DIFF2_LIN_FUNCTION_H_
