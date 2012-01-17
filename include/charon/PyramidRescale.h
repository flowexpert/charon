/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file PyramidRescale.h
 *  Declaration of the parameter class PyramidRescale.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 11.02.2011
 */

#ifndef _PYRAMID_RESCALE_H_
#define _PYRAMID_RESCALE_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef pyramidrescale_EXPORTS
/// Visual C++ specific code
#define pyramidrescale_DECLDIR __declspec(dllexport)
#else
#define pyramidrescale_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define pyramidrescale_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>
#include <charon-utils/Roi.h>

/// Rescaling for pyramid-based flow-estimation algorithms.
/**
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 *  \ingroup charon-image-manipulators
 */
template <typename T>
class pyramidrescale_DECLDIR PyramidRescale :
		public TemplatedParameteredObject<T> {
public:
	/// sequence input
	InputSlot < cimg_library::CImgList<T> > seqIn;
	/// flow input
	InputSlot < cimg_library::CImgList<T> > flowIn;
	/// level select (from small to larger scales)
	InputSlot < unsigned int > level;

	/// sequence output
	OutputSlot < cimg_library::CImgList<T> > seqOut;
	/// flow output
	OutputSlot < cimg_library::CImgList<T> > flowOut;
	/// current size
	OutputSlot < Roi<int>* > size;

	/// scale factor
	Parameter < double > scaleFactor;
	/// scale levels
	Parameter < unsigned int > levels;
	/// sigma used to blur before downsampling
	Parameter < double > sigma;
	/// interpolation type (see CImg::resize() documentation)
	Parameter < int > interpolation;

	/// create a new PyramidRescale object
	/// \param name          Instance name
	PyramidRescale(const std::string& name = "");

protected:
	/// Update object.
	virtual void execute();

private:
	/// content for roi pointer output slot
	Roi<int> _size;
};

#endif // _PYRAMID_RESCALE_H_

