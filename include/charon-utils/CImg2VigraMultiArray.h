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
/** @file CImg2VigraMultiArray.h
 * Declaration of the parameter class CImg2VigraMultiArray.
 * @author Daniel Kondermann
 * @date 19.01.2010
 */

#ifndef _CIMG2VIGRAMULTIARRAY_H_
#define _CIMG2VIGRAMULTIARRAY_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef cimg2vigramultiarray_EXPORTS
///Visual C++ specific code
#define cimg2vigramultiarray_DECLDIR __declspec(dllexport)
#else
#define cimg2vigramultiarray_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define cimg2vigramultiarray_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <vigra/multi_array.hxx>
#include <vigra/windows.h>
#include <charon-utils/CImg.h>

/// Convert a CImgList-Object to a vigra::MultiArray<5, T>
/** The data is copied.
 */
template <typename T>
class cimg2vigramultiarray_DECLDIR CImg2VigraMultiArray :
	public TemplatedParameteredObject<T> {

public:
	/// The CImgList object to be converted 
	InputSlot < cimg_library::CImgList<T> > in;
	/// A copy of the image stored as vigra::MultiArray<5, T>
	OutputSlot < vigra::MultiArray<5, T> > out;

	/// create a new CImg2VigraMultiArray object
	/// \param name             Instance name
	CImg2VigraMultiArray(const std::string& name = "");

	/// Update object.
	virtual void execute();
};

#endif // _CImg2VigraMultiArray_H_



