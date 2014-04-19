/*      Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

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
/** \file LayeredMedianFilter.h
 *  Declaration of the parameter class LayeredMedianFilter.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 01.02.2012
 */

#ifndef LAYEREDMEDIANFILTER_H
#define LAYEREDMEDIANFILTER_H

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef layeredmedianfilter_EXPORTS
/// Visual C++ specific code
#define layeredmedianfilter_DECLDIR __declspec(dllexport)
#else
#define layeredmedianfilter_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define layeredmedianfilter_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// calculate median of image windows
/** Use image windows of given size, calculate median within this window
 *  and use the result as new value for the window center pixel.
 *  This eliminates outliers and makes e.g. flow estimation more robust.
 *
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup image-manipulators
 */
template <typename T> class layeredmedianfilter_DECLDIR LayeredMedianFilter :
		public TemplatedParameteredObject<T>
{
public:
	/// image data input slot
	InputSlot<cimg_library::CImgList<T> > in;
	/// current layer
	InputSlot<unsigned int> layer;
	/// image data output slot
	OutputSlot<cimg_library::CImgList<T> > out;

	/// radius \f$r\f$ of image windows (size is \f$2*r+1\f$)
	ParameterList<unsigned int> windowRadiusList;

	/// create a new LayeredMedianFilter object
	/** \param name             Object name */
	LayeredMedianFilter(const std::string& name = "");

	/// apply threshold to all given images
	/// \implements ParameteredObject::execute
	virtual void execute();
};

#endif // LAYEREDMEDIANFILTER_H

