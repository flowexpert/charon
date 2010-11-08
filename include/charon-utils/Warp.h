/*  Copyright (C) 2009 Cornelius Ratsch

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
/** \file Warp.h
 *  Declaration of the parameter class Warp.
 *  \author Cornelius Ratsch
 *  \date 08.03.2010
 */

#ifndef _WARP_H_
#define _WARP_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef warp_EXPORTS
/// Visual C++ specific code
#define warp_DECLDIR __declspec(dllexport)
#else
#define warp_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define warp_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>
#include <charon-utils/Interpolator.h>

/// Warps an image
template <typename T>
class warp_DECLDIR Warp :
		public TemplatedParameteredObject<T> {
public:
	/// Image sequence 
	InputSlot < cimg_library::CImgList<T> > image_sequence;
	/// Image flow 
	InputSlot < cimg_library::CImgList<T> > flow_sequence;
	/// Interpolator 
	InputSlot < Interpolator<T> * > interpolator;
	/// Warped Image 
	OutputSlot < cimg_library::CImgList<T> > warped_image;
	/// Flow weight
	Parameter<double> weight;

	/// create a new Warp object
	/// \param name          Instance name
	Warp(const std::string& name = "");

	/// Update object.
	virtual void execute();
};

#endif // _WARP_H_



