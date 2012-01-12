/*
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
/** \file FloWriter.h
 * Declaration of the parameter class FloWriter.
 * \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *     Michael Baron</a>
 * \date 23.11.2011
 */

#ifndef _FLOWRITER_H_
#define _FLOWRITER_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef flowriter_EXPORTS
///Visual C++ specific code
#define flowriter_DECLDIR __declspec(dllexport)
#else
#define flowriter_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define flowriter_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// Simple class to write vision.middlebury.edu/flow motion files.
/** This class is able to write motion as provided by
 *  vision.middlebury.edu
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup data-io
 */
template <typename T>
class flowriter_DECLDIR FloWriter : public TemplatedParameteredObject<T> {
public:
	/// filename to read ground-truth from
	Parameter<std::string> filename;

	/// motion data as output slot
	InputSlot<cimg_library::CImgList<T> > in;

	/// create a new sample object
	/// \param name             Object name
	FloWriter(const std::string& name = "");

protected:
	/// Update object.
	/// Reload image and put new data into the output slot.
	virtual void execute();
};

#endif // _FLOWRITER_H_

