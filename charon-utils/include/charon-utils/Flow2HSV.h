/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/// \file Flow2HSV.h
/// Declaration of the parameter class Flow2HSV.
/// \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
///     Jens-Malte Gottfried</a>
/// \date 26.11.2010

#ifndef FlOW_TO_HSV_H_
#define FlOW_TO_HSV_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef flow2hsv_EXPORTS
///Visual C++ specific code
#define flow2hsv_DECLDIR __declspec(dllexport)
#else
#define flow2hsv_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define flow2hsv_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

/// Convert Flow into HSV Color Representation
/**
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup image-manipulators
 */
template<typename T>
class flow2hsv_DECLDIR Flow2HSV : public TemplatedParameteredObject<T>
{
public:
	/// standard constructor
	Flow2HSV(const std::string& name = "" /** [in] instance name*/);

	/// select how image is scaled: 0 saturation, 1 value, 2 none
	Parameter<unsigned int> scaleChannel;

	/// normalization factor (0=auto)
	Parameter<T> normalizationFactor;

	/// if motion exceeds maximum, it will be truncated, zero to disable
	Parameter< T > maxMotion;

	/// if a flow component exceeds this value, this pixel will be
	/// considered as invalid and be ignored (zero flow)
	Parameter < double > invalid;

	/// flow input
	InputSlot<cimg_library::CImgList<T> > flow;

	/// HSV output (converted to RGB).
	/** The CImg list uses the convention [t](x,y,z,c) to be able to be
	 *  displayed properly. */
	OutputSlot<cimg_library::CImgList<T> > out;

protected:
	/// \copybrief ParameteredObject::execute()
	/** \copydetails ParameteredObject::execute() */
	virtual void execute();
};

#endif // FlOW_TO_HSV_H_

