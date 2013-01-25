/*  Copyright (C) 2013
                  University of Heidelberg (IWR/HCI)

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
/** \file PyramidRescaleMask.h
 *  Declaration of the parameter class PyramidRescaleMask.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 21.01.2013
 */

#ifndef _PYRAMID_RESCALE_MASK_H_
#define _PYRAMID_RESCALE_MASK_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef pyramidrescalemask_EXPORTS
/// Visual C++ specific code
#define pyramidrescalemask_DECLDIR __declspec(dllexport)
#else
#define pyramidrescalemask_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define pyramidrescalemask_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// Pyramid rescaling of binary masks for multi-scale motion estimation.
/**
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 *  \ingroup charon-image-manipulators
 */
template <typename T>
class pyramidrescalemask_DECLDIR PyramidRescaleMask :
		public TemplatedParameteredObject<T> {
public:
	/// mask input
	InputSlot < cimg_library::CImgList<T> > mask;
	/// level select (from small to larger scales)
	InputSlot < unsigned int > level;
	/// current mask output
	OutputSlot < cimg_library::CImgList<T> > currentMask;
	/// previous mask output
	OutputSlot < cimg_library::CImgList<T> > previousMask;

	/// scale factor
	Parameter < double > scaleFactor;
	/// scale levels
	Parameter < unsigned int > levels;

	/// create a new PyramidRescaleMask object
	/// \param name          Instance name
	PyramidRescaleMask(const std::string& name = "");

protected:
	/// Update object.
	virtual void execute();
};

#endif // _PYRAMID_RESCALE_MASK_H_

