/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/// @file LinFilter.h
/// Declaration of the parameter class LinFilter
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @date 02.10.2009

#ifndef LINFILTER_H_
#define LINFILTER_H_

#include <charon-core/ParameteredObject.h>
#include "CImg.h"

#ifdef HANDLE_DLL
#ifdef linfilter_EXPORTS
/// DLL handling
#define linfilter_DECLDIR __declspec(dllexport)
#else
#define linfilter_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// No DLL handling
#define linfilter_DECLDIR
#endif

/**
 * Linear filtering plugin.
 *
 * This filter plugin convolves a given image (input slot) with
 * fiven filter masks (mask input) and provides the result on its
 * output slot. If more than one mask is given, they are applied
 * sequently.
 */
template <typename T>
class linfilter_DECLDIR LinFilter : public TemplatedParameteredObject<T> {
public:
	/// standart constructor
	/// \param name		instance name
	LinFilter(const std::string& name = "");

	/// filter mask input (multislot)
	InputSlot<cimg_library::CImgList<T> > masks;
	/// image input
	InputSlot<cimg_library::CImgList<T> > in;
	/// result output
	OutputSlot<cimg_library::CImgList<T> > out;

	/** Convolve with given filter masks.
	 *  \copydetails ParameteredObject::execute()
	 *
	 * The filter masks are convolved with each element in the CImgList.
	 * Make sure, that the mask-"lists" contain exactly one element.
	 */
	virtual void execute();
};

#endif // LINFILTER_H_

