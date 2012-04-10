/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License
	as published by the Free Software Foundation, either version 3
	of the License, or (at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/// \file Toggle.h
/// Declaration of the parameter class Toggle.
/// \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
///     Michael Baron</a>
/// \date 10.04.2012

#ifndef TOGGLE_H_
#define TOGGLE_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef toggle_EXPORTS
///Visual C++ specific code
#define toggle_DECLDIR __declspec(dllexport)
#else
#define toggle_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define toggle_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/ToggleTimeout.h>
#include <charon-utils/CImg.h>

/// Toggle interface
class ToggleInterface
{
public:
	/// toggle function
	virtual void toggleToggle() = 0;
};

/// Toggle two inputs wrt a given timeout.
/**
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 */
template<typename T>
class toggle_DECLDIR Toggle :
	public TemplatedParameteredObject<T>,
	public virtual ToggleInterface
{
public:
	/// standard constructor
	Toggle(const std::string& name = "" /** [in] instance name*/);

	/// \copybrief ParameteredObject::execute()
	/** \copydetails ParameteredObject::execute() */
	virtual void execute();

	/// timeout parameter
	Parameter< int > timeout;

	/// first input slot
	InputSlot<cimg_library::CImgList<T> > in1;

	/// second input slot
	InputSlot<cimg_library::CImgList<T> > in2;

	/// output slot
	OutputSlot<cimg_library::CImgList<T> > out;

	/// toggleToggle interface implementation
	void toggleToggle();

private:
	/// toggle timeout object
	ToggleTimeout *toggleTimeout;

	/// toggle state
	int state;
};

#endif

