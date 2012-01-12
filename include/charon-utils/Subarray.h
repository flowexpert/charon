/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file Subarray.h
 *  Declaration of the parameter class Subarray.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 25.01.2011
 */

#ifndef SUBARRAY_H
#define SUBARRAY_H

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef subarray_EXPORTS
///Visual C++ specific code
#define subarray_DECLDIR __declspec(dllexport)
#else
#define subarray_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define subarray_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <vigra/multi_array.hxx>
#include <charon-utils/Roi.h>

/// vigra implementation of crop
/**
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup image-manipulators
 */
template <typename T>
class subarray_DECLDIR Subarray :
		public TemplatedParameteredObject<T>
{
public:
	/// data input
	InputSlot< vigra::MultiArray<5, T> >  in;
	/// crop region
	InputSlot< Roi<int>* > roi;
	/// data output
	OutputSlot< vigra::MultiArray<5,T> > out;
	/// \name select dimensions to crop
	//  \{
	Parameter<bool> x,y,z,t,v;
	//  \}

	/// create a new Subarray object
	/// @param name             Object name
	Subarray(const std::string& name = "");

protected:
	/// \implements ParameteredObject::execute
	virtual void execute ();
};

#endif // SUBARRAY_H
