/*  Copyright (C) 2012, 2013
                  University of Heidelberg (IWR/HCI)

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

/** \file SimpleDiff.h
 *  Declaration of the parameter class SimpleDiff.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 09.10.2012 - 09.01.2013
 */

#ifndef _SIMPLEDIFF_H_
#define _SIMPLEDIFF_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef simplediff_EXPORTS
/// Visual C++ specific code
#define simplediff_DECLDIR __declspec(dllexport)
#else
#define simplediff_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define simplediff_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

#include <charon/Warper.h>

/// Calculate 2D derivatives.
/** This plugin calculates two dimensional derivatives.
 *
 *  \ingroup charon-image-manipulators
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T>
class simplediff_DECLDIR SimpleDiff :
		public TemplatedParameteredObject<T> {
public:
	/// image input 
	InputSlot < cimg_library::CImgList<T> > img;
	/// warper input
	InputSlot < Warper<T>* > warper;

	/// derivative wrt x 
	OutputSlot < cimg_library::CImgList<T> > dx;
	/// derivative wrt y 
	OutputSlot < cimg_library::CImgList<T> > dy;
	/// derivative wrt t 
	OutputSlot < cimg_library::CImgList<T> > dt;

	/// create a new SimpleDiff object
	/// \param name          Instance name
	SimpleDiff(const std::string& name = "");

protected:
	/// Update object.
	virtual void execute();

	/// cubic interpolation with mirroring at image borders
	T __cubic_atXY(cimg_library::CImg<T> img, const float fx, const float fy);

	/// image dimensions
	//\{
	int _width;
	int _height;
	//\}
};

#endif // _SIMPLEDIFF_H_

