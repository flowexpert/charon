/*  Copyright (C) 2013
                  University of Heidelberg (IWR/HCI)

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
/** \file SimpleWarp.h
 *  Declaration of the parameter class SimpleWarp.
 *  \author Michael Baron
 *  \date 14.02.2013
 */

#ifndef _SIMPLEWARP_H_
#define _SIMPLEWARP_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef simplewarp_EXPORTS
/// Visual C++ specific code
#define simplewarp_DECLDIR __declspec(dllexport)
#else
#define simplewarp_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define simplewarp_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>
#include <charon-utils/Interpolator.h>

/// Simple Warping of images and image sequences
/** This module performs simple warping of image sequences.
 *
 *  \ingroup charon-modules
 */
template <typename T>
class simplewarp_DECLDIR SimpleWarp :
		public TemplatedParameteredObject<T> {
public:
	/// Image sequence 
	InputSlot < cimg_library::CImgList<T> > seqInput;
	/// Image flow 
	InputSlot < cimg_library::CImgList<T> > flowInput;
	/// Interpolator 
	InputSlot < Interpolator<T> * > interpolator;
	/// SimpleWarped Image 
	OutputSlot < cimg_library::CImgList<T> > out;
	/// mask (masked, if warp within bounds)
	OutputSlot < cimg_library::CImgList<T> > warpWithinBounds;

	/// create a new SimpleWarp object
	/// \param name          Instance name
	SimpleWarp(const std::string& name = "");

protected:
	/// Update object.
	virtual void execute();
};

#endif // _SIMPLEWARP_H_

