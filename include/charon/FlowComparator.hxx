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
#include <iomanip>

template <typename T>
FlowComparator<T>::FlowComparator(const std::string& name) :
		TemplatedParameteredObject<T>("FlowComparator", name,
			"Compare results of some flow calculation with the underlaying "
			"ground truth.<br> Calculation results are passed through, e.g. "
			"to be saved later.<br> This may avoid multiple calculations."),
	mask(true,false) // optional
{
	ParameteredObject::_addInputSlot(result, "result",
		"flow result from some flow estimation", "CImgList<T>");
	ParameteredObject::_addInputSlot(groundtruth, "groundtruth",
		"underlaying ground truth", "CImgList<T>");
	ParameteredObject::_addInputSlot(mask, "mask",
		"mask for weigthed mean (validity of gt or result)", "CImgList<T>");
}

template <typename T>
void FlowComparator<T>::execute() {
	sout << "     \tmean endpoint error: <b>"
		<< std::fixed << getMeanEndpointError() << "</b>" << std::endl;
}

template <typename T>
double FlowComparator<T>::getMeanEndpointError()  {
	if (!result().is_sameNXYZC(groundtruth())) {
		std::ostringstream msg;
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
		ParameteredObject::raise(msg.str());
	}
	if (result().size() < 1) {
		ParameteredObject::raise("empty result list");
	}

	// perform calculations
	const cimg_library::CImgList<T> &res=result(), &gt=groundtruth();
	cimg_library::CImg<T> epe(res[0]);
	epe.fill(T(0));
	cimglist_for(res, kk) {
		epe += (res[kk]-gt[kk]).sqr();
	}
	epe.sqrt();
	if (mask.connected()) {
		sout << "(II) \tusing weighted mean" << std::endl;
		if (mask().size() < 1) {
			ParameteredObject::raise("emty mask list");
		}
		const cimg_library::CImg<T> &m = mask()[0];
		epe.mul(m);
		return (epe.sum() / m.sum());
	}
	return epe.mean();;
}

#endif /* _FLOWCOMPARATOR_HXX_ */



