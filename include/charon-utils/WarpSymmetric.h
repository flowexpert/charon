/*  Copyright (C) 2010 Jens-Malte Gottfried

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
 *  Declaration of the parameter class WarpSymmetric.
 *  \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried</a>
 *  \date 12.11.2010
 */

#ifndef _WARP_SYMMETRIC_H_
#define _WARP_SYMMETRIC_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef warpsymmetric_EXPORTS
/// Visual C++ specific code
#define warpsymmetric_DECLDIR __declspec(dllexport)
#else
#define warpsymmetric_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define warpsymmetric_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>
#include <charon-utils/Interpolator.h>

/// Symmetric warping of images pairs
/** This module performs warping of two consecutive images (image pair),
 *  and works similarly as the module Warp. The difference is
 *  that warping is done in a symmetric way, i.e. the first image is
 *  warped towards the second one (using half the negative flow),
 *  the second one is warped towards the first one (using half the flow).
 *  This avoids artifacts resulting when compared the warped,
 *  second image with the original one,
 *  as done usually e.g. using the module Warp.
 *
 *  Since the negative flow is not equal to the inverted flow, the inversion
 *  is approximated by warping the flow with itself. To get symmetric
 *  processing for both images, the flow is also warped half way
 *  (forward for the first image and backward for the second one).
 *
 *  If the flow is correct, the two resulting images (-\> image pair)
 *  of this warping step should look much more similar than before.
 *  The difference between the two output images is called residual.
 *
 *  The flow is multiplied with the weight parameter before the warping is
 *  actually performed, this way warping into the opposite direction
 *  (weight = -1) or warping half-way (weight = 0.5) is possible.
 */
template <typename T>
class warpsymmetric_DECLDIR WarpSymmetric :
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

	/// create a new Warp object
	/// \param name          Instance name
	WarpSymmetric(const std::string& name = "");

	/// Update object.
	virtual void execute();
};

#endif // _WARP_SYMMETRIC_H_



