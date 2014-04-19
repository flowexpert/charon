/*

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
/** \file CGSolverHelper.hxx
 *  Implementation of the parameter class CGSolverHelper.
 *  \author Michael Baron <michael.baron@iwr.uni-heidelberg.de>
 *  \date 22.11.2011
 */

#ifndef _CGSOLVER_HELPER_HXX_
#define _CGSOLVER_HELPER_HXX_

#include "CGSolverHelper.h"

template <typename T>
CGSolverHelper<T>::CGSolverHelper(const std::string& name) :
		TemplatedParameteredObject<T>("CGSolverHelper", name,
			"Helper for connecting CGSolver to stencils. <br>"
			"This provides current flow/initial flow for further "
			"processing."),
		initFlow(true,false)
{
	ParameteredObject::_addInputSlot(in, "in",
		"original sequence input", "CImgList<T>");
	ParameteredObject::_addInputSlot(initFlow, "initFlow",
		"initial flow guess", "CImgList<T>");
	ParameteredObject::_addParameter(flowDimensions, "flowDimensions",
		"number of flow components for initialization", 2u);
	ParameteredObject::_addOutputSlot(sequence, "sequence",
		"original sequence output", "CImgList<T>");
	ParameteredObject::_addOutputSlot(flow, "flow",
		"current flow solution", "CImgList<T>");
	ParameteredObject::_addOutputSlot(self, "self",
		"self-pointer", "CGSolverHelper<T>*");
	ParameteredObject::_setTags("charon-flow;Solvers");
}

template <typename T>
void CGSolverHelper<T>::execute() {
	self() = this;
	if (initFlow.connected()) {
		flow().assign(initFlow());
	} else {
		flow().assign(flowDimensions(),
                        in()[0].width(),in()[0].height(),in()[0].depth(),
                        in()[0].spectrum()-1,T(0));
	}
	sequence().assign(in());
}

#endif /* _CGSOLVER_HELPER_HXX_ */

