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
/** \file Int2Uint.h
 *  \author <a href="mailto:jmgottfried@web.de">
 *      Jens-Malte Gottfried</a>
 *  \date 16.01.2014
 *  Declaraion of parameter class Int2Uint.
 */
#ifndef _INT2UINT_H_
#define _INT2UINT_H_

#ifdef _MSC_VER
#ifdef int2uint_EXPORTS
/// Visual C++ specific code
#define int2uint_DECLDIR __declspec(dllexport)
#else
#define int2uint_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define int2uint_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>

/// Slot Converter
/** converts slots from Int to Uint
 *
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup charon-helpers
 */
class int2uint_DECLDIR Int2Uint : public ParameteredObject {
public:
	/// default constructor
	/// \param name             instance name
	Int2Uint(const std::string& name = "");

	/// int input
	InputSlot< int > in;

	/// uint output
	OutputSlot< unsigned int > out;


protected:
	/// Update object.
	virtual void execute();
};

#endif /* _INT2UINT_H_ */
