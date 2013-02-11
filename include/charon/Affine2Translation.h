/*  Copyright (C) 2013
                  University of Heidelberg (IWR/HCI)

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
/// \file Affine2Translation.h
/// Declaration of the parameter class Affine2Translation.
/// \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
///   Michael Baron</a>
/// \date 11.02.2013

#ifndef AFFINE2TRANSLATION_H_
#define AFFINE2TRANSLATION_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef affine2translation_EXPORTS
///Visual C++ specific code
#define affine2translation_DECLDIR __declspec(dllexport)
#else
#define affine2translation_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define affine2translation_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>
#include <charon-utils/Roi.h>

/// Transform affine motion parameters to translation
/**
 *  \ingroup charon-modules
 */
template<typename T>
class affine2translation_DECLDIR Affine2Translation : public TemplatedParameteredObject<T>
{
public:
	/// standard constructor
	Affine2Translation(const std::string& name = "" /** [in] instance name*/);

	/// parameter input
	InputSlot<cimg_library::CImgList<T> > parameters;
	/// roi input
	InputSlot< Roi<int>* > roi;

	/// flow output
	OutputSlot<cimg_library::CImgList<T> > flow;

protected:
	/// \copybrief ParameteredObject::execute()
	/** \copydetails ParameteredObject::execute() */
	virtual void execute();
};

#endif // AFFINE2TRANSLATION_H_

