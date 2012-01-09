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
/** \file Residual.h
 *  Declaration of the parameter class Residual.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 09.01.2012
 */

#ifndef RESIDUAL_H
#define RESIDUAL_H

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef residual_EXPORTS
/// Visual C++ specific code
#define residual_DECLDIR __declspec(dllexport)
#else
#define residual_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define residual_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// calculates motion compensated intensity difference (residual)
/** Takes two images and both flow components and computes
 *  the residual (the motion compensated intensity difference).
 *
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T> class residual_DECLDIR Residual :
		public TemplatedParameteredObject<T>
{
public:
	/// image 1 data input slota
	InputSlot<cimg_library::CImgList<T> > img1;
	/// image 2 data input slota
	InputSlot<cimg_library::CImgList<T> > img2;

	/// horizontal motion component
	InputSlot<cimg_library::CImgList<T> > motU;
	/// vertical motion component
	InputSlot<cimg_library::CImgList<T> > motV;

	/// residual (output)
	OutputSlot<cimg_library::CImgList<T> > out;

	/// create a new Residual object
	/** \param name             Object name */
	Residual(const std::string& name = "");

	/// apply threshold to all given images
	/// \implements ParameteredObject::execute
	virtual void execute();
};

#endif // RESIDUAL_H

