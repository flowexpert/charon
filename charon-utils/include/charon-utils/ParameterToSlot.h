/*  Copyright (C) 2013 Gerald Mwangi

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
/** \file ParameterToSlot.h
 *  Declaration of the parameter class ParameterToSlot.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 20.09.2013
 */

#ifndef _PARAMETERTOSLOT_H_
#define _PARAMETERTOSLOT_H_

#ifdef _MSC_VER
#ifdef parametertoslot_EXPORTS
/// Visual C++ specific code
#define parametertoslot_DECLDIR __declspec(dllexport)
#else
#define parametertoslot_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define parametertoslot_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>

/// Copies a parameter to a slot
/** Copies a parameter to a slot
 */
template <typename T>
class parametertoslot_DECLDIR ParameterToSlot :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	ParameterToSlot(const std::string& name = "");

	/// value from parameter
	OutputSlot< T > value;

	/// param_val to be copied to slot
	Parameter< T > param_val;


protected:
	/// Update object.
	virtual void execute();
};

#endif // _PARAMETERTOSLOT_H_
