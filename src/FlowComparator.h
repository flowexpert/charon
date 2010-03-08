/*  Copyright (C) 2009 <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>

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
/** \file FlowComparator.h
 *  Declaration of the parameter class FlowComparator.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 11.02.2010
 */

#ifndef _FLOWCOMPARATOR_H_
#define _FLOWCOMPARATOR_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef flowcomparator_EXPORTS
/// Visual C++ specific code
#define flowcomparator_DECLDIR __declspec(dllexport)
#else
#define flowcomparator_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define flowcomparator_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// Compare results of some flow calculation with the underlaying ground truth.
/// Calculation results are passed through, e.g. to be saved later.
/// This may avoid multiple calculations.
template <typename T>
class flowcomparator_DECLDIR FlowComparator :
		public TemplatedParameteredObject<T> {
public:
	/// flow result from some flow estimation 
	InputSlot < cimg_library::CImgList<T> > result;
	/// underlaying ground truth 
	InputSlot < cimg_library::CImgList<T> > groundtruth;
	/// result passthrough 
	OutputSlot < cimg_library::CImgList<T> > passthrough;

	/// create a new FlowComparator object
	/// \param name          Instance name
	FlowComparator(const std::string& name);

	/// Update object.
	virtual void execute();

	/// \name getters of calculated information
	//  \{
	double getMeanEndpointError() const;
	//  \}
};

#endif // _FLOWCOMPARATOR_H_



