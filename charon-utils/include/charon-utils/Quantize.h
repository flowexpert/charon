/*  Copyright (C) 2013 Moritz Becker

	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file Quantize.h
 *  Declaration of the parameter class Quantize.
 *  \author <a href="mailto:moritz.becker@iwr.uni-heidelberg.de">
 *      Moritz Becker</a>
 *  \date 17.03.2013
 */

#ifndef _QUANTIZE_H_
#define _QUANTIZE_H_

#ifdef _MSC_VER
#ifdef quantize_EXPORTS
/// Visual C++ specific code
#define quantize_DECLDIR __declspec(dllexport)
#else
#define quantize_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define quantize_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

/// Applies quantize function of CImg library
/** Applies quantize function of CImg library
 */
template <typename T>
class quantize_DECLDIR Quantize :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	Quantize(const std::string& name = "");

	/// Input images
	InputSlot< cimg_library::CImgList<T> > image;

	/// Output images
	OutputSlot< cimg_library::CImgList<T> > imagesOut;

	/// Number of quantize levels
	Parameter< int > levels;


protected:
	/// Update object.
	virtual void execute();
};

#endif // _QUANTIZE_H_
