/*  Copyright (C) 2012 Helen Morrison

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
/** \file LucasKanadeSmoothing.h
 *  Declaration of the parameter class LucasKanadeSmoothing.
 *  \author <a href="mailto:helen.morrison@iwr.uni-heidelberg.de">
 *		Helen Morrison</a>
 *  \date 16.04.2012
 */

#ifndef _LUCASKANADESMOOTHING_H_
#define _LUCASKANADESMOOTHING_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef lucas_kanade_smoothing_EXPORTS
/// Visual C++ specific code
#define lucas_kanade_smoothing_DECLDIR __declspec(dllexport)
#else
#define lucas_kanade_smoothing_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define lucas_kanade_smoothing_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// Calculate locally smoothed derivatives
/** This plugin serves as a preparation step for a Lucas & Kanade motion
 *  motion model (local constant) by locally smoothing the derivatives in
 *  advance
 *
 *  \ingroup charon-image-manipulators
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T>
class lucas_kanade_smoothing_DECLDIR LucasKanadeSmoothing :
		public TemplatedParameteredObject<T> {
public:
	/// linear 1D filter mask
	ParameterList < T > smoothMask;

	/// derivative wrt x 
	InputSlot < cimg_library::CImgList<T> > dx;
	/// derivative wrt y 
	InputSlot < cimg_library::CImgList<T> > dy;
	/// derivative wrt t 
	InputSlot < cimg_library::CImgList<T> > dt;

	/// <Ix * Ix>
	OutputSlot < cimg_library::CImgList<T> > IxIx;
	/// <Ix * Iy>
	OutputSlot < cimg_library::CImgList<T> > IxIy;
	/// <Iy * Iy>
	OutputSlot < cimg_library::CImgList<T> > IyIy;
	/// <Ix * It>
	OutputSlot < cimg_library::CImgList<T> > IxIt;
	/// <Iy * It>
	OutputSlot < cimg_library::CImgList<T> > IyIt;

	/// create a new LucasKanadeSmoothing object
	/// \param name          Instance name
	LucasKanadeSmoothing(const std::string& name = "");

	/// Update object.
	virtual void execute();
};

#endif // _LUCASKANADESMOOTHING_H_

