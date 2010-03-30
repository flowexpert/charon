/*  This file is part of Charon.

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
/** @file IteratorHelper.h
 *  Implementation of class IteratorHelper.
 *  @author <a href="mailto:techfreaq@web.de">
 *      Nina Hernitschek</a>
 *  @date 26.02.2010
 */

#ifndef _ITERATORHELPER_H_
#define _ITERATORHELPER_H_

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

/// helper class used for iterative solving
template <typename T>
class iteratorhelper_DECLDIR IteratorHelper :
		public TemplatedParameteredObject<T> {

protected:
	/// constructor for derived classes
	IteratorHelper(
		const std::string& classname  /** [in] class name          */,
		const std::string& name       /** [in] instance name       */,
		const std::string& doc        /** [in] class documentation */
	);

public:
	/// \name pointers used by the IteratorHelper
	//  \{
	/// CImgList from file
	InputSlot<cimg_library::CImgList<T> > imgListFileIn;
	/// CImgList from IterativeSolver
	//InputSlot<cimg_library::CImgList<T> > imgListIn;
	/// output imgList
	OutputSlot<cimg_library::CImgList<T> > imgListOut;
	/// Output slot containing current iteration step
	OutputSlot < unsigned int > iterationStepOut;
	/// Output slot containing the this-pointer of the object
	OutputSlot<IteratorHelper<T>*> out;


	/// number of iterations
	Parameter<int> iterations;
	//  \}

	/// default constructor
	IteratorHelper(const std::string& name = "" /**[in] instance name*/);

	/// main function
	virtual void execute();


	/// increments iterationStep
	virtual void nextStep();

	/// update image
	virtual void update(cimg_library::CImgList<T> imgList);

private:
	/// common initialization code
	void _init();

	/// imageList
	cimg_library::CImgList<T> imgListIn;

	/// current iteration
	unsigned int iterationStep;
};


#endif // _ITERATORHELPER_H_
