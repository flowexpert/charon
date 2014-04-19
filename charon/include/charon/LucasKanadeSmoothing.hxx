/*  Copyright (C) 2012 Helen Morrison

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
/** \file LucasKanadeSmoothing.hxx
 *  Implementation of the parameter class LucasKanadeSmoothing.
 *  \author Helen Morrison
 *  \date 16.04.2012
 */

#ifndef _LUCASKANADESMOOTHING_HXX_
#define _LUCASKANADESMOOTHING_HXX_

#include "LucasKanadeSmoothing.h"
#include <charon-utils/Convolution.h>

template <typename T>
LucasKanadeSmoothing<T>::LucasKanadeSmoothing(const std::string& name) :
		TemplatedParameteredObject<T>("lucas_kanade_smoothing", name,
			"Calculate locally smoothed derivatives<br><br>"
			"This plugin serves as a preparation step for a Lucas & Kanade"
			"motion model by locally smoothing the products of the derivatives"
			"which will be needed"),
		smoothMask("0.1875;0.625;0.1875")
{
	ParameteredObject::_addParameter (
			smoothMask, "smoothMask",
			"linear 1D filter mask",
			"T_list");
	ParameteredObject::_addInputSlot(
			dx, "dx", "derivative wrt x", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			dy, "dy", "derivative wrt y", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			dt, "dt", "derivative wrt t", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			IxIx, "IxIx", "<I_x * I_x>", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			IxIy, "IxIy", "<I_x * I_y>", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			IyIy, "IyIy", "<I_y * I_y>", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			IxIt, "IxIt", "<I_x * I_t>", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			IyIt, "IyIt", "<I_y * I_t>", "CImgList<T>");
}

template <typename T>
void LucasKanadeSmoothing<T>::execute() {
	sout << "\tcalculating products:" << std::endl;

	IxIx().assign(this->dx());
	cimglist_for(IxIx(), kk){
		cimg_forXYZC(IxIx()[kk],x,y,z,t){
			IxIx()(kk,x,y,z,t) = this->dx()(kk,x,y,z,t)*this->dx()(kk,x,y,z,t);
		}
	}

	IxIy().assign(this->dx());
	cimglist_for(IxIy(), kk){
		cimg_forXYZC(IxIy()[kk],x,y,z,t){
			IxIy()(kk,x,y,z,t) = this->dx()(kk,x,y,z,t)*this->dy()(kk,x,y,z,t);
		}
	}

	IyIy().assign(this->dy());
	cimglist_for(IyIy(), kk){
		cimg_forXYZC(IyIy()[kk],x,y,z,t){
			IyIy()(kk,x,y,z,t) = this->dy()(kk,x,y,z,t)*this->dy()(kk,x,y,z,t);
		}
	}

	IxIt().assign(this->dx());
	cimglist_for(IxIt(), kk){
		cimg_forXYZC(IxIt()[kk],x,y,z,t){
			IxIt()(kk,x,y,z,t) = this->dx()(kk,x,y,z,t)*this->dt()(kk,x,y,z,t);
		}
	}

	IyIt().assign(this->dy());
	cimglist_for(IyIt(), kk){
		cimg_forXYZC(IyIt()[kk],x,y,z,t){
			IyIt()(kk,x,y,z,t) = this->dy()(kk,x,y,z,t)*this->dt()(kk,x,y,z,t);
		}
	}

	sout << "\tcalculating smoothing:" << std::endl;

	cimg_library::CImg<T> smoothX(smoothMask().size(),1u,1u,1u);
	cimg_library::CImg<T> smoothY(1u,smoothMask().size(),1u,1u);

	cimg_forX(smoothX, x) smoothX(x,0,0,0) = smoothMask()[x];
	cimg_forY(smoothY, y) smoothY(0,y,0,0) = smoothMask()[y];

	cimglist_for(IxIx(),kk) {
		Convolution::convolve(smoothX, IxIx()[kk],IxIx()[kk]);
		Convolution::convolve(smoothX, IxIy()[kk],IxIy()[kk]);
		Convolution::convolve(smoothX, IyIy()[kk],IyIy()[kk]);
		Convolution::convolve(smoothX, IxIt()[kk],IxIt()[kk]);
		Convolution::convolve(smoothX, IyIt()[kk],IyIt()[kk]);
	}

	cimglist_for(IxIx(),kk) {
		Convolution::convolve(smoothY, IxIx()[kk],IxIx()[kk]);
		Convolution::convolve(smoothY, IxIy()[kk],IxIy()[kk]);
		Convolution::convolve(smoothY, IyIy()[kk],IyIy()[kk]);
		Convolution::convolve(smoothY, IxIt()[kk],IxIt()[kk]);
		Convolution::convolve(smoothY, IyIt()[kk],IyIt()[kk]);
	}

}
#endif /* _LUCASKANADESMOOTHING_HXX_ */

