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
/** \file ValueBuffer.h
 *  Declaration of the parameter class ValueBuffer.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 27.03.2012
 */

#ifndef _VALUEBUFFER_H_
#define _VALUEBUFFER_H_

#ifdef _MSC_VER
#ifdef valuebuffer_EXPORTS
/// Visual C++ specific code
#define valuebuffer_DECLDIR __declspec(dllexport)
#else
#define valuebuffer_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define valuebuffer_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>

/// Buffers a value for use in Whilegroups
/** Buffers a value for use in Whilegroups
 */
template <typename T>
class valuebuffer_DECLDIR ValueBuffer :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	ValueBuffer(const std::string& name = "");

	/// value to be buffered
	InputSlot< T > value;

	/// buffered value
	OutputSlot< T > buffered_value;

	/// Update object.
	virtual void execute();
};

#endif // _VALUEBUFFER_H_
