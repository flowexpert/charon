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
template<typename T>
class flow2hsv_DECLDIR Flow2HSV : public TemplatedParameteredObject<T>
{
public:
	/// standard constructor
	Flow2HSV(const std::string& name = "" /** [in] instance name*/) ;

	/// \copybrief ParameteredObject::execute()
	/** \copydetails ParameteredObject::execute() */
	virtual void execute() ;

	/// select how image is scaled: 0 saturation, 1 value, 2 none
	Parameter<int> scaleChannel ;

	/// flow input
	InputSlot<cimg_library::CImgList<T> > flow;

	/// hsv output
	OutputSlot<cimg_library::CImgList<T> > out;
};

#endif // FlOW_TO_HSV_H_
