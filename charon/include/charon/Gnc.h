/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

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
/** \file Gnc.h
 *  Declaration of the parameter class Gnc.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 05.04.2012
 */

#ifndef _GNC_H
#define _GNC_H

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef gnc_EXPORTS
///Visual C++ specific code
#define gnc_DECLDIR __declspec(dllexport)
#else
#define gnc_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define gnc_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// simple class to apply graduated non convexity scheme
/** Simple class to apply graduated non convexity scheme.
 */
template <typename T>
class gnc_DECLDIR Gnc : public TemplatedParameteredObject<T>
{
public:
	/// input for data, which is faded out
	InputSlot< cimg_library::CImgList<T> >  initial;
	/// input for data, which is faded in
	InputSlot< cimg_library::CImgList<T> >  final;

	/// gnc iteration step
	InputSlot< unsigned int > step;

	/// output
	OutputSlot< cimg_library::CImgList<T> > out;

	/// ratio (output = final * step * ratio
	///               + initial * (1 - step * ratio))
	Parameter< T > ratio;

	/// create a new Gnc object
	/// @param name             Object name
	Gnc(const std::string& name = "");

	/// \implements ParameteredObject::execute
	virtual void execute ();
};

#endif // _GNC_H

