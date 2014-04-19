/*  Copyright (C) 2010 Stephan Meister

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
/** \file Cut.h
 *  Declaration of the parameter class Cut.
 *  \author Stephan Meister
 *  \date 30.07.2010
 */

#ifndef _CUT_H_
#define _CUT_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef cut_EXPORTS
/// Visual C++ specific code
#define cut_DECLDIR __declspec(dllexport)
#else
#define cut_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define cut_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// calculate depth map from given disparity map
template <typename T>
class cut_DECLDIR Cut :
		public TemplatedParameteredObject<T> {
public:
	/// lower cutoff value
	Parameter < double > _lowerCut ;
	/// upper cutoff value
	Parameter < double > _upperCut ;

	/// normalize
	Parameter < bool > _normalize ;

	/// input image
	InputSlot < cil::CImgList<T> > _in;
	/// cut output
	OutputSlot < cil::CImgList<T> > _out;

	/// create a new Cut object
	/// \param name          Instance name
	Cut(const std::string& name);

protected:
	/// Update object.
	virtual void execute();
};

#endif // _DISP2DEPTH_H_



