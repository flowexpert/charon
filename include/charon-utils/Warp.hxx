/*  Copyright (C) 2009 Cornelius Ratsch

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
/** \file Warp.hxx
 *  Implementation of the parameter class Warp.
 *  \author Cornelius Ratsch
 *  \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried</a>
 *  \date 08.03.2010
 */

#ifndef _WARP_HXX_
#define _WARP_HXX_

#include "Warp.h"
#include <limits>

template<typename T>
Warp<T>::Warp(const std::string& name) :
	TemplatedParameteredObject<T> (
			"Warp", name,
			"Warping of images and image sequences<br><br>"
			"This module performs warping of single images or images sequences "
			"with a given flow. This is useful e.g. to check if a calculated "
			"optical flow explains the changes in a consecutive image pair."
			"<br><br>"
			"Usually, the flow information has one time step less than the "
			"input sequence. In this case, the first image will stay "
			"untouched. A message is printed to sout showing how much images "
			"have been ignored. If there are more time steps in the flow "
			"image, the last ones are ignored (also with message printed to "
			"sout).<br><br>"
			"If the flow is correct, the two resulting images "
			"(-&gt; image pair) "
			"of this warping step should look much more similar than before. "
			"The difference between the two output images is called residual. "
			"<br><br>"
			"The flow is multiplied with the weight parameter before the "
			"warping is actually performed, this way warping into the opposite "
			"direction (weight = -1) or warping half-way (weight = 0.5) "
			"is possible."
			) {
	ParameteredObject::_setTags("charon-utils;CImg") ;

	ParameteredObject::_addInputSlot(seqInput, "seqInput",
			"Image sequence", "CImgList<T>");
	ParameteredObject::_addInputSlot(flowInput, "flowInput",
			"Image flow", "CImgList<T>");
	ParameteredObject::_addInputSlot(interpolator, "interpolator",
			"Interpolator", "Interpolator<T>*");
	ParameteredObject::_addOutputSlot(out, "out",
			"Warped Images", "CImgList<T>");

	ParameteredObject::_addParameter(weight, "weight", "Flow weight", 1.f);
	ParameteredObject::_addParameter(warpToNan, "warpToNan", "warps to NaN, if motion exceeds boundaries", false);
}

template<typename T>
void Warp<T>::execute() {
	// check sizes
	const cimg_library::CImgList<T>& seq = seqInput();
	const cimg_library::CImgList<T>& flow = flowInput();
	cimg_library::CImgList<T>& warped = out();
	Interpolator<T>& interp = *interpolator();

	//unsigned int dn = seq.size();
	unsigned int dx = seq[0].width();
	unsigned int dy = seq[0].height();
	unsigned int dz = seq[0].depth();
	unsigned int dt = seq[0].spectrum();

	const bool is3D = (dz > 1);
	const double l = weight();

	if(flow.size() < 2 || (is3D && flow.size() < 3)) {
		throw std::runtime_error(
				"Given flow must contain at "
				"least 2 images (uv for 2D sequences)"
				"or at least 3 images (uvw for 3D sequences).");
	}

	warped.assign(seq);

	int ignore = dt - flow[0].spectrum();
	if (ignore < 0) {
		sout << "\tgiven flow has more time steps than sequence. "
			<< "Ignoring last ones." << std::endl;
		ignore = 0;
	}
	if (ignore > 0) {
		sout << "\tignoring first " << ignore << " image(s) of sequence "
			<< "(leaving them untouched)" << std::endl;
	}

	// continuous coordinates for interpolation (interpolator uses float)
	float xn, yn, zn ;
	T res ;
	int tf;
	cimglist_for(warped, i) { // for each color channel
		cimg_forC(warped[i], t) { // for each image in sequence
			tf = t - ignore;
			if(tf < 0)
				continue;

			cimg_forXYZ(warped[i],x,y,z) { // for each position within volume
				xn = (float)x+l*flow[0].atXYZC(x,y,z,tf);
				yn = (float)y+l*flow[1].atXYZC(x,y,z,tf);
				zn = (float)z;
				if(is3D) {
					zn += l*flow[2].atXYZC(x,y,z,tf);
				}
				if (!warpToNan) {
					res = interp.interpolate(seq[i], xn, yn, zn, t);

				} else if ((xn>=-2.f)         && (xn<0.f)             &&  (yn>=-2.f) && (yn<0.f)) {
					res = interp.interpolate(seq[i], -xn-1.f,     -yn-1.f, zn, t);
				} else if ((xn>=0.f)          && (xn<=(float)dx-1.f)  &&  (yn>=-2.f) && (yn<0.f)) {
					res = interp.interpolate(seq[i], xn,          -yn-1.f, zn, t);
				} else if ((xn>(float)dx-1.f) && (xn<=(float)dx+1.f)  &&  (yn>=-2.f) && (yn<0.f)) {
					res = interp.interpolate(seq[i], 2*dx-xn-1.f, -yn-1.f, zn, t);

				} else if ((xn>=-2.f)         && (xn<0.f)             &&  (yn>=0.f)  && (yn<=(float)dy-1.f)) {
					res = interp.interpolate(seq[i], -xn-1.f,      yn, zn, t);
				} else if ((xn>=0.f)          && (xn<=(float)dx-1.f)  &&  (yn>=0.f)  && (yn<=(float)dy-1.f)) {
					res = interp.interpolate(seq[i], xn,           yn, zn, t);
				} else if ((xn>(float)dx-1.f) && (xn<=(float)dx+1.f)  &&  (yn>=0.f)  && (yn<=(float)dy-1.f)) {
					res = interp.interpolate(seq[i], 2*dx-xn-1.f,  yn, zn, t);

				} else if ((xn>=-2.f)         && (xn<0.f)             &&  (yn>(float)dy-1.f)  && (yn<=(float)dy+1.f)) {
					res = interp.interpolate(seq[i], -xn-1.f,      2*dy-yn-1.f, zn, t);
				} else if ((xn>=0.f)          && (xn<=(float)dx-1.f)  &&  (yn>(float)dy-1.f)  && (yn<=(float)dy+1.f)) {
					res = interp.interpolate(seq[i], xn,           2*dy-yn-1.f, zn, t);
				} else if ((xn>(float)dx-1.f) && (xn<=(float)dx+1.f)  &&  (yn>(float)dy-1.f)  && (yn<=(float)dy+1.f)) {
					res = interp.interpolate(seq[i], 2*dx-xn-1.f,  2*dy-yn-1.f, zn, t);

				} else {
					res = std::numeric_limits<T>::quiet_NaN();
				}
				warped.atNXYZC(i,x,y,z,t) = res;
			}
		}
	}
}

#endif /* _WARP_HXX_ */

