/*  Copyright (C) 2013
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
/** \file SelectFromList.h
 *  Declaration of the parameter class SelectFromList.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 21.03.2013
 */

#ifndef _SELECTFROMLIST_H_
#define _SELECTFROMLIST_H_

#ifdef _MSC_VER
#ifdef selectfromlist_EXPORTS
/// Visual C++ specific code
#define selectfromlist_DECLDIR __declspec(dllexport)
#else
#define selectfromlist_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define selectfromlist_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

/// Select an image from an image list
/** Select an image from an image list.
 */
template <typename T>
class selectfromlist_DECLDIR SelectFromList :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	SelectFromList(const std::string& name = "");

	/// image list input
	InputSlot< cimg_library::CImgList<T> > list;

	/// image output
	OutputSlot< cimg_library::CImgList<T> > out;

	/// index of first frame
	InputSlot< unsigned int > first;

	/// count of frames to return
	Parameter< unsigned int > count;

	/// offset parameter
	Parameter< int > offset;

protected:
	/// Update object.
	virtual void execute();
};

#endif // _SELECTFROMLIST_H_

