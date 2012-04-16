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
/// @file LucasKanade.hxx
/// implements class LucasKanade
/// @author <a href="mailto:helen.morrison@iwr.uni-heidelberg.de">
///		Helen Morrison</a>
/// @date 13.04.2012

#ifndef _MotionModels_LucasKanade_hxx_
#define _MotionModels_LucasKanade_hxx_

#include "LucasKanade.h"
#include <charon-utils/Convolution.h>
template<class T>
std::set<std::string>& MotionModels::LucasKanade<T>::getUnknowns()
{
	this->unknowns.insert("a1");
	this->unknowns.insert("a2");
	if (dz.connected())
		this->unknowns.insert("a3");
	return this->unknowns;
}

template<class T>
void MotionModels::LucasKanade<T>::compute(
		const Point4D<int>& p, const int& v,
		std::map<std::string, T>& term, T& rhs,
		const std::string& unknown)
{
	static const bool is3D = dz.connected();
	if(!is3D)
		assert(p.z == 0u); // 2D only

	cimg_library::CImgList<T> smooth_dx;
	cimg_library::CImgList<T> smooth_dy;
	cimg_library::CImgList<T> smooth_dz;
	cimg_library::CImgList<T> smooth_dt;

	cimg_library::CImg<T> smoothX(smoothMask().size(),1u,1u,1u);
	cimg_library::CImg<T> smoothY(1u,smoothMask().size(),1u,1u);
	cimg_library::CImg<T> smoothTZ(1u,1u,smoothMask().size(),1u);

	cimg_forX(smoothX, x) smoothX(x,0,0,0) = smoothMask()[x];
	cimg_forY(smoothY, y) smoothY(0,y,0,0) = smoothMask()[y];
	cimg_forZ(smoothTZ, z) smoothTZ(0,0,z,0) = smoothMask()[z];

	smooth_dx.assign(this->dx());
	cimglist_for(smooth_dx,kk) {
		Convolution::convolve(smoothX, smooth_dx[kk],smooth_dx[kk]);
	}

	smooth_dy.assign(this->dy());
	cimglist_for(smooth_dy,kk) {
		Convolution::convolve(smoothY, smooth_dy[kk],smooth_dy[kk]);
	}

	if(dz.connected()){
		smooth_dz.assign(this->dz());
		cimglist_for(smooth_dz,kk) {
			Convolution::convolve(smoothTZ, smooth_dz[kk],smooth_dz[kk]);
		}
	}

	smooth_dt.assign(this->dt());
	// since convolution along dim4 does not work, we have
	// to switch dim3 and dim4 in temporary CImg instance.
	cimg_library::CImg<T> temp(
			this->dt()[0].width(),
			this->dt()[0].height(),
			this->dt()[0].spectrum(),
			this->dt()[0].depth());
	cimglist_for(this->dt(),kk) {
		cimg_forXYZC(this->dt()[kk],x,y,z,t)
			temp(x,y,t,z) = this->dt()(kk,x,y,z,t);
		Convolution::convolve(smoothTZ,temp,temp);
		cimg_forXYZC(this->dt()[kk],x,y,z,t)
			smooth_dt(kk,x,y,z,t) = temp(x,y,t,z);
	}

	const T& iX = smooth_dx(v, p.x, p.y, p.z, p.t);
	const T& iY = smooth_dy(v, p.x, p.y, p.z, p.t);
	const T& iZ = dz.connected() ? smooth_dz(v, p.x, p.y, p.z, p.t) : T(0);
	const T& iT = smooth_dt(v, p.x, p.y, p.z, p.t);
	T factor = T(1);

	// multiply with derivative wrt unkown, if any unknown is given
	if (unknown.length()) {
		if (unknown == "a1")
			factor = iX;
		else if (unknown == "a2")
			factor = iY;
		else if (is3D && unknown == "a3")
			factor = iZ;
		else {
			std::ostringstream msg;
			msg << __FILE__ << ":" << __LINE__ << std::endl;
			msg << "\tInvalid Unknown given!" << std::endl;
			msg << "\tGiven unknown: \"" << unknown << "\"";
			throw std::out_of_range(msg.str().c_str());
		}
	}

	// calculate values to return
	term["a1"] += factor * iX;
	term["a2"] += factor * iY;
	if (is3D)
		term["a3"] += factor * iZ;
	rhs -= factor * iT;
}

template<class T>
MotionModels::LucasKanade<T>::LucasKanade(const std::string& name) :
		MotionModel<T>::MotionModel("motionmodels_lucaskanade", name),
		smoothMask("0.1875;0.625;0.1875"),
		dz(true, false)  // dz is optional, for handling 3D flows
{
	_addInputSlot(dx, "dx", "derivation in x", "CImgList<T>");
	_addInputSlot(dy, "dy", "derivation in y", "CImgList<T>");
	_addInputSlot(dz, "dz", "derivation in z (for 3D mode)", "CImgList<T>");
	_addInputSlot(dt, "dt", "derivation in t", "CImgList<T>");
	_addParameter (smoothMask, "smoothMask",
			"linear 1D filter mask applied to derivatives",
			"T_list");
}
#endif
