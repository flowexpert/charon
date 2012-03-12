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
/** \file FitSizes.h
 *  Declaration of the parameter class FitSizes.
 *  \author <a href="mailto:daniel@kondermann.de">
 *      Daniel Kondermann</a>
 *  \date 05.08.2011
 */

#ifndef _FITSIZES_H_
#define _FITSIZES_H_

#ifdef _MSC_VER
#ifdef fitsizes_EXPORTS
/// Visual C++ specific code
#define fitsizes_DECLDIR __declspec(dllexport)
#else
#define fitsizes_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define fitsizes_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

/// Takes two images and scales the second image to fit the size of the first
/** Uses CImg internal interpolation methods.
 */
template <typename T>
class fitsizes_DECLDIR FitSizes :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	FitSizes(const std::string& name = "");

	/// This image remains untouched. Only its size (x,y,z) is used as target
	/// size.
	InputSlot< cimg_library::CImgList<T> > target;
	/// This image will be scaled to fit the target size.
	InputSlot< cimg_library::CImgList<T> > toScale;

	/// The resized image.
	OutputSlot< cimg_library::CImgList<T> > out;

	/// The way the image is interpolated.
	Parameter< std::string > interpolationMethod;
	/// The image is scaled so that one side becomes too large. This part is
	/// cropped.
	Parameter< bool > crop;

	/// Update object.
	virtual void execute();
};

#endif // _FITSIZES_H_
