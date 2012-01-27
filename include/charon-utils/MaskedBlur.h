/*  Copyright (C) 2011 Daniel Kondermann

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
/** \file MaskedBlur.h
 *  Declaration of the parameter class MaskedBlur.
 *  \author <a href="mailto:daniel@kondermann.de">
 *      Daniel Kondermann</a>
 *  \date 19.12.2011
 */

#ifndef _MASKEDBLUR_H_
#define _MASKEDBLUR_H_

#ifdef _MSC_VER
#ifdef maskedblur_EXPORTS
/// Visual C++ specific code
#define maskedblur_DECLDIR __declspec(dllexport)
#else
#define maskedblur_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define maskedblur_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

/// Applies a Gaussian blur with an individual sigma for each pixel
/** The provided mask is used to scale between a minimum and maximum sigma
 *  value which is used as blur strength.
 */
template <typename T>
class maskedblur_DECLDIR MaskedBlur :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	MaskedBlur(const std::string& name = "");

	/// Image to be blurred
	InputSlot< cimg_library::CImgList<T> > img;
	/// Mask (with same dimensions as img) with values between 0 and 1 used
	/// for blur strength
	InputSlot< cimg_library::CImgList<float> > mask;

	/// Resulting blurred image
	OutputSlot< cimg_library::CImgList<T> > out;

	/// Sigma to be used when mask value is zero
	Parameter< float > minSigma;
	/// Sigma to be used when mask value is 1 (linearly interpolated between
	/// minSigma and maxSigma)
	Parameter< float > maxSigma;
	/// When useOpacity is true, the image is blurred once with maxSigma
	/// and then blended according to the mask with the original
	Parameter< bool > useOpacity;
	/// When true, the mask is normalized per image list entry
	Parameter< bool > doNormalize;
	/// When true, the mask is inverted per image list entry (after normalization)
	Parameter< bool > doInvert;

	/// Update object.
	virtual void execute();
};

#endif // _MASKEDBLUR_H_
