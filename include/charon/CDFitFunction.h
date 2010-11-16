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
/// \file CDFitFunction.h
/** Declaration of abstract class CDFitFunction.
 *  \author Jens-Malte Gottfried
 *  \date 16.09.2009
 */
#ifndef FIT_FUNCTION_H_
#define FIT_FUNCTION_H_

#include <charon-core/ParameteredObject.h>
#include <charon/Functions/Function.h>

#ifdef HANDLE_DLL
#ifdef cdfitfunction_EXPORTS
#define cdfitfunction_DECLDIR __declspec(dllexport)
#else
#define cdfitfunction_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
#define cdfitfunction_DECLDIR
#endif

#include <charon/Functions/Function.h>

/// Abstract function to be fitted to a probability density
class cdfitfunction_DECLDIR CDFitFunction : public ParameteredObject {
public:
	/// parameters to fit
	std::vector<double> fitParameters;

	/// self-pointer
	OutputSlot<CDFitFunction*> self;
	/// probability function
	OutputSlot<Function*> probability;
	/// energy function
	OutputSlot<Function*> energy;

	/// standard constructor.
	/** Passes arguments to ParameteredObject constructor.
	 *  \param className    class name
	 *  \param instanceName instance name
	 *  \param doc          documentation string
	 */
	CDFitFunction(const std::string& className,
			const std::string& instanceName,
			const std::string& doc);

	virtual ~CDFitFunction();

	/// normalize
	virtual void normalize();

	/// Range hint for histogram generation.
	/** Default implementation simply returns 1.
	 *  \param i    select dimension to query range hint for
	 */
	virtual double rangeHint(std::vector<double>::size_type i = 0) const;

	/// calculate log of the derivative wrt i-th fit-parameter
	/** \param x    evaluation location
	 *  \param i    derivation wrt i-th parameter
	 *  \returns \f$\frac{\partial}{\partial p_i}\log f(x)\f$
	 */
	virtual double diffLog(
			std::vector<double> x,
			std::vector<double>::size_type i) const = 0;
};

#endif // FIT_FUNCTION_H_
