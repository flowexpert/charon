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
/** \file ErodeDilate.h
 *  Declaration of the parameter class ErodeDilate.
 *  \author <a href="mailto:daniel@kondermann.de">
 *      Daniel Kondermann</a>
 *  \date 19.12.2011
 */

#ifndef _ERODEDILATE_H_
#define _ERODEDILATE_H_

#ifdef _MSC_VER
#ifdef erodedilate_EXPORTS
/// Visual C++ specific code
#define erodedilate_DECLDIR __declspec(dllexport)
#else
#define erodedilate_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define erodedilate_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

/// ErodeDilates a binary or grayvalue image
/** Wraps CImg-Function erode()
 */
template <typename T>
class erodedilate_DECLDIR ErodeDilate :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	ErodeDilate(const std::string& name = "");

	/// Input Image
	InputSlot< cimg_library::CImgList<T> > in;

	/// Result
	OutputSlot< cimg_library::CImgList<T> > out;

	/// ErodeDilate or Dilate
	Parameter< std::string > method;
	/// Block width of box-shaped structuring element.
	Parameter< unsigned int > size;

	/// Update object.
	virtual void execute();
};

#endif // _ERODEDILATE_H_
