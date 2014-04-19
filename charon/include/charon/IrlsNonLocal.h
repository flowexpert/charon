/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

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
/** \file IrlsNonLocal.h
 *  Declaration of the parameter class IrlsNonLocal.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 10.01.2012
 */

#ifndef IRLSNONLOCAL_H
#define IRLSNONLOCAL_H

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef irlsnonlocal_EXPORTS
/// Visual C++ specific code
#define irlsnonlocal_DECLDIR __declspec(dllexport)
#else
#define irlsnonlocal_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define irlsnonlocal_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// calculate non-local median of image windows
/** Use image windows of given size, calculate non-local
 *  median within this window
 *  and use the result as new value for the window center pixel.
 *  This eliminates outliers and makes e.g. flow estimation more robust.
 *  Median calculation is being performed using the
 *  "Iteratively reweighted least squares" approach
 *  (e.g. "Numerical Methods in Scientific Computing Vol. II"
 *   by Germund Dahlquist and Åke Björck, SIAM, section 8.7.5 )
 *
 *  The non-local approach was introduced by D. Sun within his
 *  2010 CVPR paper "Secrets of optical flow".
 *
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T> class irlsnonlocal_DECLDIR IrlsNonLocal :
		public TemplatedParameteredObject<T>
{
public:
	/// image data input slot
	InputSlot<cimg_library::CImgList<T> > img;

	/// weight input slot
	InputSlot<cimg_library::CImgList<T> > motionUV;

	/// image data output slot
	OutputSlot<cimg_library::CImgList<T> > out;

	/// radius \f$r\f$ of image windows (size is \f$2*r+1\f$)
	Parameter<unsigned int> windowRadius;

	/// iteration count
	Parameter<unsigned int> iterations;

	/// spatial difference weight
	Parameter< T > sigma_spatial;

	/// color difference weight
	Parameter< T > sigma_color;

	/// occlusion divergence weight
	Parameter< T > sigma_occ_divergence;

	/// occlusion color diff weight
	Parameter< T > sigma_occ_color;

	/// create a new IrlsNonLocal object
	/** \param name             Object name */
	IrlsNonLocal(const std::string& name = "");

	/// apply threshold to all given images
	/// \implements ParameteredObject::execute
	virtual void execute();

	///gaussian distribution function
	inline T _gauss( T x, T mu, T sigma );
};

#endif // IRLSNONLOCAL_H

