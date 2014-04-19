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
/** \file IteratorHelper.hxx
 *  Implementation of the parameter class IteratorHelper.
 *  \author Jens-Malte Gottfried <jmgottfried@web.de>
 *  \date 01.02.2010
 */

#ifndef _ITERATOR_HELPER_HXX_
#define _ITERATOR_HELPER_HXX_

#include "IteratorHelper.h"
#include <ImgTool.h>
#include <sstream>

template <typename T>
IteratorHelper<T>::IteratorHelper(const std::string& name) :
		TemplatedParameteredObject<T>("IteratorHelper", name,
			"Helper for iterative image processing algorithms.<br>"
			"This provides current values/initial values for further "
			"processing.<br>"
			"If an inital flow is provided via the optional initFlow "
			"input slot, it is copied to the flow output in the "
			"first iteration. If left unconnected, an initial zero flow "
			"is generated matching the input sequence x/y/z dimensions "
			"and using the flowFrames (as t) and flowDimensions (as v)."
			"Auto detection of flowFrames means one frame less than "
			"the input sequence."),
		initFlow(true, false),
		count(0u),
		countAll(0u)
{
	ParameteredObject::_addInputSlot(in, "in",
		"original sequence input", "CImgList<T>");
	ParameteredObject::_addInputSlot(initFlow, "initFlow",
		"initial flow guess", "CImgList<T>");
	ParameteredObject::_addParameter(flowDimensions, "flowDimensions",
		"number of flow components for initialization", 2u);
	ParameteredObject::_addParameter(flowFrames, "flowFrames",
		"number of frames for initial flow ( use -1 for auto detection )", -1);
	ParameteredObject::_addOutputSlot(sequence, "sequence",
		"original sequence output", "CImgList<T>");
	ParameteredObject::_addOutputSlot(flow, "flow",
		"current flow solution", "CImgList<T>");
	ParameteredObject::_addOutputSlot(residual, "residual",
		"current residual energy", "CImgList<T>");
	ParameteredObject::_addOutputSlot(count, "count",
		"iteration counter (resetted on iteration start, e.g. inner loop)");
	ParameteredObject::_addOutputSlot(countAll, "countAll",
		"iteration counter (not resetted, increasing only)");
	ParameteredObject::_addOutputSlot(self, "self",
		"self-pointer", "IteratorHelper<T>*");

	ParameteredObject::_setTags("charon-flow;Iterators");
}

template <typename T>
void IteratorHelper<T>::execute() {
	self() = this;

	// first initialization
	// later updated by iterator
	if(sequence().is_empty())
		reset();
}

template <typename T>
void IteratorHelper<T>::reset() {
	count() = 0;
	const cimg_library::CImgList<T>& seqIn = in();

	if (seqIn.size() <= 0) {
		ParameteredObject::raise("input sequence is empty!");
	}

	// copy input sequence
	sequence().assign(seqIn);

	// copy input flow if present, otherwise create dummy flow with zeros
	const unsigned int df = flowDimensions();
	const unsigned int dt = flowFrames()<0?seqIn[0].spectrum()-1:flowFrames();
	if(initFlow.connected()) {
		sout << "(DD) initFlow given, ignoring flowDimensions and flowFrames."
			 << std::endl;
		if (initFlow().size() <= 0 || !initFlow()[0].is_sameXYZ(seqIn[0])) {
			sout << "(WW) initial flow is empty or does not match the "
				"dimensions of the input image" << std::endl;
		}
		flow().assign(initFlow());
	}
	else {
		if (dt==0 || df==0) {
			flow().assign();
		}
		else {
			flow().assign(
				df,seqIn[0].width(),seqIn[0].height(),seqIn[0].depth(),dt,
				T(0));
		}
	}

	// assign initial residual
	residual().assign(
		1,seqIn[0].width(),seqIn[0].height(),seqIn[0].depth(),dt,T(0));
}

template <typename T>
void IteratorHelper<T>::update(bool c) {
	sout << "(II) Updating " << this->getClassName()
		<< " \"" << this->getName() << "\"" << std::endl;

	if (c) {
		count()++;
		countAll()++;
	}
}

#endif /* _ITERATOR_HELPER_HXX_ */

