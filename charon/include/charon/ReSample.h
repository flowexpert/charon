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
/** \file ReSample.h
 *  Declaration of the parameter class ReSample.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 02.09.2013
 */

#ifndef _RESAMPLE_H_
#define _RESAMPLE_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef resample_EXPORTS
/// Visual C++ specific code
#define resample_DECLDIR __declspec(dllexport)
#else
#define resample_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define resample_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>
#include <charon-utils/Interpolator.h>
#include <charon-utils/Roi.h>

/// Pyramidal resampling of data.
/**
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T>
class resample_DECLDIR ReSample :
		public TemplatedParameteredObject<T> {
public:
	/// data input
	InputSlot < cimg_library::CImgList<T> > in;
	/// level select (from small to larger scales)
	InputSlot < unsigned int > level;
	/// interpolator used to resample
	InputSlot < Interpolator<T>* > interpolator;

	/// sequence output
	OutputSlot < cimg_library::CImgList<T> > out;
	/// current size
	OutputSlot < Roi<int>* > size;

	/// scale factor
	Parameter < double > scaleFactor;
	/// scale data, if set
	Parameter < bool > scaleData;
	/// scale levels
	Parameter < unsigned int > levels;
	/// level offset
	Parameter < int > levelOffset;

	/// create a new ReSample object
	/// \param name          Instance name
	ReSample(const std::string& name = "");

protected:
	/// Update object.
	virtual void execute();

private:
	/// content for roi pointer output slot
	Roi<int> _size;
};

#endif // _RESAMPLE_H_

