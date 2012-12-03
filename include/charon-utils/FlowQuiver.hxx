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
/// \file FlowQuiver.hxx
/// Implementation of the parameter class FlowQuiver.
/// \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
///     Jens-Malte Gottfried</a>
/// \date 27.11.2010

#ifndef FLOW_QUIVER_HXX_
#define FLOW_QUIVER_HXX_

#include "FlowQuiver.h"
#include <charon-core/ParameteredObject.hxx>

template<typename T>
FlowQuiver<T>::FlowQuiver(const std::string& name) :
		TemplatedParameteredObject<T>(
				"FlowQuiver", name,
				"Draw overlay with arrows representing the optical flow"),
		color("255;0;0")
{
	ParameteredObject::_setTags("charon-utils;CImg") ;

	ParameteredObject::_addParameter(
			length, "length", "quiver lenght", 5.f);
	ParameteredObject::_addParameter(
			sampling, "sampling", "quiver sampling", 16u);
	ParameteredObject::_addParameter(
			color, "color", "quiver color");
	ParameteredObject::_addInputSlot(
			in, "in", "image data input (gray values) [t](x,y,z,1)",
			"CImgList<T>");
	ParameteredObject::_addInputSlot(
			flow, "flow", "flow input [u/v/...](x,y,z,t)",
			"CImgList<T>");
	ParameteredObject::_addOutputSlot(
			out, "out", "RGB data output [t](x,y,z,c)",
			"CImgList<T>");
}

template<typename T>
void FlowQuiver<T>::execute() {
	if(color.size() != 3u)
		throw std::invalid_argument(
				"FlowQuiver: need 3 color components");
	int c[3];
	for(unsigned int ii=0; ii<3u;ii++) {
		c[ii] = color[ii];
		if (c[ii] > 255)
			throw std::invalid_argument(
					"FlowQuiver: color components have to be in [0-255]");
	}

	const cimg_library::CImgList<T>& f = flow();
	if (f.size() < 2)
		throw std::invalid_argument(
				"FlowQuiver: At least 2 flow components required!");

	cimg_library::CImgList<T>& o = out();
	o.assign(in());
	if (f[0].spectrum() != (int) o.size() || !f.is_sameXYZ(o))
		throw std::invalid_argument(
				"FlowQuiver: Dimensions of flow and in do not match!");

	cimg_library::CImg<T> curFlow(o[0].width(),o[0].height(),o[0].depth(),2u);
	const unsigned int& sam = sampling();
	const float& len = length();
	cimglist_for(o,t) {
		curFlow = f[0].get_channel(t).get_append(f[1].get_channel(t),'c');
		o.at(t).draw_quiver(curFlow, c, 1.0, sam, len);
	}
}
#endif // FLOW_QUIVER_HXX_

