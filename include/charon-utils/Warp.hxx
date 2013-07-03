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
			"optical flow explains the changes in a consecutive image pair "
			"or to remove image distortions after camera calibration."
			"<br><br>"
			"(This section is only valid if uniformWarp is set to false)<br>"
			"Usually, the flow information has one time step less than the "
			"input sequence. In this case, the first image will stay "
			"untouched. A message is printed to sout showing how much images "
			"have been ignored. If there are more time steps in the flow "
			"image, the last ones are ignored (also with message printed to "
			"sout).<br>"
			"If the z dimension of the input is > 1 the data is treated as a "
			"volume and the corresponding warping flow field must be 3dimensional!<br>"
			"<br><br>"
			"The flow is multiplied with the weight parameter before the "
			"warping is actually performed, this way warping into the opposite "
			"direction (weight = -1) or warping half-way (weight = 0.5) "
			"is possible.<br><br>"
			"When this plugin is used e.g. for image rectification, set the "
			"uniformWarp parameter to true to deactivate all complex dimension handling"
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
	ParameteredObject::_addParameter(uniformWarp, "uniformWarp", 
		"treat all images in seqInput as 2D and warp them with exactly the same flow field<br>"
		"flowInput will be treated as having the dimension w x h x 1 x 1 x 2 (CimgList with 2 entries)",
		false) ;
}

template<typename T>
void Warp<T>::execute() {
	// check sizes
	if(seqInput().width() == 0)
	{	ParameteredObject::raise("Input sequence is empty!") ;	}
	
	const cimg_library::CImgList<T>& seq = seqInput();
	const cimg_library::CImgList<T>& flow = flowInput();
	cimg_library::CImgList<T>& warped = out();

	if(!interpolator())
	{	ParameteredObject::raise("Interpolator is null-pointer") ;	}
	Interpolator<T>& interp = *interpolator();

	//unsigned int dn = seq.size();
	unsigned int dx = seq[0].width();
	unsigned int dy = seq[0].height();
	unsigned int dz = seq[0].depth();
	unsigned int dt = seq[0].spectrum();
	const double l = weight();

	if(!uniformWarp())
	{
		const bool is3D = (dz > 1);

		if(flow.size() < 2 || (is3D && flow.size() < 3)) {
			ParameteredObject::raise(
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
						warped.atNXYZC(i,x,y,z,t) = interp.interpolate(seq[i], xn, yn, zn, t);
						continue;
					}

					if ((xn>=0) && (xn<=dx-1) && (yn>=0) && (yn<=dy-1)) {
						res = interp.interpolate(seq[i], xn, yn, zn, t);
					} else {
						res = std::numeric_limits<T>::quiet_NaN();
					}
					warped.atNXYZC(i,x,y,z,t) = res;
				}
			}
		}
	}
	else
	{
		if(flow.width() < 2)
		{	ParameteredObject::raise("Input flow field c dimension is less than 2!") ;}
		if(!flow[0].is_sameXY(seq[0]) || !flow[1].is_sameXY(seq[0]))
		{
			ParameteredObject::raise("Image sequence and warping flow field have different XY sizes!");
		}
		
		warped.assign(seq);
		float xn, yn ;
		T res ;
		cimglist_for(warped, i) { // for each color channel
			cimg_forZC(warped[i], z,c) { // for each image in sequence

				cimg_forXY(warped[i],x,y) { // for each position within volume
					xn = (float)x+l*flow[0].atXYZC(x,y,z,c);
					yn = (float)y+l*flow[1].atXYZC(x,y,z,c);
					if (!warpToNan) {
						warped.atNXYZC(i,x,y,z,c) = interp.interpolate(seq[i], xn, yn, z, c);
						continue;
					}

					if ((xn>=0) && (xn<=dx-1) && (yn>=0) && (yn<=dy-1)) {
						res = interp.interpolate(seq[i], xn, yn, z, c);
					} else {
						res = std::numeric_limits<T>::quiet_NaN();
					}
					warped.atNXYZC(i,x,y,z,c) = res;
				}
			}
		}
	}
}

#endif /* _WARP_HXX_ */

