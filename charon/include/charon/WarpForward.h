/*  Copyright (C) 2013
                  University of Heidelberg (IWR/HCI)

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
/** \file WarpForward.h
 *  Declaration of the parameter class WarpForward.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 10.04.2013
 */

#ifndef _WARPFORWARD_H_
#define _WARPFORWARD_H_

#ifdef _MSC_VER
#ifdef warpforward_EXPORTS
/// Visual C++ specific code
#define warpforward_DECLDIR __declspec(dllexport)
#else
#define warpforward_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define warpforward_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

/// Module for forward warping
/** Module for forward warping.
 */
template <typename T>
class warpforward_DECLDIR WarpForward :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	WarpForward(const std::string& name = "");

	/// data input
	InputSlot< cimg_library::CImgList<T> > data;
	/// motion input
	InputSlot< cimg_library::CImgList<T> > motion;

	/// output
	OutputSlot< cimg_library::CImgList<T> > out;


protected:
	/// Update object.
	virtual void execute();
};

#endif // _WARPFORWARD_H_

