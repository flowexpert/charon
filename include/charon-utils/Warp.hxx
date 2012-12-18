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
	const cimg_library::CImgList<T>& seq = seqInput();
	const cimg_library::CImgList<T>& flow = flowInput();
	Interpolator<T>& interp = *interpolator();

	unsigned int dn = seq.size();
	unsigned int dx = seq[0].width();
	unsigned int dy = seq[0].height();
	unsigned int dz = seq[0].depth();

	const bool is3D = (dz > 1);
	const double l = weight();

	if(flow.size() < 2 || (is3D && flow.size() < 3)) {
		throw std::runtime_error(
				"Given flow must contain at "
				"least 2 images (uv for 2D sequences)"
				"or at least 3 images (uvw for 3D sequences).");
	}

	out().assign( seq );

	int ignore = dn;
	sout << "\tignoring first " << ignore << " image(s) of sequence "
	     << "(leaving them untouched)" << std::endl;

	T xn, yn, zn ;
	T res ;
	int tf;
	cimglist_for(out(), i) { // for each color channel
		cimg_forC(out()[i], t) { // for each image in sequence
			tf = t - ignore;
			if(tf < 0)
				continue;

			cimg_forXYZ(out()[i],x,y,z) { // for each position within volume
				xn = (T)x+l*flow[0].atXYZC(x,y,z,tf);
				yn = (T)y+l*flow[1].atXYZC(x,y,z,tf);
				zn = (T)z;
				if(is3D) {
					zn += l*flow[2].atXYZC(x,y,z,tf);
				}
				if (!warpToNan ||
				    ((xn>=0.0) && (xn<=(double)dx-1.0) &&
				     (yn>=0.0) && (yn<=(double)dy-1.0) &&
				     (zn>=0.0) && (zn<=(double)dz-1.0))) {
					res = interp.interpolate(seq[i], xn, yn, zn, t);
				} else {
					res = NAN;
				}
				out().atNXYZC(i,x,y,z,t) = res;
			}
		}
	}
}

#endif /* _WARP_HXX_ */

