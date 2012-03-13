/*  Copyright (C) 2009 Cornelius Ratsch

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
/** \file Images2Sequence.h
 *  Declaration of the parameter class Images2Sequence.
 *  \author Cornelius Ratsch
 *  \date 04.03.2010
 */

#ifndef _IMAGES2SEQUENCE_H_
#define _IMAGES2SEQUENCE_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef images2sequence_EXPORTS
/// Visual C++ specific code
#define images2sequence_DECLDIR __declspec(dllexport)
#else
#define images2sequence_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define images2sequence_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// Generates an image sequence out of multiple images
/** The images are sorted by the name of the connected module in
 *  ascending order.
 *
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup converters
 */
template <typename T>
class images2sequence_DECLDIR Images2Sequence :
		public TemplatedParameteredObject<T> {
public:
	/// One or more images 
	InputSlot < cimg_library::CImgList<T> > images;
	/// image sequence containing every input image 
	OutputSlot < cimg_library::CImgList<T> > image_sequence;
	

	/// create a new Images2Sequence object
	/// \param name          Instance name
	Images2Sequence(const std::string& name = "");

protected:
	/// Update object.
	virtual void execute();
};

#endif // _IMAGES2SEQUENCE_H_



