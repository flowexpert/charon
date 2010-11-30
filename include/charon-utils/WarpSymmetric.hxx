/*  Copyright (C) 2010 Jens-Malte Gottfried

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
/** \file WarpSymmetric.hxx
 *  Implementation of the parameter class WarpSymmetric.
 *  \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried</a>
 *  \date 12.11.2010
 */

#ifndef _WARP_SYMMETRIC_HXX_
#define _WARP_SYMMETRIC_HXX_

#include "WarpSymmetric.h"

template<typename T>
WarpSymmetric<T>::WarpSymmetric(const std::string& name) :
	TemplatedParameteredObject<T> (
			"WarpSymmetric", name,
			"Symmetric warping of images pairs<br><br>"
			"This module performs warping of two consecutive images "
			"(image pair), and works similarly as the module Warp. "
			"The difference is that warping is done in a symmetric way, "
			"i.e. the first image is warped towards the second one "
			"(using half the negative flow), the second one is warped "
			"towards the first one (using half the flow)."
			"This avoids artifacts resulting when compared the warped, "
			"second image with the original one, as done usually "
			"e.g. using the module Warp.<br><br>"
			"Since the negative flow is not equal to the inverted flow, "
			"the inversion is approximated by warping the flow with itself. "
			"To get symmetric processing for both images, the flow is also "
			"warped half way (forward for the first image and backward "
			"for the second one).<br><br>"
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
	ParameteredObject::_addInputSlot(seqInput, "seqInput",
			"Image sequence", "CImgList<T>");
	ParameteredObject::_addInputSlot(flowInput, "flowInput",
			"Image flow", "CImgList<T>");
	ParameteredObject::_addInputSlot(interpolator, "interpolator",
			"Interpolator", "Interpolator<T>*");
	ParameteredObject::_addOutputSlot(out, "out",
			"Warped Images", "CImgList<T>");

	ParameteredObject::_addParameter(weight, "weight", "Flow weight", 1.f);
}

template<typename T>
void WarpSymmetric<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	// check sizes
	const cimg_library::CImgList<T>& seq = seqInput();
	const cimg_library::CImgList<T>& flow = flowInput();
	cimg_library::CImgList<T>& warped = out();
	Interpolator<T>& interp = *interpolator();

	assert(seq.size() >= 1);
	unsigned int dz = seq[0].depth();
	unsigned int dt = seq[0].spectrum();
	if(dt != 2u) {
		throw std::runtime_error(
				"Only image pairs are allowed!");
	}

	assert(flow.size() > 0);
	assert(flow[0].spectrum() == 1u); // only one time step is used
	assert(flow.is_sameXYZ(seq));

	const bool is3D = (dz > 1);
	const double l = weight();

	if(flow.size() < 2 || (is3D && flow.size() < 3)) {
		throw std::runtime_error(
				"Given flow must contain at "
				"least 2 images (uv for 2D sequences)"
				"or at least 3 images (uvw for 3D sequences).");
	}

	warped.assign(seq);

	// allocate variables used in loop
	cimg_library::CImg<T> i1, i2;
	float xf1, yf1, zf1;
	float xf2, yf2, zf2;
	float xn1, yn1, zn1;
	float xn2, yn2, zn2;
	T flowX1, flowY1, flowZ1;
	T flowX2, flowY2, flowZ2;
	const float wf1 = +0.5f  , wf2 = -0.5f;
	const float wn1 = -0.5f*l, wn2 = +0.5f*l;

	cimglist_for(seq, kk) {
		// get references of current images
		i1 = seq[kk].get_shared_channel(0u);
		i2 = seq[kk].get_shared_channel(1u);

		cimg_forXYZ(seq[kk],x,y,z) {
			// calculate positions where to evaluate the flow
			xf1=x+wf1*flow(0u,x,y,z);   yf1=y+wf1*flow(1u,x,y,z);   zf1=z;
			xf2=x+wf2*flow(0u,x,y,z);   yf2=y+wf2*flow(1u,x,y,z);   zf2=z;

			// get interpolated flow values at calculated positions
			flowX1 = interp.interpolate(flow[0u],xf1,yf1,zf1);
			flowY1 = interp.interpolate(flow[1u],xf1,yf1,zf1);

			flowX2 = interp.interpolate(flow[0u],xf2,yf2,zf2);
			flowY2 = interp.interpolate(flow[1u],xf2,yf2,zf2);

			// calculate positions where to read the image
			xn1 = x+wn1*flowX1;   yn1 = y+wn1*flowY1;   zn1 = z;
			xn2 = x+wn2*flowX2;   yn2 = y+wn2*flowY2;   zn2 = z;

			if(is3D) {
				// same processing for z dimension
				zf1 += wf1*flow(2u,x,y,z);
				zf2 += wf2*flow(2u,x,y,z);
				flowZ1 = interp.interpolate(flow[2u],xf1,yf1,zf1);
				flowZ2 = interp.interpolate(flow[2u],xf2,yf2,zf2);
				zn1 += wn1*flowZ1;
				zn2 += wn2*flowZ2;
			}

			// get interpolated image values (= warping)
			warped(kk,x,y,z,0u) = interp.interpolate(i1,xn1,yn1,zn1);
			warped(kk,x,y,z,1u) = interp.interpolate(i2,xn2,yn2,zn2);
		}
	}
}

#endif // _WARP_SYMMETRIC_HXX_

