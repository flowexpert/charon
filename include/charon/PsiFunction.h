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
/** \file PsiFunction.h
 *  Declaration of the parameter class PsiFunction.
 *  \author Jens-Malte Gottfried <jmgottfried@web.de>
 *  \date 27.04.2010
 */

#ifndef PSIFUNCTION_H
#define PSIFUNCTION_H

#ifdef HANDLE_DLL
#ifdef psifunction_EXPORTS
#define psifunction_DECLDIR __declspec(dllexport)
#else
#define psifunction_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
#define psifunction_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon/Functions/Diff2Function.h>
#include <charon/Functions/Diff2LinFunction.h>

/// function implementing psi energy term
/** which looks as follows:
 *  \f[\Psi(x)=\sqrt{x^2+\varepsilon^2}\f]
 *
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 *  \ingroup charon-stencil-helpers
 */
class psifunction_DECLDIR PsiFunction :
		public ParameteredObject,
		public Diff2Function,
		public Diff2LinFunction {
public:
	/// default constructor
	PsiFunction(const std::string& name /** [in] instance name */);

	/// parameter epsilon
	Parameter<double> epsilon;
	/// rescaling
	Parameter<double> lambda;

	/// self pointer
	OutputSlot<Function*> self;

	virtual std::vector<double>::size_type dims() const{
		return 1u;
	}
	virtual double calculate(
			std::vector<double> x) const;
	virtual double diff(
		std::vector<double>x,
		std::vector<double>::size_type i = 0) const;
	virtual double diff2(
			std::vector<double>x,
			std::vector<double>::size_type i = 0,
			std::vector<double>::size_type j = 0) const;
	virtual double diff2Linearized(
			std::vector<double>x,
			std::vector<double>::size_type i = 0,
			std::vector<double>::size_type j = 0) const;

	/// execute object
	/** Precalculate _ep2
	 *
	 *  \copydetails ParameteredObject::execute()
	 */
	virtual void execute();

private:
	double _ep2;    ///< precalculated \f$\varepsilon^2\f$
	double _lambda; ///< saved lambda
};

#endif // PSIFUNCTION_H
