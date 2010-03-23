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
}

template <typename T>
void FlowComparator<T>::execute() {
	ParameteredObject::execute();

	// shared assignment, no copying
	passthrough().assign(result(), true);

	sout << "\tmean endpoint error: " << getMeanEndpointError() << std::endl;
}

template <typename T>
double FlowComparator<T>::getMeanEndpointError() const {
	if (!result().is_sameNXYZC(groundtruth())) {
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__ << "\n\t";
		msg << "Dimensions of result and groundtruth do not match.";
		msg << "\n\t\tResult     : ";
		const cimg_library::CImgList<T>& res = result();
		assert(&res);
		const unsigned int& is = res.size();
		msg << is << "; ";
		if (is) {
			const unsigned int& iw = res[0].width();
			const unsigned int& ih = res[0].height();
			const unsigned int& id = res[0].depth();
			const unsigned int& it = res[0].spectrum();
			msg << iw << "; " << ih << "; " << id << "; " << it;
		}
		else
			msg << " (no content)";

		msg << "\n\t\tGroundTruth: ";
		const cimg_library::CImgList<T>& truth = groundtruth();
		assert(&truth);
		const unsigned int& gs = truth.size();
		msg << gs << "; ";
		if (gs) {
			const unsigned int& gw = truth[0].width();
			const unsigned int& gh = truth[0].height();
			const unsigned int& gd = truth[0].depth();
			const unsigned int& gt = truth[0].spectrum();
			msg << gw << "; " << gh << "; " << gd << "; " << gt;
		}
		else
			msg << " (no content)";
		throw std::invalid_argument(msg.str());
	}

	// perform calculations
	cimg_library::CImg<T> endpointError(result()[0]);
	endpointError.fill(T(0));
	cimglist_for(result(), kk) {
		endpointError += (result()[kk]-groundtruth()[kk]).sqr();
	}
	endpointError.sqrt();
	return endpointError.mean();;
}

#endif /* _FLOWCOMPARATOR_HXX_ */



