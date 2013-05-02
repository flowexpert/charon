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
/** \file DiscretizeByThreshold.h
 *  Declaration of the parameter class DiscretizeByThreshold.
 *  \author <a href="mailto:moritz.becker@iwr.uni-heidelberg.de">
 *      Moritz Becker</a>
 *  \date 17.03.2013
 */

#ifndef _DISCRETIZEBYTHRESHOLD_H_
#define _DISCRETIZEBYTHRESHOLD_H_

#ifdef _MSC_VER
#ifdef discretizebythreshold_EXPORTS
/// Visual C++ specific code
#define discretizebythreshold_DECLDIR __declspec(dllexport)
#else
#define discretizebythreshold_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define discretizebythreshold_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

/// Discretizes an image by a given threshold
/** Discretizes an image by a given threshold. All pixels >= threshold will be
 *  set to 1.
 */
template <typename T>
class discretizebythreshold_DECLDIR DiscretizeByThreshold :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	DiscretizeByThreshold(const std::string& name = "");

	/// Images to discretize
	InputSlot< cimg_library::CImgList<T> > images;

	/// Discretized images
	OutputSlot< cimg_library::CImgList<T> > imagesOut;

	/// Threshold for pixel values
	Parameter< float > threshold;


protected:
	/// Update object.
	virtual void execute();
};

#endif // _DISCRETIZEBYTHRESHOLD_H_
