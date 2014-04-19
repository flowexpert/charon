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
/** \file Demux2.h
 *  Declaration of the parameter class Demux2.
 *  \author Michael Baron
 *  \date 24.11.2011
 */

#ifndef _DEMUX2_H_
#define _DEMUX2_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef demux2_EXPORTS
/// Visual C++ specific code
#define demux2_DECLDIR __declspec(dllexport)
#else
#define demux2_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define demux2_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// Packs one CImgList into two
/** Packs one CImgList into two wrt the n index.
 *
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup converters
 */
template <typename T>
class demux2_DECLDIR Demux2 :
		public TemplatedParameteredObject<T> {
public:
	/// input image list
	InputSlot < cimg_library::CImgList<T> > in;

	/// output slot 1
	OutputSlot < cimg_library::CImgList<T> > out1;

	/// output slot 2
	OutputSlot < cimg_library::CImgList<T> > out2;

	/// create a new Demux2 object
	/// \param name          Instance name
	Demux2(const std::string& name = "");

protected:
	/// Update object.
	virtual void execute();
};

#endif // _DEMUX2_H_

