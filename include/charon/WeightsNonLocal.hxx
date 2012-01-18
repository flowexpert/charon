/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

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
/** \file WeightsNonLocal.hxx
 *  Implementation of the parameter class WeightsNonLocal.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 18.01.2012
 */

#ifndef _WEIGHTSNONLOCAL_HXX_
#define _WEIGHTSNONLOCAL_HXX_

#include "WeightsNonLocal.h"

#define EPS_INVERSE 1e9

template<typename T>
WeightsNonLocal<T>::WeightsNonLocal(const std::string& name) :
	TemplatedParameteredObject<T>(
			"WeightsNonLocal", name,
			"calculates non-local weights.<br><br>")
{
	this->_addInputSlot(img,    "img",    "image input",  "CImgList<T>");
	this->_addInputSlot(motionUV, "motionUV", "motion input", "CImgList<T>");
	this->_addOutputSlot(out,  "out",   "weights output", "CImgList<T>");
	this->_addParameter(
			windowRadius, "windowRadius",
			"radius r of image windows (size is 2*r+1)",  7u);
	this->_addParameter(
			sigma_spatial, "sigma_spatial",
			"spatial difference weight", T(7));
	this->_addParameter(
			sigma_color, "sigma_color",
			"color difference weight", T(7));
	this->_addParameter(
			sigma_occ_divergence, "sigma_occ_divergence",
			"occlusion divergence weight", T(0.3));
	this->_addParameter(
			sigma_occ_color, "sigma_occ_color",
			"occlusion color difference weight", T(20));
}

template<typename T>
void WeightsNonLocal<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	T _sigma_spatial = sigma_spatial();
	T _sigma_color = sigma_color();
	T _sigma_occ_divergence = sigma_occ_divergence();
	T _sigma_occ_color = sigma_occ_color();

	const cimg_library::CImgList<T>& _img = img();
	const cimg_library::CImgList<T>& _motionUV = motionUV();

	cimg_library::CImgList<T>& o = out();
	const unsigned int& r = windowRadius();
	o = _motionUV;

	T weight;
	T dx, dy, dz, dxU, dyV;

	T spatial_weight;
	T color_weight, color_diff;
	T occlusion_weight, motX, motY, mot_comp_diff, divergence;

	cimg_forXYZC(o[0],xx,yy,zz,tt)
	for (unsigned int i=0; i<2*r+1; ++i)
	for (unsigned int j=0; j<2*r+1; ++j)
	{
		dx = i - r;
		dy = j - r;
		dz = T(0);

		// calculate motion divergence
		dxU = (_motionUV.atNXYZC( 0, xx+dx+1, yy+dy,   zz+dz, 0 )
		      -_motionUV.atNXYZC( 0, xx+dx-1, yy+dy,   zz+dz, 0 ))/2;
		dyV = (_motionUV.atNXYZC( 1, xx+dx,   yy+dy+1, zz+dz, 0 )
		      -_motionUV.atNXYZC( 1, xx+dx,   yy+dy-1, zz+dz, 0 ))/2;
		divergence = dxU + dyV;
		if (divergence > 0) divergence = 0;

		// calculate spatial weight
		spatial_weight = _gauss( sqrt(double(dx*dx + dy*dy)),
		                         0, _sigma_spatial );

		// calculate spatial color weight
		color_diff = _img.atNXYZC( 0, xx,    yy,    zz, 0 )
		           - _img.atNXYZC( 0, xx+dx, yy+dy, zz, 0 );
		color_weight = _gauss( color_diff, 0, _sigma_color );

		// calculate residual
		motX = _motionUV.atNXYZC( 0, xx+dx, yy+dy, zz+dz, 0 );
		motY = _motionUV.atNXYZC( 1, xx+dx, yy+dy, zz+dz, 0 );
		mot_comp_diff = _img.atNXYZC( 0, xx+dx,      yy+dy,      zz+dz, 0 )
		              - _img.atNXYZC( 0, xx+dx+motX, yy+dy+motY, zz+dz, 1 );

		// calculate occlusion weight
		occlusion_weight =  _gauss( divergence,
		                            0, _sigma_occ_divergence );
		occlusion_weight *= _gauss( mot_comp_diff,
		                            0, _sigma_occ_color );

		weight = spatial_weight * color_weight * occlusion_weight;

		// write back
		o(0,xx,yy,zz,tt) = weight;
	}
}

template <typename T>
inline T WeightsNonLocal<T>::_gauss( T x, T mu, T sigma )
{
  return 1/(sqrt(2*M_PI*sigma*sigma))
    * exp(double(-(x - mu)*(x - mu)/(2*sigma*sigma))) ;
}

#endif /* _WEIGHTSNONLOCAL_HXX_ */

