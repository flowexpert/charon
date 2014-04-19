/*

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
/** \file Mux2.h
 *  Declaration of the parameter class Mux2.
 *  \author Michael Baron
 *  \date 24.11.2011
 */

#ifndef _MUX2_H_
#define _MUX2_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef mux2_EXPORTS
/// Visual C++ specific code
#define mux2_DECLDIR __declspec(dllexport)
#else
#define mux2_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define mux2_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// Packs two CImgLists into one
/** Packs two CImgLists into one wrt the n index.
 *
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup converters
 */
template <typename T>
class mux2_DECLDIR Mux2 :
		public TemplatedParameteredObject<T> {
public:
	/// First image list
	InputSlot < cimg_library::CImgList<T> > list1;

	/// First image list
	InputSlot < cimg_library::CImgList<T> > list2;

	/// output slot
	OutputSlot < cimg_library::CImgList<T> > out;

	/// create a new Mux2 object
	/// \param name          Instance name
	Mux2(const std::string& name = "");

protected:
	/// Update object.
	virtual void execute();
};

#endif // _MUX2_H_

