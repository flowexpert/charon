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
/** \file PyramidRescaleMatlab.h
 *  Declaration of the parameter class PyramidRescaleMatlab.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 02.08.2012
 */

#ifndef _PYRAMID_RESCALE_MATLAB_H_
#define _PYRAMID_RESCALE_MATLAB_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef pyramidrescalematlab_EXPORTS
/// Visual C++ specific code
#define pyramidrescalematlab_DECLDIR __declspec(dllexport)
#else
#define pyramidrescalematlab_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define pyramidrescalematlab_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>
#include <charon-utils/Roi.h>

/// MATLAB-like Rescaling for pyramid-based flow-estimation algorithms.
/**
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 *  \ingroup charon-image-manipulators
 */
template <typename T>
class pyramidrescalematlab_DECLDIR PyramidRescaleMatlab :
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

	/// create a new PyramidRescaleMatlab object
	/// \param name          Instance name
	PyramidRescaleMatlab(const std::string& name = "");

protected:
	/// Update object.
	virtual void execute();

private:
	/// content for roi pointer output slot
	Roi<int> _size;

	/// computes FilterMask
	cimg_library::CImg<T> _computeFilterMask( T smooth_sigma );

	/// normal distribution
	T _gauss( T x, T mu, T sigma );

	/// bilinear interpolation function
	T _bilinearInterpolation( cimg_library::CImg<T> data, T x, T y );
};

#endif // _PYRAMID_RESCALE_MATLAB_H_

