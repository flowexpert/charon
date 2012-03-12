/*  Copyright (C) 2012 Daniel Kondermann

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
/** \file StrictResizeXY.h
 *  Declaration of the parameter class StrictResizeXY.
 *  \author <a href="mailto:daniel@kondermann.de">
 *      Daniel Kondermann</a>
 *  \date 02.01.2012
 */

#ifndef _STRICTRESIZEXY_H_
#define _STRICTRESIZEXY_H_

#ifdef _MSC_VER
#ifdef strictresizexy_EXPORTS
/// Visual C++ specific code
#define strictresizexy_DECLDIR __declspec(dllexport)
#else
#define strictresizexy_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define strictresizexy_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

/// Reduces the size of each frame by a given factor by averaging exactly
/// each n x n pixels
/** This method is used to create no correlation in noise, for example to
 *  downsample reference data.
 */
template <typename T>
class strictresizexy_DECLDIR StrictResizeXY :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	StrictResizeXY(const std::string& name = "");

	/// Input image sequence (all dimensions handled correctly)
	InputSlot< cimg_library::CImgList<T> > in;

	/// Same sequence but halfed size in XY-dimensions
	OutputSlot< cimg_library::CImgList<T> > out;

	/// resize factor
	Parameter< int > factor ;

	/// Update object.
	virtual void execute();
};

#endif // _STRICTRESIZEXY_H_
