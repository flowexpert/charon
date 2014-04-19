/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

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
/** \file Value.h
 *  Declaration of the parameter class Value;
 *  \author Michael Baron
 *  \date 06.08.2012
 */

#ifndef _VALUE_H_
#define _VALUE_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef value_EXPORTS
/// Visual C++ specific code
#define value_DECLDIR __declspec(dllexport)
#else
#define value_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define value_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>

/// Module returning a value.
/** This module returns a value.
 *
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T>
class value_DECLDIR Value :
		public TemplatedParameteredObject<T> {
public:
	/// output value
	OutputSlot < unsigned int > out;

	/// parameter value
	Parameter< unsigned int > value;

	/// default constructor
	Value(const std::string& name = "" /** [in] Instance name*/);

protected:
	/// Update object.
	virtual void execute();
};

#endif // _VALUE_H_

