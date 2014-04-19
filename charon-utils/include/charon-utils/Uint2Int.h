/*  Copyright (C) 2014 Jens-Malte Gottfried

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
/** \file Uint2Int.h
 *  \author <a href="mailto:jmgottfried@web.de">
 *      Jens-Malte Gottfried</a>
 *  \date 16.01.2014
 *  Declaraion of parameter class Uint2Int.
 */
#ifndef _UINT2INT_H_
#define _UINT2INT_H_

#ifdef _MSC_VER
#ifdef uint2int_EXPORTS
/// Visual C++ specific code
#define uint2int_DECLDIR __declspec(dllexport)
#else
#define uint2int_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define uint2int_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>

/// Slot Converter
/** converts from uint to int
 *
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup charon-helpers
 */
class uint2int_DECLDIR Uint2Int : public ParameteredObject {
public:
	/// default constructor
	/// \param name             instance name
	Uint2Int(const std::string& name = "");

	/// uint input
	InputSlot< unsigned int > in;

	/// int output
	OutputSlot< int > out;


protected:
	/// Update object.
	virtual void execute();
};

#endif /* _UINT2INT_H_ */
