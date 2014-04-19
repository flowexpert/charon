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
/** \file CliqueWeightNonLocal.hxx
 *  Implementation of the parameter class CliqueWeightNonLocal.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 29.03.2012
 */

#ifndef _CLIQUEWEIGHTNONLOCAL_HXX_
#define _CLIQUEWEIGHTNONLOCAL_HXX_

#include "CliqueWeightNonLocal.h"

#include <charon/CliqueWeight.hxx>

template <class T>
CliqueWeightNonLocal<T>::CliqueWeightNonLocal(const std::string& name) :
	CliqueWeight<T>(
		"CliqueWeightNonLocal", name,
		"This module can be connected to a WeightedMedian filter. "
		"It yields a <i>non-local</i> weight for each element within "
		"the filter window, depending on the properties of the "
		"clique consisting of the window center position and the "
		"considered element position. "
		"The clique-weights are computed as described within the paper "
		"<b>Secrets of Optical Flow Estimation and Their Principles</b> "
		"by <i>D. Sun</i>, <i>S. Roth</i> and <i>M. Black</i> (2010), "
		"see eq. 9 for details."
	)
{
	this->_addInputSlot(data,    "data",   "data input",   "CImgList<T>");
	this->_addInputSlot(motion,  "motion", "motion input", "CImgList<T>");

	this->_addParameter(sigma_spatial, "sigma_spatial",
	                    "spatial difference stddev", T(7));
	this->_addParameter(sigma_color, "sigma_color",
	                    "color difference stddev", T(7));
	this->_addParameter(sigma_occ_divergence, "sigma_occ_divergence",
	                    "occlusion divergence stddev", T(0.3));
	this->_addParameter(sigma_occ_color, "sigma_occ_color",
	                    "occlusion color difference stddev", T(20));

	ParameteredObject::_setTags("charon-flow;MedianFilters;CImg");
}

template <class T>
void CliqueWeightNonLocal<T>::execute()
{
	CliqueWeight<T>::execute();

	_data = data();
	_motion = motion();

	_sigma_spatial = sigma_spatial();
	_sigma_color = sigma_color();
	_sigma_occ_divergence = sigma_occ_divergence();
	_sigma_occ_color = sigma_occ_color();
}

template <class T>
T CliqueWeightNonLocal<T>::getCliqueWeight( int, int xx, int yy, int zz, int,
                                            int, int x,  int y,  int z,  int )
{
	// calculate motion divergence
	T dxU = (_motion.atNXYZC( 0, xx+x+1, yy+y,   zz+z, 0 )
	        -_motion.atNXYZC( 0, xx+x-1, yy+y,   zz+z, 0 ))/2;
	T dyV = (_motion.atNXYZC( 1, xx+x,   yy+y+1, zz+z, 0 )
	        -_motion.atNXYZC( 1, xx+x,   yy+y-1, zz+z, 0 ))/2;
	T divergence = dxU + dyV;
	if (divergence > 0) divergence = 0;

	// calculate spatial weight
	T spatial_weight = _gauss( sqrt(double(x*x + y*y)), 0, _sigma_spatial );

	// calculate spatial color weight
	T color_diff = _data.atNXYZC( 0, xx,   yy,   zz,   0 )
	             - _data.atNXYZC( 0, xx+x, yy+y, zz+z, 0 );
	T color_weight = _gauss( color_diff, 0, _sigma_color );

	// calculate motion compensated data difference
	T motU = _motion.atNXYZC( 0, xx+x, yy+y, zz+z, 0 );
	T motV = _motion.atNXYZC( 1, xx+x, yy+y, zz+z, 0 );
	T mot_comp_diff = _data.atNXYZC( 0, xx+x,      yy+y,      zz+z, 0 )
	                - _data.atNXYZC( 0, xx+x+motU, yy+y+motV, zz+z, 1 );

	// calculate occlusion weight
	T occlusion_weight =  _gauss( divergence, 0, _sigma_occ_divergence );
	occlusion_weight   *= _gauss( mot_comp_diff, 0, _sigma_occ_color );

	T weight = spatial_weight * color_weight * occlusion_weight;

	return weight;
}

template <typename T>
inline T CliqueWeightNonLocal<T>::_gauss( T x, T mu, T sigma )
{
	return 1/(sqrt(2*M_PI*sigma*sigma))
	  * exp(double(-(x - mu)*(x - mu)/(2*sigma*sigma))) ;
}

#endif /* _CLIQUEWEIGHTNONLOCAL_HXX_ */

