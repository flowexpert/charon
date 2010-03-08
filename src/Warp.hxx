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
 *  \date 08.03.2010
 */

#ifndef _WARP_HXX_
#define _WARP_HXX_

#include "Warp.h"

template<typename T>
Warp<T>::Warp(const std::string& name) :
	TemplatedParameteredObject<T> ("Warp", name, "Warps an image") {
	ParameteredObject::_addInputSlot(image_sequence, "image_sequence",
			"Image sequence", "CImgList<T>");
	ParameteredObject::_addInputSlot(flow_sequence, "flow_sequence",
			"Image flow", "CImgList<T>");
	ParameteredObject::_addInputSlot(interpolator, "interpolator",
			"Interpolator", "Interpolator<T>*");
	ParameteredObject::_addOutputSlot(warped_image, "warped_image",
			"Warped Image", "CImgList<T>");

	ParameteredObject::_addParameter(weight, "weight", "Flow weight", 1.);
}

template<typename T>
void Warp<T>::execute() {
	ParameteredObject::execute();

	//sicherstellen, dass flussbilder gleichgroß wie die anderen bilder

	int size = this->image_sequence().size();
	int width = this->image_sequence()[0].width();
	int height = this->image_sequence()[0].height();
	int depth = this->image_sequence()[0].depth();
	int spectrum = this->image_sequence()[0].spectrum();

	int spectrum_increment = 0;
	if (spectrum == this->flow_sequence()[0].spectrum() + 1) {
		sout << "Ignoring first image in sequence." << std::endl;
		spectrum_increment = 1;
	}
	this->warped_image().assign(size, width, height, depth, spectrum
			- spectrum_increment);

	const bool is3D = (this->flow_sequence().size() == 3);
cimglist_for(this->warped_image(), i) {
	cimg_forXYZC(this->warped_image()[i],x,y,z, t) {
		T x_new = x + this->weight() * this->flow_sequence()[0](x, y, z, t);
		T y_new = y + this->weight() * this->flow_sequence()[1](x, y, z, t);
		T z_new = z;
		if(is3D) {
			z_new += this->weight() * this->flow_sequence()[2](x, y, z, t);
		}

		T res = this->interpolator()->interpolate(
				this->image_sequence()[i], (float) x_new,
				(float) y_new, (float) z_new, t + spectrum_increment);

		this->warped_image()[i](x, y, z, t) = res;
	}
}

}

#endif /* _WARP_HXX_ */

