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
/** \file PrintValue.h
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 22.02.2012
 *  Declaraion of parameter class PrintValue.
 */
#ifndef _PRINTVALUE_H_
#define _PRINTVALUE_H_

#ifdef _MSC_VER
#ifdef printvalue_EXPORTS
/// Visual C++ specific code
#define printvalue_DECLDIR __declspec(dllexport)
#else
#define printvalue_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define printvalue_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>

/// Prints a value to stdout
/** Prints a value to stdout
 */
class printvalue_DECLDIR PrintValue : public ParameteredObject {
public:
	/// default constructor
	/// \param name             instance name
	PrintValue(const std::string& name = "");

	/// value
	InputSlot< double > value;

	/// Update object.
	virtual void execute();
};

#endif /* _PRINTVALUE_H_ */
