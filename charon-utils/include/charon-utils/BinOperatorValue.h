/*  Copyright (C) 2012 Gerald Mwangi

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
/** \file BinOperatorValue.h
 *  Declaration of the parameter class BinOperatorValue.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">Gerald Mwangi</a>
 *  \date 27.03.2012
 */

#ifndef _BINOPERATORVALUE_H_
#define _BINOPERATORVALUE_H_

#ifdef _MSC_VER
#ifdef binoperatorvalue_EXPORTS
/// Visual C++ specific code
#define binoperatorvalue_DECLDIR __declspec(dllexport)
#else
#define binoperatorvalue_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define binoperatorvalue_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>

/// Binary operators for numbers
/** \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup charon-helpers
 */
template <typename T>
class binoperatorvalue_DECLDIR BinOperatorValue :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	BinOperatorValue(const std::string& name = "");

	/// value1
	InputSlot< T > value1;
	/// value2
	InputSlot< T > value2;

	/// result
	OutputSlot< T > res;

	/// operation on the values
	Parameter< std::string > operation;

	/// Update object.
	virtual void execute();
};

#endif // _BINOPERATORVALUE_H_
