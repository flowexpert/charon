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
/// \file FlowQuiver.h
/// Declaration of the parameter class FlowQuiver.
/// \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
///     Jens-Malte Gottfried</a>
/// \date 27.11.2010

#ifndef FLOW_QUIVER_H_
#define FLOW_QUIVER_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef flowquiver_EXPORTS
///Visual C++ specific code
#define flowquiver_DECLDIR __declspec(dllexport)
#else
#define flowquiver_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define flowquiver_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

/// Draw overlay with arrows representing the optical flow.
/**
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup image-manipulators
 */
template<typename T>
class flowquiver_DECLDIR FlowQuiver : public TemplatedParameteredObject<T>
{
public:
	/// standard constructor
	FlowQuiver(const std::string& name = "" /** [in] instance name*/);

	/// quiver lenght
	Parameter<float> length;

	/// quiver sampling
	Parameter<unsigned int> sampling;

	/// quiver color
	ParameterList<unsigned int> color;

	/// image data input (rgb values) [t](x,y,z,c)
	InputSlot<cimg_library::CImgList<T> > in;

	/// flow input [u/v/...](x,y,z,t)
	InputSlot<cimg_library::CImgList<T> > flow;

	/// RGB data output [t](x,y,z,c)
	OutputSlot<cimg_library::CImgList<T> > out;

protected:
	/// \copybrief ParameteredObject::execute()
	/** \copydetails ParameteredObject::execute() */
	virtual void execute();
};

#endif // FLOW_QUIVER_H_
