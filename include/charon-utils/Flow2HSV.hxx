/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/// \file Flow2HSV.hxx
/// Implementation of the parameter class Flow2HSV.
/// \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
///     Jens-Malte Gottfried</a>
/// \date 26.11.2010

#ifndef FlOW_TO_HSV_HXX_
#define FlOW_TO_HSV_HXX_

#include <cassert>
#include <vector>
#include <algorithm>
#include <charon-core/ParameteredObject.hxx>
#include "Flow2HSV.h"

template<typename T>
Flow2HSV<T>::Flow2HSV(const std::string& name) :
		TemplatedParameteredObject<T>("Flow2HSV", name,
				"Convert Flow into HSV Color Representation")
{
	ParameteredObject::_addParameter(
			scaleChannel, "scaleChannel",
			"select how image is scaled: 0 saturation, 1 value, 2 none");
	ParameteredObject::_addParameter(
			normalizationFactor, "normalizationFactor",
			"normalization factor (0=auto)", "T");
	ParameteredObject::_addInputSlot(
			flow, "flow", "flow input", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			out, "out", "hsv representation output [t](x,y,z,c)",
			"CImgList<T>");
}

template<typename T>
void Flow2HSV<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	// copy input image for manipulations
	const cimg_library::CImgList<T>& i = flow();
	cimg_library::CImgList<T>& o = out();

	if(i.size() < 2)
		throw std::invalid_argument(
				"FlowToHSVOp::flow must be at least of dim 2");

	if(i.size() > 2) {
		sout << "\tdiscard additional dimensions" << std::endl;
	}
	const int& sc = scaleChannel();
	if(sc < 0 || sc > 2)
		throw std::invalid_argument(
				"scale channel has to be in {0;1;2}!");

	cimg_library::CImgList<double> interm(
			2u,i[0].width(),i[0].height(),i[0].depth(),i[0].spectrum());
	o.assign(3u,i[0].width(),i[0].height(),i[0].depth(),i[0].spectrum());

	// calculate lenght and angle of the flow vector,
	// store intermediate result in interm[0] (lenght) and interm[1] (angle)
	cimg_forXYZC(i[0],x,y,z,t) {
		const double u = i(0,x,y,z,t);
		const double v = i(1,x,y,z,t);
		double len = std::sqrt(std::pow(u,2)+std::pow(v,2));
		double phi = std::atan2(u, v);
		interm(0,x,y,z,t) = len;
		interm(1,x,y,z,t) = phi;
	}

	// normalize lenth (max lenth will be set to 1)
	double maxLen = interm[0].max();
	// use given normalization factor, if not zero
	if(normalizationFactor() > 0)
		maxLen = normalizationFactor();
	interm[0] /= maxLen;

	// in argos code, the following calculation was applied to len:
	// len = (len+0.05)/1.05 --> why?

	// use lenght and angle for HSV values, respect value of "scaleChannels"
	// convert HSV to RGB and store final result in out
	cimg_forXYZC(i[0],x,y,z,t) {
		const double& len = interm(0,x,y,z,t);
		const double& phi = interm(1,x,y,z,t);
		// here conversion from double to T is performed
		o(t,x,y,z,0) = phi;
		if(sc==0)o(t,x,y,z,1) = len; // length is saturation
		if(sc==1)o(t,x,y,z,2) = len; // length is value
	}
	cimglist_for(o,kk) {
		o[kk].HSVtoRGB();
	}
}

#endif // FlOW_TO_HSV_HXX_
