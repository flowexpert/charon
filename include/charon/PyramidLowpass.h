/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

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
/** \file PyramidLowpass.h
 *  Declaration of the parameter class PyramidLowpass.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 26.07.2012
 */

#ifndef _PYRAMID_LOWPASS_H_
#define _PYRAMID_LOWPASS_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef pyramidlowpass_EXPORTS
/// Visual C++ specific code
#define pyramidlowpass_DECLDIR __declspec(dllexport)
#else
#define pyramidlowpass_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define pyramidlowpass_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// Lowpass filtering for pyramid-based flow-estimation algorithms.
/**
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 *  \ingroup charon-image-manipulators
 */
template <typename T>
class pyramidlowpass_DECLDIR PyramidLowpass :
		public TemplatedParameteredObject<T> {
public:
	/// sequence input
	InputSlot < cimg_library::CImgList<T> > seqIn;
	/// mask for bilateral blur (optional)
	InputSlot < cimg_library::CImgList<T> > blurMask;
	/// level select (from small to larger blur factors)
	InputSlot < unsigned int > level;

	/// sequence output
	OutputSlot < cimg_library::CImgList<T> > seqOut;

	/// blur levels
	ParameterList < T > sigmas;

	/// bilateral blur, if checked
	Parameter< bool > bilateralBlur;

	/// radius for bilateral blur window
	Parameter< unsigned int > blurRadius;

	/// create a new PyramidLowpass object
	/// \param name          Instance name
	PyramidLowpass(const std::string& name = "");

protected:
	/// Update object.
	virtual void execute();

	/// bi-lateral blur function
	cimg_library::CImg<T> _blur( int kk, T sigma, int radius );

	/// normal distribution function
	inline T _gauss( T x, T mu, T sigma );
};

#endif // _PYRAMID_LOWPASS_H_

