/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

	This file is part of charon-free.

	charon-free is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License
	as published by the Free Software Foundation, either version 3
	of the License, or (at your option) any later version.

	charon-free is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with charon-free.  If not, see <http://www.gnu.org/licenses/>.
*/
/// \file Quiver.h
/// Declaration of the parameter class Quiver.
/// \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
///     Michael Baron</a>
/// \date 23.04.2012

#ifndef QUIVER_H_
#define QUIVER_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef quiver_EXPORTS
///Visual C++ specific code
#define quiver_DECLDIR __declspec(dllexport)
#else
#define quiver_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define quiver_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/QuiverPlot.h>
#include <charon-utils/CImg.h>

class QWidget;
class mglData;
class mglGraph;

/// Plot flow quiver as a transparent widget
/// onto an image displayed by an ArgosDisplay.
/**
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 */
template<typename T>
class quiver_DECLDIR Quiver :
	public TemplatedParameteredObject<T>
{
public:
	/// standard constructor
	Quiver(const std::string& name = "" /** [in] instance name*/);

	/// \copybrief ParameteredObject::execute()
	/** \copydetails ParameteredObject::execute() */
	virtual void execute();

	/// motion input slot
	InputSlot<cimg_library::CImgList<T> > motion;

	/// output slot
	OutputSlot< QWidget* > _display;

	/// output slot (CImgList)
	OutputSlot< cimg_library::CImgList<T> > quiverImage;

	/// step width parameter
	Parameter< unsigned int > stepWidth;

private:
	mglData motionUdata;
	mglData motionVdata;

	QuiverPlot *quiverPlot;

	bool hasWidget;
};

#endif

