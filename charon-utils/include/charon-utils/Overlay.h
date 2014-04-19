/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file Overlay.h
 *  Declaration of the parameter class Overlay.
 *  \author <a href="mailto:jmgottfried@web.de">
 *      Jens-Malte Gottfried</a>
 *  \date 16.12.2011
 */

#ifndef _OVERLAY_H_
#define _OVERLAY_H_

#ifdef _MSC_VER
#ifdef overlay_EXPORTS
/// Visual C++ specific code
#define overlay_DECLDIR __declspec(dllexport)
#else
#define overlay_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define overlay_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

/// draw one image above another
/** One image is used as foreground, a second image as backround. Using the
 *  alpha parameter, the weight of each image may be adjusted.
 */
template <typename T>
class overlay_DECLDIR Overlay :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	Overlay(const std::string& name = "");

	/// foreground image
	InputSlot< cimg_library::CImgList<T> > foreground;
	/// background image
	InputSlot< cimg_library::CImgList<T> > background;

	/// overlay output
	OutputSlot< cimg_library::CImgList<T> > out;

	/// foreground/background ratio, 0=back,1=front
	Parameter< float > alpha;

	/// black is treated as transparent, if set
	Parameter< bool > blackIsTransparent;

protected:
	/// Update object.
	virtual void execute();
};

#endif // _OVERLAY_H_
