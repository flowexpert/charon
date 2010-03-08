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
/** \file SequenceGenerator.h
 *  Declaration of the parameter class SequenceGenerator.
 *  \author Cornelius Ratsch
 *  \date 04.03.2010
 */

#ifndef _SEQUENCEGENERATOR_H_
#define _SEQUENCEGENERATOR_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef sequencegenerator_EXPORTS
/// Visual C++ specific code
#define sequencegenerator_DECLDIR __declspec(dllexport)
#else
#define sequencegenerator_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define sequencegenerator_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <CImg.h>

/// Generates an image sequence out of multiple images
template <typename T>
class sequencegenerator_DECLDIR SequenceGenerator :
		public TemplatedParameteredObject<T> {
public:
	

	/// One or more images 
	InputSlot < cimg_library::CImgList<T> > images;
	/// image sequence containing every input image 
	OutputSlot < cimg_library::CImgList<T> > image_sequence;
	

	/// create a new SequenceGenerator object
	/// \param name          Instance name
	SequenceGenerator(const std::string& name);

	/// Update object.
	virtual void execute();
};

#endif // _SEQUENCEGENERATOR_H_



