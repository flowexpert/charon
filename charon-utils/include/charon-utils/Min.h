/*  Copyright (C) 2013 Moritz Becker

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
/** \file Min.h
 *  Declaration of the parameter class Min.
 *  \author <a href="mailto:moritz.becker@iwr.uni-heidelberg.de">
 *      Moritz Becker</a>
 *  \date 14.03.2013
 */

#ifndef _MIN_H_
#define _MIN_H_

#ifdef _MSC_VER
#ifdef min_EXPORTS
/// Visual C++ specific code
#define min_DECLDIR __declspec(dllexport)
#else
#define min_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define min_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

/// Returns piecewise minimums of two images
/** Returns piecewise minimums of two images
 */
template <typename T>
class min_DECLDIR Min :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	Min(const std::string& name = "");

	/// Image 1
	InputSlot< cimg_library::CImgList<T> > image1;
	/// Image 2
	InputSlot< cimg_library::CImgList<T> > image2;

	/// Minimum of both images
	OutputSlot< cimg_library::CImgList<T> > min;


protected:
	/// Update object.
	virtual void execute();
};

#endif // _MIN_H_
