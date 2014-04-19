/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Charon.  If not, see <http://www.gnu.org/licenses/>.
 */
/** \file IrlsNonLocal.hxx
 *  Implementation of the parameter class IrlsNonLocal.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 10.01.2012
 */

#ifndef _IRLSNONLOCAL_HXX_
#define _IRLSNONLOCAL_HXX_

#include "IrlsNonLocal.h"

#define EPS_INVERSE 1e9

template<typename T>
IrlsNonLocal<T>::IrlsNonLocal(const std::string& name) :
	TemplatedParameteredObject<T>(
			"IrlsNonLocal", name,
			"<h2>deprecated</h2><br>"
			"see Irls documentation for more details."
			)
{
	this->_addInputSlot(img,    "img",    "image input",  "CImgList<T>");
	this->_addInputSlot(motionUV, "motionUV", "motion input", "CImgList<T>");
	this->_addOutputSlot(out,  "out",   "image output", "CImgList<T>");
	this->_addParameter(
			windowRadius, "windowRadius",
			"radius r of image windows (size is 2*r+1)",  7u);
	this->_addParameter(
			iterations, "iterations",
			"iteration count (0 => mean filtering)", 10u);
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
	ParameteredObject::_setTags("charon-flow;CImg;MedianFilters;deprecated");
}

template<typename T>
void IrlsNonLocal<T>::execute() {
	T _sigma_spatial = sigma_spatial();
	T _sigma_color = sigma_color();
	T _sigma_occ_divergence = sigma_occ_divergence();
	T _sigma_occ_color = sigma_occ_color();

	const cimg_library::CImgList<T>& _img = img();
	const cimg_library::CImgList<T>& _motionUV = motionUV();

	cimg_library::CImgList<T>& o = out();
	const unsigned int& r = windowRadius();
	o = _motionUV;

	int dim = _motionUV.size();
	std::vector<cimg_library::CImg<T> > window( dim );
	std::vector<T> med( dim );
	std::vector<T> tmp( dim );

	int _iterations = iterations();

	T weight, weight_sum, dist;
	T dx, dy, dz, dxU, dyV;

	T spatial_weight;
	T color_weight, color_diff;
	T occlusion_weight, motX, motY, mot_comp_diff, divergence;

	T tmp_weight;

	cimg_forXYZC(o[0],xx,yy,zz,tt) {
		for (int d=0; d<dim; ++d) {
			window[d] = _motionUV[d].get_crop(xx-r,yy-r,zz,tt,xx+r,yy+r,zz,tt);
		}

		// calculate mean as an initial guess
		for (int d=0; d<dim; ++d) {
			med[d] = T(0);
			weight_sum = T(0);
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

				weight_sum += weight;
				med[d] += weight * window[d]( i, j, zz, tt );
			}
			med[d] /= weight_sum;
		}

		// perform iteratively weighted least squares
		// see "Numerical Methods for Scientific Computing Vol. II"
		// by Germund Dahlquist and Åke Björck, section 8.7.5 for details
		for (int i=0; i<_iterations; ++i) {
			weight_sum = T(0);
			for (int d=0; d<dim; ++d) {
				tmp[d] = T(0);
			}
			for (unsigned int i=0; i<2*r+1; ++i)
			for (unsigned int j=0; j<2*r+1; ++j)
			{
				dist = T(0);
				for (int d=0; d<dim; ++d) {
					dist += pow( double(window[d]( i, j, zz, tt ) - med[d]), 2.0 );
				}
				dist = pow( double(dist), 0.5 );

				if (dist != 0) {
					weight = 1.0 / dist;
				} else {
					weight = EPS_INVERSE;
				}

				tmp_weight = weight;

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

                                weight =  spatial_weight * color_weight * occlusion_weight;

				weight *= tmp_weight;

				weight_sum += weight;
				for (int d=0; d<dim; ++d) {
					tmp[d] += weight * window[d]( i, j, zz, tt );
				}
			}
			for (int d=0; d<dim; ++d) {
				med[d] = tmp[d] / weight_sum;
			}
		}

		// write back
		for (int d=0; d<dim; ++d) {
			o(d,xx,yy,zz,tt) = med[d];
		}
	}
}

template <typename T>
inline T IrlsNonLocal<T>::_gauss( T x, T mu, T sigma )
{
  return 1/(sqrt(2*M_PI*sigma*sigma))
    * exp(double(-(x - mu)*(x - mu)/(2*sigma*sigma))) ;
}

#endif /* _IRLSNONLOCAL_HXX_ */

