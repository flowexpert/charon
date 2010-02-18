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
/** \file Diff2D.hxx
 *  Implementation of the parameter class Diff2D.
 *  \author Jens-Malte Gottfried
 *  \date 10.02.2010
 */

#ifndef _DIFF2D_HXX_
#define _DIFF2D_HXX_

#include "Diff2D.h"
#include <charon-utils/Convolution.h>

template <typename T>
Diff2D<T>::Diff2D(const std::string& name) :
		TemplatedParameteredObject<T>("Diff2D", name,
			"Calculate 2D derivatives.<br><br>"
			"This plugin serves to simplify calculations of 2D derivations. "
			"Although derivatives may be calculated using class LinearFilter, "
			"this plugin makes derivative calculations more compact.<br>"
			"Only the derivatives of connected output slots are calculated."
			"<br><br>If you perform some kind of iterations, it is essential "
			"to connect the count input slot, because recalculation takes "
			"place on count changes only. "
			"If count is not connected, only one calculation is performed "
			"and reused later."
			),
		diffMaskXY("0.5;0;-0.5"),
		smoothMaskXY("0.1875;0.625;0.1875"),
		diffMaskT("1;-1"),
		count(true,false), // optional
		_count(-1)
{
	ParameteredObject::_addParameter (
			diffMaskXY, "diffMaskXY",
			"linear 1D filter mask used for derivation wrt x,y", "T_list");
	ParameteredObject::_addParameter (
			smoothMaskXY, "smoothMaskXY",
			"linear 1D filter mask applied orthogonal of derivation (wrt x,y)",
			"T_list");
	ParameteredObject::_addParameter (
			diffMaskT, "diffMaskT",
			"linear 1D filter mask used for derivation wrt t", "T_list");
	ParameteredObject::_addInputSlot(
			img, "img",
			"image input", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			count, "count",
			"if connected, calculations are performed on count change only, "
			"if unconnected, calculations are performed exactly once");
	ParameteredObject::_addOutputSlot(
			dx, "dx", "derivative wrt x", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			dy, "dy", "derivative wrt y", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			dt, "dt", "derivative wrt t", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			dxx, "dxx", "second derivative wrt x", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			dxy, "dxy", "second derivative wrt x,y", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			dyy, "dyy", "second derivative wrt y", "CImgList<T>");
}

template <typename T>
void Diff2D<T>::execute() {
	ParameteredObject::execute();

	// check if calculation needed
	if (_count < -1 || (count.connected() && _count == int(count()))) {
		sout << "\tskipping derivative calculation" << std::endl;
		return;
	}

	sout << "\tcalculating derivatives:" << std::endl;

	// update "old" counter value
	if (count.connected())
		_count = count();
	else
		_count = -2;

	// set up convolution masks
	cimg_library::CImg<T> maskDx (diffMaskXY().size()  ,1u,1u,1u);
	cimg_library::CImg<T> maskDy (1u,diffMaskXY().size()  ,1u,1u);
	cimg_library::CImg<T> maskDt (1u,1u, diffMaskT().size()  ,1u);
	cimg_library::CImg<T> smoothX(smoothMaskXY().size(),1u,1u,1u);
	cimg_library::CImg<T> smoothY(1u,smoothMaskXY().size(),1u,1u);

	cimg_forX(maskDx,  x) maskDx (x,0,0,0) = diffMaskXY()  [x];
	cimg_forY(maskDy,  y) maskDy (0,y,0,0) = diffMaskXY()  [y];
	cimg_forZ(maskDt,  z) maskDt (0,0,z,0) = diffMaskT()   [z];
	cimg_forX(smoothX, x) smoothX(x,0,0,0) = smoothMaskXY()[x];
	cimg_forY(smoothY, y) smoothY(0,y,0,0) = smoothMaskXY()[y];

	if(dx.connected() || dxx.connected() || dxy.connected()) {
		sout << "\t\tdx" << std::endl;
		dx().assign(img());
		cimglist_for(dx(),kk) {
			Convolution::convolve(maskDx, dx()[kk],dx()[kk]);
			if (smoothMaskXY().size() >= 2)
				Convolution::convolve(smoothY,dx()[kk],dx()[kk]);
		}
	}
	if(dy.connected() || dxy.connected() || dyy.connected()) {
		sout << "\t\tdy" << std::endl;
		dy().assign(img());
		cimglist_for(dy(), kk) {
			Convolution::convolve(maskDy, dy()[kk],dy()[kk]);
			if (smoothMaskXY().size() >= 2)
				Convolution::convolve(smoothX,dy()[kk],dy()[kk]);
		}
	}
	if(dt.connected()) {
		sout << "\t\tdt" << std::endl;
		dt().assign(img());
		// since convolution along dim4 does not work, we have
		// to switch dim3 and dim4 in temporary CImg instance.
		cimg_library::CImg<T> temp(
				img()[0].width(),
				img()[0].height(),
				img()[0].spectrum(),
				img()[0].depth());
		cimglist_for(img(),kk) {
			cimg_forXYZC(img()[kk],x,y,z,t)
				temp(x,y,t,z) = img()(kk,x,y,z,t);
			Convolution::convolve(maskDt,temp,temp);
			cimg_forXYZC(dt()[kk],x,y,z,t)
				dt()(kk,x,y,z,t) = temp(x,y,t,z);
		}
	}
	if(dxx.connected()) {
		sout << "\t\tdxx" << std::endl;
		dxx().assign(dx());
		cimglist_for(dxx(),kk) {
			Convolution::convolve(maskDx,dxx()[kk],dxx()[kk]);
			if (smoothMaskXY().size() >= 2)
				Convolution::convolve(smoothY,dxx()[kk],dxx()[kk]);
		}
	}
	if(dxy.connected()) {
		sout << "\t\tdxy" << std::endl;
		dxy().assign(dx());
		cimglist_for(dxy(),kk) {
			Convolution::convolve(maskDy,dxy()[kk],dxy()[kk]);
			if (smoothMaskXY().size() >= 2)
				Convolution::convolve(smoothX,dxy()[kk],dxy()[kk]);
		}
	}
	if(dyy.connected()) {
		sout << "\t\tdyy" << std::endl;
		dyy().assign(dy());
		cimglist_for(dyy(),kk) {
			Convolution::convolve(maskDy,dyy()[kk],dyy()[kk]);
			if (smoothMaskXY().size() >= 2)
				Convolution::convolve(smoothX,dyy()[kk],dyy()[kk]);
		}
	}
}

#endif /* _DIFF2D_HXX_ */



