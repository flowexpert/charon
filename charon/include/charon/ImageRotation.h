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
/// \file ImageRotation.h
/// Declaration of the parameter class ImageRotation.
/// \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
///   Michael Baron</a>
/// \date 11.02.2013

#ifndef IMAGEROTATION_H_
#define IMAGEROTATION_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef imagerotation_EXPORTS
///Visual C++ specific code
#define imagerotation_DECLDIR __declspec(dllexport)
#else
#define imagerotation_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define imagerotation_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

/// Image rotation and ground-truth generation
/**
 *  \ingroup charon-modules
 */
template<typename T>
class imagerotation_DECLDIR ImageRotation : public TemplatedParameteredObject<T>
{
public:
	/// standard constructor
	ImageRotation(const std::string& name = "" /** [in] instance name*/);

	/// image input
	InputSlot<cimg_library::CImgList<T> > in;

	/// image output
	OutputSlot<cimg_library::CImgList<T> > out;

	/// ground truth output
	OutputSlot<cimg_library::CImgList<T> > groundTruth;

	/// rotation angle
	Parameter< T > angle;

	/// boundary condition (0==dirichlet, 1==neumann, 2==cyclic)
	Parameter< unsigned int > boundaryCondition;

	/// interpolation type (0==nearest, 1==linear, 2==cubic)
	Parameter< unsigned int > interpolationType;

protected:
	/// \copybrief ParameteredObject::execute()
	/** \copydetails ParameteredObject::execute() */
	virtual void execute();
};

#endif // IMAGEROTATION_H_

