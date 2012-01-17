/*  Copyright (C) 2011 Julian Coordts

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
/** \file GammaCorrection.h
 *  Declaration of the class GammaCorrection.
 *  \author Julian Coordts
 *  \date 13.01.2012
 */

#ifndef _GAMMACORRECTION_H_
#define _GAMMACORRECTION_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef gammacorrection_EXPORTS
/// Visual C++ specific code
#define gammacorrection_DECLDIR __declspec(dllexport)
#else
#define gammacorrection_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define gammacorrection_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// plugin does nothing
template <typename T>
class gammacorrection_DECLDIR GammaCorrection :
		public TemplatedParameteredObject<T> {
public:

	/// input image
	InputSlot < cimg_library::CImgList<T> > _input;

	/// output image
	OutputSlot < cimg_library::CImgList<T> > _output;
	
	/// Parameters
	Parameter < double > _gamma;
	/// the color value range of the input
	/** When set to zero, this will be set to the maximum of the input image */
	Parameter < double > _maxColorValue;
	

    /// create a new GammaCorrection object
	/// \param name          Instance name
    GammaCorrection(const std::string& name);

	/// Update object.
	virtual void execute();

};

#endif // _GAMMACORRECTION_H_
