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
/** @file IterativeSolver.hxx
 *  Implementation of class IteratorHelper.
 *  @author <a href="mailto:techfreaq@web.de">
 *      Nina Hernitschek</a>
 *  @date 26.02.2010
 */

#ifndef _ITERATORHELPER_HXX_
#define _ITERATORHELPER_HXX_

#include "IteratorHelper.h"

template <typename T>
void IteratorHelper<T>::_init() {

	this->_addInputSlot(imgListFileIn,"imgListFileIn",
		"CImgList from IterativeSolver","CImgList<T>");
	this->_addParameter(maxIterations,"maxIterations",
		"maximum number of iteration steps",5);
	this->_addOutputSlot(imgListOut,"imgListOut",
		"CImgList containing the CImgList used in iteration","CImgList<T>");
	this->_addOutputSlot(flowListOut,"flowListOut",
		"CImgList containing the flow from iterativeSolver","CImgList<T>");
	this->_addOutputSlot(iterationStepOut,"iterationStepOut",
		"current iteration step");
	this->_addOutputSlot(out,"this",
		"Pointer to itself", "IteratorHelper<T>*");

	out = this;

	iterationStep = 1;
}

template <typename T>
IteratorHelper<T>::IteratorHelper(
		const std::string& classname,
		const std::string& name,
		const std::string& doc) :
		TemplatedParameteredObject<T>(classname,name,
			doc + "<br><br>derived by class IteratorHelper")
{
	_init();
}

template <typename T>
IteratorHelper<T>::IteratorHelper(
		const std::string& name) :
		TemplatedParameteredObject<T>("IteratorHelper",name,
			"Helper for iterative solvers")
{
	_init();
}

template <typename T>
void IteratorHelper<T>::update(cimg_library::CImgList<T> imgList, cimg_library::CImgList<T> flowList)
{
	imgListIn = imgList;
	flowListIn = flowList;
}

template <typename T>
void IteratorHelper<T>::execute() {
	ParameteredObject::execute();

	sout << "IteratorHelper, iteration " << iterationStep << std::endl;

	if (iterationStep<=maxIterations)
	{
		if(iterationStep==1) // first iteration step
		{
			// read from file, use imgListFile In

			imgListOut=imgListFileIn;	

			// initial guess: flow is 0

		/*	unsigned int width = imgListFileIn()[0].width();
			unsigned int height = imgListFileIn()[0].height();
			unsigned int depth = imgListFileIn()[0].depth();
		*/
			flowListOut().assign(3,imgListFileIn()[0].width(),imgListFileIn()[0].height(),imgListFileIn()[0].depth(), 1, 0);
		}
		else
		{
			// read from IterativeSolver, use imgListIn
			imgListOut=imgListIn;
			
			// use calculated flow
			flowListOut=flowListIn;
		}
	}
}


template <typename T>
void IteratorHelper<T>::nextStep() {
	iterationStep++;
	iterationStepOut = iterationStep;

	execute();
}

template <typename T>
int IteratorHelper<T>::getCurrentStep() {
	return iterationStep;
}

template <typename T>
int IteratorHelper<T>::getMaxIterations() {
	return maxIterations;
}

#endif // _ITERATORHELPER_HXX_


