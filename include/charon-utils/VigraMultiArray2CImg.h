/*  Copyright (C) 2009 Daniel Kondermann

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
/** @file VigraMultiArray2CImg.h
 * Declaration of the parameter class VigraMultiArray2CImg.
 * @author Daniel Kondermann
 * @date 19.01.2010
 *

 */

#ifndef _VIGRAMULTIARRAY2CIMG_H_
#define _VIGRAMULTIARRAY2CIMG_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef vigramultiarray2cimg_EXPORTS
///Visual C++ specific code
#define vigramultiarray2cimg_DECLDIR __declspec(dllexport)
#else
#define vigramultiarray2cimg_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define vigramultiarray2cimg_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <vigra/multi_array.hxx>
#include <charon-utils/CImg.h>
using namespace cimg_library;

/// Convert a vigra::MultiArrayView<5, T> to a CImgList-Object
/** The data is copied for compatibility reasons. Please note that this
 *  conversion is not always possible if the MultiArray is strided or has too
 *  many dimensions! Dimensions are mapped the following way:
 *  0->X, 1->Y, 2->Z, 3->V, 4->ListIndex.
 *
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup converters
 */
template <typename T>
class vigramultiarray2cimg_DECLDIR VigraMultiArray2CImg :
		public TemplatedParameteredObject<T> {
public:

	/// The vigra::MultiArray object to be converted. 
	InputSlot < vigra::MultiArrayView<5, T> > in;
	/// The same image but with a CImgList<T> data structure around it. 
	OutputSlot < CImgList<T> > out;

	/// create a new VigraMultiArray2CImg object
	/// @param name             Object name
	VigraMultiArray2CImg(const std::string& name = "");

	/// Update object.
	virtual void execute();
};

#endif // _VigraMultiArray2CImg_H_



