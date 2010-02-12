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
/** \file FlowComparator.hxx
 *  Implementation of the parameter class FlowComparator.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 11.02.2010
 */

#ifndef _FLOWCOMPARATOR_HXX_
#define _FLOWCOMPARATOR_HXX_

#include "FlowComparator.h"

template <typename T>
FlowComparator<T>::FlowComparator(const std::string& name) :
		TemplatedParameteredObject<T>("FlowComparator", name,
			"Compare results of some flow calculation with the underlaying "
			"ground truth.<br> Calculation results are passed through, e.g. "
			"to be saved later.<br> This may avoid multiple calculations.") {
	ParameteredObject::_addInputSlot(result, "result",
		"flow result from some flow estimation", "CImgList<T>");
	ParameteredObject::_addInputSlot(groundtruth, "groundtruth",
		"underlaying ground truth", "CImgList<T>");
	ParameteredObject::_addOutputSlot(passthrough, "passthrough",
		"result passthrough", "CImgList<T>");

	_addFunction(FlowComparator<T>::getMeanEndpointError);
}

template <typename T>
void FlowComparator<T>::execute() {
	ParameteredObject::execute();

	// shared assignment, no copying
	passthrough().assign(result(), true);

	if (!result().is_sameNXYZC(groundtruth())) {
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__ << "\n\t";
		msg << "Dimensions of result and groundtruth do not match.";
		msg << "\n\t\tResult     : "
				<< result()[0].width() << "; "
				<< result()[0].height() << "; "
				<< result()[0].depth() << "; "
				<< result()[0].spectrum() << "; "
				<< result().size();
		msg << "\n\t\tGroundTruth: "
				<< groundtruth()[0].width() << "; "
				<< groundtruth()[0].height() << "; "
				<< groundtruth()[0].depth() << "; "
				<< groundtruth()[0].spectrum() << "; "
				<< groundtruth().size();
		throw std::invalid_argument(msg.str());
	}

	// perform calculations
	_meanEndpointError = 0;
	cimg_library::CImg<T> endpointError(result()[0]);
	endpointError.fill(T(0));
	cimglist_for(result(), kk) {
		endpointError += (result()[kk]-groundtruth()[kk]).sqr();
	}
	endpointError.sqrt();
	_meanEndpointError = endpointError.mean();

	sout << "\tmean endpoint error: " << _meanEndpointError << std::endl;
}

template <typename T>
T FlowComparator<T>::getMeanEndpointError() const {
	return _meanEndpointError;
}

#endif /* _FLOWCOMPARATOR_HXX_ */



