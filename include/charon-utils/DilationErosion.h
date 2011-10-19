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
/** \file DilationErosion.h
 *  Declaration of the parameter class DilationErosion.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 25.01.2011
 */

#ifndef DILATION_EROSION_H
#define DILATION_EROSION_H

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef dilationerosion_EXPORTS
///Visual C++ specific code
#define dilationerosion_DECLDIR __declspec(dllexport)
#else
#define dilationerosion_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define dilationerosion_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <vigra/multi_array.hxx>

/// morphology operation to dilate or erode
/**
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup image-manipulators
 */
template <typename T>
class dilationerosion_DECLDIR DilationErosion :
		public TemplatedParameteredObject<T>
{
public:
	/// data input
	InputSlot< vigra::MultiArrayView<5,T> >  in;
	/// data output
	OutputSlot< vigra::MultiArray<5,T> > out;

	/// dilation radius (pos: dilate, neg: erode)
	Parameter<int> radius;

	/// create a new DilationErosion object
	/// @param name             Object name
	DilationErosion(const std::string& name = "");

	/// \implements ParameteredObject::execute
	virtual void execute ();
};

#endif // DILATION_EROSION_H
