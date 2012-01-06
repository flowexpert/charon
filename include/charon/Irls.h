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
/** \file Irls.h
 *  Declaration of the parameter class Irls.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 06.01.2012
 */

#ifndef IRLS_H
#define IRLS_H

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef irls_EXPORTS
/// Visual C++ specific code
#define irls_DECLDIR __declspec(dllexport)
#else
#define irls_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define irls_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// calculate median of image windows
/** Use image windows of given size, calculate median within this window
 *  and use the result as new value for the window center pixel.
 *  This eliminates outliers and makes e.g. flow estimation more robust.
 *  Median calculation is being performed using the
 *  "Iteratively reweighted least squares" approach
 *  (e.g. "Numerical Methods in Scientific Computing Vol. II"
 *   by Germund Dahlquist and Åke Björck, SIAM, section 8.7.5 )
 *
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 *  \ingroup image-manipulators
 */
template <typename T> class irls_DECLDIR Irls :
		public TemplatedParameteredObject<T>
{
public:
	/// image data input slot
	InputSlot<cimg_library::CImgList<T> > in;
	/// image data output slot
	OutputSlot<cimg_library::CImgList<T> > out;

	/// radius \f$r\f$ of image windows (size is \f$2*r+1\f$)
	Parameter<unsigned int> windowRadius;

	/// iteration count
	Parameter<unsigned int> iterations;

	/// create a new Irls object
	/** \param name             Object name */
	Irls(const std::string& name = "");

	/// apply threshold to all given images
	/// \implements ParameteredObject::execute
	virtual void execute();
};

#endif // IRLS_H

