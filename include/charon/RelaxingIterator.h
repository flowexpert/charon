/*  Copyright (C) 2009 Jens-Malte Gottfried <jmgottfried@web.de>

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
/** \file RelaxingIterator.h
 *  Declaration of the parameter class RelaxingIterator.
 *  \author Jens-Malte Gottfried <jmgottfried@web.de>
 *  \date 05.02.2010
 */

#ifndef _RELAXINGITERATOR_H_
#define _RELAXINGITERATOR_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef relaxingiterator_EXPORTS
/// Visual C++ specific code
#define relaxingiterator_DECLDIR __declspec(dllexport)
#else
#define relaxingiterator_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define relaxingiterator_DECLDIR
#endif

#include "SimpleIterator.h"
#include <charon/Stencil.h>

/// Iterator based on SimpleIterator class but with additional relaxing feature.
/** Stencils may be connected and iteration causes the Stencil weights to be
 *  multiplied by a factor moving from 0 to 1 (some with increasing, some with
 *  decreasing weight).
 *  This helps to use convex optimization to generate a good initial guess and
 *  then switch to some non-convex optimization problem.
 */
template <typename T>
class relaxingiterator_DECLDIR RelaxingIterator :
	public SimpleIterator<T> {
private:
	std::vector<T> _initialLambdas;   ///< lambda values of initial stencils
	std::vector<T> _finalLambdas;     ///< lambda values of final stencils

protected:
	virtual void _preIterationHook();
	virtual void _beginIterationHook();
	virtual void _afterIterationHook();

public:
	/// Stencils which weight will be decreased, starting with full weight.
	InputSlot < Stencil<T>* > initial;
	/// Stencils which weight will be increased, starting with zero weight.
	InputSlot < Stencil<T>* > final;

	/// mixture at first iteration
	Parameter<double> initialWeight;
	/// mixture at last iteration
	Parameter<double> finalWeight;

	/// create a new RelaxingIterator object
	/// \param name          Instance name
	RelaxingIterator(const std::string& name);

	/// get current relaxation status
	/** A value of 0 means that only the initial stencils are used,
	 *  a value of 1 means that only the final stencils are used.
	 *  Values in between cause a weighted mean with weight <i>(1-cur)</i>
	 *  of the initial and weight <i>cur</i> of the final stencils.
	 *  \returns current relaxation status
	 */
	double getCur() const;
};

#endif // _RelaxingIterator_H_



