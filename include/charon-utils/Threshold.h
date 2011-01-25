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
/** \file Threshold.h
 *  Declaration of the parameter class Threshold.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 25.01.2011
 */

#ifndef THRESHOLD_H
#define THRESHOLD_H

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef threshold_EXPORTS
///Visual C++ specific code
#define threshold_DECLDIR __declspec(dllexport)
#else
#define threshold_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define threshold_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <vigra/multi_array.hxx>

/// simple class to threshold images
///
/// This class uses the vigra threshold command to threshold
/// images at the given boundaries. If image values are
/// between boundaries (i.e. within the closed interval [lower, heigher])
/// result is set to yesresult, otherwise to noresult.
template <typename T>
class threshold_DECLDIR Threshold : public TemplatedParameteredObject<T>
{
public:
	/// image data input slot
	InputSlot< vigra::MultiArrayView<5,T> >  in;
	/// image data output slot
	OutputSlot< vigra::MultiArray<5,T> > out;

	/// lower bound
	Parameter<T> lower;
	/// upper bound
	Parameter<T> higher;
	/// result if value is between bounds
	Parameter<T> yesresult;
	/// result if value is out of bounds
	Parameter<T> noresult;

	/// create a new Threshold object
	/// @param name             Object name
	Threshold(const std::string& name = "");

	/// \implements ParameteredObject::execute
	virtual void execute ();
};

#endif // THRESHOLD_H
