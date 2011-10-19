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
/** \file Mirror.h
 *  Declaration of the parameter class Mirror.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 11.03.2011
 */

#ifndef MIRROR_H
#define MIRROR_H

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef mirror_EXPORTS
///Visual C++ specific code
#define mirror_DECLDIR __declspec(dllexport)
#else
#define mirror_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define mirror_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <vigra/multi_array.hxx>

/// mirror selected dimension(s)
/**
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup converters
 */
template <typename T>
class mirror_DECLDIR Mirror :
		public TemplatedParameteredObject<T>
{
public:
	/// data input
	InputSlot< vigra::MultiArrayView<5,T> >  in;
	/// data output
	OutputSlot< vigra::MultiArray<5,T> > out;
	/// \name select dimensions to mirror
	//  \{
	Parameter<bool> x,y,z,t,v;
	//  \}

	/// create a new Mirror object
	/// @param name             Object name
	Mirror(const std::string& name = "");

	/// \implements ParameteredObject::execute
	virtual void execute ();
};

#endif // MIRROR_H
