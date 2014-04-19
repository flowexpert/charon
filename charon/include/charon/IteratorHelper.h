/*  Copyright (C) 2009 Jens-Malte Gottfried <jmgottfried@web.de>

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
/** \file IteratorHelper.h
 *  Declaration of the parameter class IteratorHelper.
 *  \author Jens-Malte Gottfried <jmgottfried@web.de>
 *  \date 01.02.2010
 */

#ifndef _ITERATOR_HELPER_H_
#define _ITERATOR_HELPER_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef iteratorhelper_EXPORTS
/// Visual C++ specific code
#define iteratorhelper_DECLDIR __declspec(dllexport)
#else
#define iteratorhelper_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define iteratorhelper_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// Helper for iterative image processing algorithms.
/** This provides current values/initial values for further processing.
 *
 *  If an inital flow is provided via the optional initFlow input slot,
 *  it is copied to the flow output in the first iteration.
 *  If left unconnected, an initial zero flow is generated matching the
 *  input sequence x/y/z dimensions and using the flowFrames (as t) and
 *  flowDimensions (as v). Auto detection of flowFrames means one frame
 *  less than the input sequence.
 *
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 *  \ingroup charon-iterators
 */
template <typename T>
class iteratorhelper_DECLDIR IteratorHelper :
		public TemplatedParameteredObject<T> {
public:
	/// original sequence input
	InputSlot < cimg_library::CImgList<T> > in;
	/// initial flow guess
	InputSlot < cimg_library::CImgList<T> > initFlow;
	/// number of flow components for initialization
	Parameter < unsigned int > flowDimensions;
	/// number of frames for initial flow ( use -1 for auto detection )
	Parameter < int > flowFrames;
	/// original sequence output
	OutputSlot < cimg_library::CImgList<T> > sequence;
	/// current flow solution
	OutputSlot < cimg_library::CImgList<T> > flow;
	/// current residual energy
	OutputSlot < cimg_library::CImgList<T> > residual;
	/// iteration counter (resetted on iteration start, e.g. inner loop)
	OutputSlot < unsigned int > count;
	/// iteration counter (not resetted, increasing only)
	OutputSlot < unsigned int > countAll;
	/// self-pointer
	OutputSlot < IteratorHelper<T>* > self;

	/// default constructor
	IteratorHelper(const std::string& name = "" /** [in] Instance name*/);

	/// reset helper, copy input data to output slots, reset counters
	void reset();

	/// update counters
	/** \param[in] count increase counters */
	virtual void update (bool count = true);

protected:
	/// Update object.
	virtual void execute();
};

#endif // _ITERATOR_HELPER_H_
