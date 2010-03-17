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
 *  Implementation of class IterativeSolver.
 *  @author <a href="mailto:techfreaq@web.de">
 *      Nina Hernitschek</a>
 *  @date 23.11.2009
 */

#ifndef _ITERATIVESOLVER_HXX_
#define _ITERATIVESOLVER_HXX_

#include "IterativeSolver.h"

template <typename T>
IterativeSolver<T>::IterativeSolver(
		const std::string& classname,
		const std::string& name,
		const std::string& doc) :
		TemplatedParameteredObject<T>(classname,name,
				doc + "<br><br>This class inherits IterativeSolver.")
{
	this->_addInputSlot(imgListIn,"imgListIn",
		"CImgList","CImgList<T>");
	this->_addInputSlot(flowListIn,"flowListIn",
		"CImgList","CImgList<T>");
	this->_addInputSlot(interpolator, "interpolator",
		"Pointer to Interpolator", "Interpolator<T>*");
	this->_addInputSlot(iteratorHelper, "iteratorHelper",
		"Pointer to iteratorHelper", "IteratorHelper<T>*");
	this->_addOutputSlot(flowListOut,"flowListOut",
		"CImg containing the solution","CImgList<T>");
	//this->_addOutputSlot(imgListOut,"imgListOut",
	//	"CImgList containing the warped images","CImgList<T>");
}

#endif // _ITERATIVESOLVER_HXX_
