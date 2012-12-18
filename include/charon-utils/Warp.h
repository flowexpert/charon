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
 *  \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried</a>
 *  \date 08.03.2010
 *
 *  \b Changes:
 *  - added option to warp symmetrically
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

/// Warping of images and image sequences
/** This module performs warping of single images or images sequences
 *  with a given flow. This is useful e.g. to check if a calculated optical
 *  flow explains the changes in a consecutive image pair.
 *
 *  Usually, the flow information has one time step less than the input
 *  sequence. In this case, the first image will stay untouched.
 *  A message is printed to sout showing how much images have been ignored.
 *  If there are more time steps in the flow image, the last ones are
 *  ignored (also with message printed to sout).
 *
 *  If the flow is correct, the two resulting images (-\> image pair)
 *  of this warping step should look much more similar than before.
 *  The difference between the two output images is called residual.
 *
 *  The flow is multiplied with the weight parameter before the warping is
 *  actually performed, this way warping into the opposite direction
 *  (weight = -1) or warping half-way (weight = 0.5) is possible.
 *
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup image-manipulators
 */
template <typename T>
class warp_DECLDIR Warp :
		public TemplatedParameteredObject<T> {
public:
	/// Image sequence 
	InputSlot < cimg_library::CImgList<T> > seqInput;
	/// Image flow 
	InputSlot < cimg_library::CImgList<T> > flowInput;
	/// Interpolator 
	InputSlot < Interpolator<T> * > interpolator;
	/// Warped Image 
	OutputSlot < cimg_library::CImgList<T> > out;
	/// Flow weight
	Parameter< float > weight;
	/// warp to NaN
	Parameter< bool > warpToNan;

	/// create a new Warp object
	/// \param name          Instance name
	Warp(const std::string& name = "");

protected:
	/// Update object.
	virtual void execute();
};

#endif // _WARP_H_



