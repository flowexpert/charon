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
/** \file FloReader.h
 * Declaration of the parameter class FloReader.
 * \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *     Michael Baron</a>
 * \date 24.10.2011
 */

#ifndef _FLOREADER_H_
#define _FLOREADER_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef floreader_EXPORTS
///Visual C++ specific code
#define floreader_DECLDIR __declspec(dllexport)
#else
#define floreader_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define floreader_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// Simple class to read vision.middlebury.edu/flow ground-truth files.
/** This class is able to read ground-truth as provided by
 *  vision.middlebury.edu
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup data-io
 */
template <typename T>
class floreader_DECLDIR FloReader : public TemplatedParameteredObject<T> {
public:
	/// filename to read ground-truth from
	Parameter<std::string> filename;

	/// set invalid flow values to zero
	Parameter<bool> invToZero;

	/// flow data as output slot
	OutputSlot<cimg_library::CImgList<T> > out;
	/// flow validity mask (1=valid,0=flow unknown)
	/** flow values are considered unknown
	 *  where |u| or |v| are larger than 1e9
	 */
	OutputSlot<cimg_library::CImgList<T> > valid;

	/// create a new sample object
	/// \param name             Object name
	FloReader(const std::string& name = "");

protected:
	/// Update object.
	/// Reload image and put new data into the output slot.
	virtual void execute();

private:
	/// maximal reasonable absolute value of flow component
	static const float maxKnown;
};

#endif // _FLOREADER_H_

