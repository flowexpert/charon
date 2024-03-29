/*  Copyright (C) 2014 Jens-Malte Gottfried

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
/** \file RoiOffset.h
 *  Declaration of the parameter class RoiOffset.
 *  \author <a href="mailto:jmgottfried@web.de">
 *      Jens-Malte Gottfried</a>
 *  \date 16.01.2014
 */

#ifndef _ROIOFFSET_H_
#define _ROIOFFSET_H_

#ifdef _MSC_VER
#ifdef roioffset_EXPORTS
/// Visual C++ specific code
#define roioffset_DECLDIR __declspec(dllexport)
#else
#define roioffset_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define roioffset_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/Roi.h>

/// Adds given values as offset to the ROI borders
/** The borders of the passed ROI are incremented by the connected values.
 *  There are five slots corresponding the the five ROI dimensions.
 *  Unconnected slots are treated as zero, leaving this dimension
 *  unchanged.
 *  The offset is applied as well to the lower and the upper ROI border.
 *
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup charon-helpers
 */
template <typename T>
class roioffset_DECLDIR RoiOffset :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	RoiOffset(const std::string& name = "");

	/// ROI input
	InputSlot< Roi<T>* > in;
	/// x offset
	InputSlot< T > dx;
	/// y offset
	InputSlot< T > dy;
	/// z offset
	InputSlot< T > dz;
	/// t offset
	InputSlot< T > dt;
	/// v offset
	InputSlot< T > dv;

	/// ROI output
	OutputSlot< Roi<T>* > out;

protected:
	/// Update object.
	virtual void execute();

private:
	/// ROI copy to be modified and used for output
	Roi<T> _roi;
};

#endif // _ROIOFFSET_H_
