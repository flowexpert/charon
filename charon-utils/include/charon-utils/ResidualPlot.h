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
/** \file ResidualPlot.h
 *  Declaration of the parameter class ResidualPlot.
 *  \author Michael Baron
 *  \date 11.04.2012
 */

#ifndef _RESIDUALPLOT_H_
#define _RESIDUALPLOT_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef residualplot_EXPORTS
/// Visual C++ specific code
#define residualplot_DECLDIR __declspec(dllexport)
#else
#define residualplot_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define residualplot_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

class QWidget ;
class ResidualPlotWidget ;

/// Charon Plugin Frontend for residual energy plotting
/**
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup visualization
 */
template <typename T>
class residualplot_DECLDIR ResidualPlot : public TemplatedParameteredObject<T> {
public:
	/// constructor
	ResidualPlot(const std::string& name);
	/// destructor
	~ResidualPlot() ;

	/// The CImgList input
	InputSlot < cimg_library::CImgList<T> > residual;

	/// exported QWidget to display the results
	OutputSlot < QWidget* > _display ;

	/// Update object.
	virtual void execute();

private:
	/// pointer to the export widget
	ResidualPlotWidget* _exportWidget ;

	/// execution count
	int executionCount;

}; //class ResidualPlot

#endif // _RESIDUALPLOT_H_

