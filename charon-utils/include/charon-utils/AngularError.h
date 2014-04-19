/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

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
/// \file AngularError.h
/// Declaration of the parameter class AngularError.
/// \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
///     Michael Baron</a>
/// \date 03.04.2012

#ifndef ANGULARERROR_H_
#define ANGULARERROR_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef angularerror_EXPORTS
///Visual C++ specific code
#define angularerror_DECLDIR __declspec(dllexport)
#else
#define angularerror_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define angularerror_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

/// Compare two flow fields wrt their endpoint error.
/**
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 */
template<typename T>
class angularerror_DECLDIR AngularError : public TemplatedParameteredObject<T>
{
public:
	/// standard constructor
	AngularError(const std::string& name = "" /** [in] instance name*/);

	/// \copybrief ParameteredObject::execute()
	/** \copydetails ParameteredObject::execute() */
	virtual void execute();

	/// flow input
	InputSlot<cimg_library::CImgList<T> > flow;

	/// ground truth input
	InputSlot<cimg_library::CImgList<T> > groundTruth;

	/// angular error output
	OutputSlot<cimg_library::CImgList<T> > out;
};

#endif

