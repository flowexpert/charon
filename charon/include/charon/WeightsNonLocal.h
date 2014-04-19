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
/** \file WeightsNonLocal.h
 *  Declaration of the parameter class WeightsNonLocal.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 18.01.2012
 */

#ifndef WEIGHTSNONLOCAL_H
#define WEIGHTSNONLOCAL_H

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef weightsnonlocal_EXPORTS
/// Visual C++ specific code
#define weightsnonlocal_DECLDIR __declspec(dllexport)
#else
#define weightsnonlocal_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define weightsnonlocal_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// calculate non-local weights
/** This calculates non-local weights.
 *
 *  The non-local approach was introduced by D. Sun within his
 *  2010 CVPR paper "Secrets of optical flow".
 *
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T> class weightsnonlocal_DECLDIR WeightsNonLocal :
		public TemplatedParameteredObject<T>
{
public:
	/// image data input slot
	InputSlot<cimg_library::CImgList<T> > img;

	/// motion input slot
	InputSlot<cimg_library::CImgList<T> > motionUV;

	/// weights output slot
	OutputSlot<cimg_library::CImgList<T> > out;

	/// radius \f$r\f$ of image windows (size is \f$2*r+1\f$)
	Parameter<unsigned int> windowRadius;

	/// spatial difference weight
	Parameter< T > sigma_spatial;

	/// color difference weight
	Parameter< T > sigma_color;

	/// occlusion divergence weight
	Parameter< T > sigma_occ_divergence;

	/// occlusion color diff weight
	Parameter< T > sigma_occ_color;

	/// create a new WeightsNonLocal object
	/** \param name             Object name */
	WeightsNonLocal(const std::string& name = "");

	/// apply threshold to all given images
	/// \implements ParameteredObject::execute
	virtual void execute();

	/// gaussian distribution function
	inline T _gauss( T x, T mu, T sigma );
};

#endif // WEIGHTSNONLOCAL_H

