/*  Copyright (C) 2012 Gerald Mwangi

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
/** \file GreaterSmallerThenPar.h
 *  Declaration of the parameter class GreaterSmallerThenPar.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 17.02.2012
 */

#ifndef _GREATERSMALLERTHENPAR_H_
#define _GREATERSMALLERTHENPAR_H_

#ifdef _MSC_VER
#ifdef greatersmallerthenpar_EXPORTS
/// Visual C++ specific code
#define greatersmallerthenpar_DECLDIR __declspec(dllexport)
#else
#define greatersmallerthenpar_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define greatersmallerthenpar_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>

/// Compares the input value to a threshold
/** Compares the input value to a threshold
 */
template <typename T>
class greatersmallerthenpar_DECLDIR GreaterSmallerThenPar :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	GreaterSmallerThenPar(const std::string& name = "");

	/// value
	InputSlot< T > value;

	/// result
	OutputSlot< bool > result;

	/// value to be compared against
	Parameter< T > threshold;
	/// val>threshold
	Parameter< bool > greater;
	/// val<threshold
	Parameter< bool > smaller;
	/// val=threshold. Compatible with parameters greater and smaller
	Parameter< bool > equal;

	/// Update object.
	virtual void execute();
};

#endif // _GREATERSMALLERTHENPAR_H_
