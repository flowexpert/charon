/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

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
/** \file GetRoi.h
 *  Declaration of the parameter class GetRoi.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 03.08.2012
 */

#ifndef _GETROI_H_
#define _GETROI_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef getroi_EXPORTS
/// Visual C++ specific code
#define getroi_DECLDIR __declspec(dllexport)
#else
#define getroi_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define getroi_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>
#include <charon-utils/Roi.h>

/// Returns the dimensions of the input sequence as a region of interest
/**
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 */
template <typename T>
class getroi_DECLDIR GetRoi :
		public TemplatedParameteredObject<T> {
public:
	/// sequence input
	InputSlot < cimg_library::CImgList<T> > seqIn;

	/// current size
	OutputSlot < Roi<int>* > size;

	/// create a new GetRoi object
	/// \param name          Instance name
	GetRoi(const std::string& name = "");

protected:
	/// Update object.
	virtual void execute();

private:
	/// content for roi pointer output slot
	Roi<int> _size;
};

#endif // _GETROI_H_

