/*  Copyright (C) 2009 Jens-Malte Gottfried <jmgottfried@web.de>

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
/** \file SimpleIterator.h
 *  Declaration of the parameter class SimpleIterator.
 *  \author Jens-Malte Gottfried <jmgottfried@web.de>
 *  \date 01.02.2010
 */

#ifndef _SIMPLEITERATOR_H_
#define _SIMPLEITERATOR_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef simpleiterator_EXPORTS
/// Visual C++ specific code
#define simpleiterator_DECLDIR __declspec(dllexport)
#else
#define simpleiterator_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define simpleiterator_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>
#include <charon/IteratorHelper.h>

#ifdef QT_GUI_LIB
#include <charon/SimpleIteratorRemoteControl.h>
#endif

/// Iterate parts of the processing pipeline.
/** This iterator executes the processing toolchain until the flow
 *  changes become small eanough or some given iteration count
 *  is reached.
 *
 *  You have to activate at least one stop criterion by
 *  setting maxRuns or epsilon to a value greater than zero.
 *
 *  \ingroup charon-iterators
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 */
template <typename T>
class simpleiterator_DECLDIR SimpleIterator :
		public TemplatedParameteredObject<T> {
private:
#ifdef QT_GUI_LIB
	/// remote control for this iterator
	SimpleIteratorRemoteControl *_remoteControl;
#endif

	/// Common initilization code
	void _init();

protected:
	/// Init class with given metadata information.
	/// Also generates a unique instance name if necessary.
	/// @param className    Initialization value for className property.
	/// @param name         Instance name (auto generation if empty)
	/// @param doc          Class docstring (for metadata)
	SimpleIterator(const std::string& className,
		const std::string& name, const std::string& doc);

	/** \name Hook functions to manipulate iteration code.
	 *  These functions are implemented as empty dummy functions
	 *  for usage in derived classes.
	 */
	//  \{
	/// Code executed before the first iteration starts.
	virtual void _preIterationHook();
	/// Code executed at the beginning of each iteration.
	virtual void _beginIterationHook();
	/// Code executed after the last iteration.
	/** This is executed before the helper is resetted, but
	 *  after result assignment.
	 *  This means you could even change the result there.
	 */
	virtual void _afterIterationHook();
	//  \}

public:
#ifdef QT_GUI_LIB
	/// set breakpoint on iterator
	Parameter < bool > breakPoint;
#endif
	/// maximum number of iterations (0 to disable) 
	Parameter < unsigned int > maxRuns;
	/// flow change threshold 
	Parameter < double > epsilon;
	/// norm postprocessing (implemented: "max", "mean")
	Parameter < std::string > norm;
	/// set to true if iteration calculates flow difference
	/// (operates with zero initial guess)
	Parameter < bool > accumulate;
	/// update rate, i.e. ratio of change that is used
	Parameter < double > updateRate;

	/// flow result calculaged during current iteration 
	InputSlot < cimg_library::CImgList<T> > flow;
	/// initial flow guess if different from helper
	InputSlot < cimg_library::CImgList<T> > flowInit;
	/// iterator helper input
	InputSlot < IteratorHelper<T>* > helper;
	/// external stop criterion
	InputSlot < bool > stop;
	/// final flow result after all iterations
	OutputSlot < cimg_library::CImgList<T> > result;

	/// create a new SimpleIterator object
	/** \param name          Instance name */
	SimpleIterator(const std::string& name = "");

	/// initialize iterator
	/** initialize helper object and run pre iteration hook */
	void prepareIterations();

	/// perform one Iteration
	/** run begin iteration hook and perform one iteration cycle
	 *  This consists of three steps:
	 *  - prepareStep()
	 *  - performStep()
	 *  - finishStep()
	 *  \retval true    no stop criterion has been fulfilled
	 */
	bool singleStep();

	/// prepare running one Iteration
	/** runs begin iteration hook */
	void prepareStep();

	/// perform running one Iteration
	/** calls execute on all connected objects to run the iteration cycle */
	void performStep();

	/// finish one Iteration
	/** update result and check stop criteria
	 *  \retval true    no stop criterion has been fulfilled
	 */
	bool finishStep();

	/// perform all Iterations
	/** calls singleStep() as long as no stop criterion has been
	 *  fulfilled.
	 */
	void iterate();

	/// finalize iterator
	/** Commit result to output slot, run post iteration hook
	 */
	void finishIterations();

protected:
	/// Update object
	/** \copydetails ParameteredObject::execute() */
	virtual void run();
};

#endif // _SimpleIterator_H_
