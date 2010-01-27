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
/** @file PsiSolver.hxx
 *  Implementation of class PsiSolver.
 *  @author <a href="mailto:techfreaq@web.de">
 *      Nina Hernitschek</a>
 *  @date 23.11.2009
 */

#ifndef _PSISOLVER_HXX_
#define _PSISOLVER_HXX_

#include "PsiSolver.h"

template <typename T>
Solver<T>::Solver(const std::string& classname, const std::string& name) :
		TemplatedParameteredObject<T>(classname,name,"solves the linear system")
{
	this->_addInputSlot(imgin,"imgListIn","CImgList","CImgList<T>");
	this->_addInputSlot(imgin,"imgFluxIn","CImgList","CImgList<T>");
	this->_addOutputSlot(imgout,"imgListOut","CImgList containing the solution","CImgList<T>");
	this->_addInputSlot(interpolator, "interpolator", "Pointer to Interpolator", "Interpolator<T>*");

	out() = result;
}


template <typename T>
Solver<T>::~Solver() {}

#endif // _PSISOLVER_HXX_
