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
/** \file AmplitudeThreshold.h
 *  Declaration of the class AmplitudeThreshold.
 *  \author Julian Coordts
 *  \date 13.01.2012
 */

#ifndef _AMPLITUDETHRESHOLD_H_
#define _AMPLITUDETHRESHOLD_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef amplitudethreshold_EXPORTS
/// Visual C++ specific code
#define amplitudethreshold_DECLDIR __declspec(dllexport)
#else
#define amplitudethreshold_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define amplitudethreshold_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// Cut off flow vectors whose amplitude are strict higher (or lower)
/// than a given threshold.
/** The Amplitude is defined as the square-root of the sum of all pixel values
 *  along the first Dimension of the input CImgList (usually 2 for classical
 *  optical flow fields).
 *
 *  This Plugin is mostly usefull for visualizing flow fields
 *  with some outliers.
 *
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup image-manipulators
 */
template <typename T>
class amplitudethreshold_DECLDIR AmplitudeThreshold :
		public TemplatedParameteredObject<T> {
public:
	/// input image
	InputSlot < cimg_library::CImgList<T> > _input;

	/// output image
	OutputSlot < cimg_library::CImgList<T> > _output;

	/// Parameters
	Parameter < double > _threshold;

	/// Parameters
	Parameter < bool > _lowpass ;

	/// Parameters
	Parameter < bool > _killOutlier;

	/// create a new AmplitudeThreshold object
	/// \param name          Instance name
	AmplitudeThreshold(const std::string& name);

protected:
	/// Update object.
	virtual void execute();
};

#endif // _AMPLITUDETHRESHOLD_H_
