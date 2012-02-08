/*  Copyright (C) 2011 Julian Coordts

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
/** \file EmptyCImg.h
 *  Declaration of the class EmptyCImg.
 *  \author Julian Coordts
 *  \date 13.01.2012
 */


#ifndef _EMPTYCIMG_H_
#define _EMPTYCIMG_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef emptycimg_EXPORTS
/// Visual C++ specific code
#define emptycimg_DECLDIR __declspec(dllexport)
#else
#define emptycimg_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define emptycimg_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// plugin does nothing
template <typename T>
class emptycimg_DECLDIR EmptyCImg :
		public TemplatedParameteredObject<T> {
public:

	/// output image
	OutputSlot < cimg_library::CImgList<T> > _output;
	
	/// Parameters
	Parameter < int > _sizeX;

	/// Parameters
	Parameter < int > _sizeY;

	/// Parameters
	Parameter < int > _sizeZ;
	
	/// Parameters
	Parameter < int > _sizeC;

	/// Parameters
	Parameter < int > _sizeT;

	/// fill in value
	Parameter < T > _value ;


	
    /// create a new EmptyCImg object
	/// \param name          Instance name
    EmptyCImg(const std::string& name);

	/// Update object.
	virtual void execute();

};

#endif // _EMPTYCIMG_H_
