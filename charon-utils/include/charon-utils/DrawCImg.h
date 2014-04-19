/*  Copyright (C) 2011 Julian Coordts

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
/** \file DrawCImg.h
 *  Declaration of the class DrawCImg.
 *  \author Julian Coordts
 *  \date 13.01.2012
 */

#ifndef _DRAWCIMG_H_
#define _DRAWCIMG_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef drawcimg_EXPORTS
/// Visual C++ specific code
#define drawcimg_DECLDIR __declspec(dllexport)
#else
#define drawcimg_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define drawcimg_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// plugin does nothing
template <typename T>
class drawcimg_DECLDIR DrawCImg :
		public TemplatedParameteredObject<T> {
public:
	/// input image
	InputSlot < cimg_library::CImgList<T> > _inputSource;

	/// input image
	InputSlot < cimg_library::CImgList<T> > _inputTarget;

	/// output image
	OutputSlot < cimg_library::CImgList<T> > _output;

	/// Parameters
	Parameter < int > _posX;

	/// Parameters
	Parameter < int > _posY;

	/// Parameters
	Parameter < float > _opacity;

	/// Parameters
	Parameter < bool > _skipBlack;

	/// Parameters
	Parameter < float > _threshold;

	/// create a new DrawCImg object
	/// \param name          Instance name
	DrawCImg(const std::string& name);

protected:
	/// Update object.
	virtual void execute();
};

#endif // _DRAWCIMG_H_
