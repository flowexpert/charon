/*  Copyright (C) 2012, 2013
                  University of Heidelberg (IWR/HCI)

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
/** \file PyramidLowpass.hxx
 *  Implementation of the parameter class PyramidLowpass.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 26.07.2012
 */

#ifndef _PYRAMID_LOWPASS_HXX_
#define _PYRAMID_LOWPASS_HXX_

#include "PyramidLowpass.h"
#include <charon-utils/Convolution.h>
#include <iomanip>

#include <vector>

template <typename T>
PyramidLowpass<T>::PyramidLowpass(const std::string& name) :
		TemplatedParameteredObject<T>("PyramidLowpass", name,
			"Lowpass filtering for pyramid-based flow-estimation algorithms."),
		blurMask( true, false )
{
	ParameteredObject::_addInputSlot(
			seqIn, "seqIn", "sequence input", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			blurMask, "blurMask", "mask for bilateral blur", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			level, "level", "level select");
	ParameteredObject::_addOutputSlot(
			seqOut, "seqOut", "sequence output", "CImgList<T>");
	ParameteredObject::_addParameter(
			sigmas, "sigmas", "list of std. deviations in decreasing order", "T_list");
	ParameteredObject::_addParameter(
			bilateralBlur, "bilateralBlur", "if true, blurring will stop at region borders", "bool");
	ParameteredObject::_addParameter(
			blurRadius, "blurRadius", "radius for bilateral blur window", "unsigned int" );

	ParameteredObject::_setTags("charon-flow;MultiscaleApproaches;CImg");
}

template <typename T>
void PyramidLowpass<T>::execute() {
	const cimg_library::CImgList<T>& si = seqIn();
	cimg_library::CImgList<T>& so = seqOut();
	so = si;

	_width    = si[0].width();
	_height   = si[0].height();
	_depth    = si[0].depth();
	_spectrum = si[0].spectrum();

	bool _bilateralBlur = bilateralBlur();
	unsigned int _blurRadius = blurRadius();

	float blurFactor1 = 0.0;
	float blurFactor2 = 0.0;

	if (level() == 0) {
		blurFactor2 = sigmas()[level()];
		cimglist_for(so,kk)
		{
			if (_bilateralBlur && blurFactor2) {
				so.at(kk) = _blur(kk, blurFactor2, _blurRadius);
			} else if (blurFactor2) {
				so.at(kk).blur(blurFactor2);
			}
		}
	} else {
		cimg_library::CImgList<T> tmp1 = si;
		cimg_library::CImgList<T> tmp2 = si;
		blurFactor1 = sigmas()[level()-1];
		cimglist_for(tmp1,kk)
		{
			if (_bilateralBlur && blurFactor1) {
				tmp1.at(kk) = _blur(kk, blurFactor1, _blurRadius);
			} else if (blurFactor1) {
				tmp1.at(kk).blur(blurFactor1);
			}
		}
		blurFactor2 = sigmas()[level()];
		cimglist_for(tmp2,kk)
		{
			if (_bilateralBlur && blurFactor2) {
				tmp2.at(kk) = _blur(kk, blurFactor2, _blurRadius);
			} else if (blurFactor2) {
				tmp2.at(kk).blur(blurFactor2);
			}
		}
		cimglist_for(so,kk)
		{
			so.at(kk) = tmp2.at(kk) - tmp1.at(kk);
		}
	}
}

template <typename T>
cimg_library::CImg<T> PyramidLowpass<T>::_blur( int kk, T sigma, int radius )
{
	const cimg_library::CImgList<T>& si = seqIn();
	const cimg_library::CImgList<T>& _blurMask = blurMask();

	cimg_library::CImg<T> ret( _width, _height, _depth, _spectrum );

	int cx = 0, cy = 0;
	T retVal = T(0);

	std::vector< int > queueX;
	std::vector< int > queueY;
	cimg_forXY( ret, x, y )
	{
		cimg_library::CImg<bool> visited( 2*radius+1, 2*radius+1, 1, 1, false );
		cimg_library::CImg<T> probabilities( 2*radius+1, 2*radius+1, 1, 1, T(0) );
		cimg_library::CImg<T> imgVals( 2*radius+1, 2*radius+1, _depth, _spectrum, T(0) );
		T probabilitySum = T(0);

		queueX.clear();
		queueY.clear();
		queueX.push_back( x );
		queueY.push_back( y );
		while (!queueX.empty()) {
			cx = queueX.back();
			queueX.pop_back();
			cy = queueY.back();
			queueY.pop_back();

			visited.atXYZC( radius + cx - x, radius + cy - y, 0, 0 ) = true;
			for (int z=0; z<_depth; z++)
			for (int c=0; c<_spectrum; c++)
			{
				imgVals.atXYZC( radius + cx - x, radius + cy - y, z, c ) = si[kk].atXYZC( cx, cy, z, c );
			}
			probabilities.atXYZC( radius + cx - x, radius + cy - y, 0, 0 ) =
			  _gauss( pow( pow(double(cx-x), 2.0) + pow(double(cy-y), 2.0), 0.5 ), T(0.0), sigma ) ;
			probabilitySum += probabilities.atXYZC( radius + cx -x, radius + cy - y, 0, 0 );

			if (!visited.atXYZC(  radius + cx - x, radius + cy-1 - y, 0, 0 ) && 
			    _blurMask[0].atXYZC( cx, cy-1, 0, 0 ) && 
			    ((cx   - x) * (cx   - x) <= radius*radius) &&
			    ((cy-1 - y) * (cy-1 - y) <= radius*radius) ) {
				// push back north neighbor
				queueX.push_back( cx );
				queueY.push_back( cy-1 );
			}

			if (!visited.atXYZC(  radius + cx+1 - x, radius + cy - y, 0, 0 ) &&
			    _blurMask[0].atXYZC( cx+1, cy, 0, 0 ) &&
			    ((cx+1 - x) * (cx+1 - x) <= radius*radius) &&
			    ((cy   - y) * (cy   - y) <= radius*radius) ) {
				// push back east neighbor
				queueX.push_back( cx+1 );
				queueY.push_back( cy );
			}

			if (!visited.atXYZC(  radius + cx - x, radius + cy+1 - y, 0, 0 ) &&
			    _blurMask[0].atXYZC( cx, cy+1, 0, 0 ) &&
			    ((cx   - x) * (cx   - x) <= radius*radius) &&
			    ((cy+1 - y) * (cy+1 - y) <= radius*radius) ) {
				// push back south neighbor
				queueX.push_back( cx );
				queueY.push_back( cy+1 );
			}

			if (!visited.atXYZC(  radius + cx-1 - x, radius + cy - y, 0, 0 ) &&
			    _blurMask[0].atXYZC( cx-1, cy, 0, 0 ) &&
			    ((cx-1 - x) * (cx-1 - x) <= radius*radius) &&
			    ((cy   - y) * (cy   - y) <= radius*radius) ) {
				// push back west neighbor
				queueX.push_back( cx-1 );
				queueY.push_back( cy );
			}
		}

		for (int z=0; z<_depth; z++)
		for (int c=0; c<_spectrum; c++)
		{
			retVal = T(0);
			cimg_forXY( imgVals, nx, ny )
			{
				retVal += imgVals.atXYZC( nx, ny, z, c )
				        * probabilities.atXYZC( nx, ny, 0, 0 );
			}	
			if (probabilitySum) {
				retVal /= probabilitySum;
			} else {
				retVal = T(0);
			}
			ret.atXYZC( x, y, z, c ) = retVal;
		}
	}

	return ret;
}

template <typename T>
inline T PyramidLowpass<T>::_gauss( T x, T mu, T sigma )
{
  return // 1/(sqrt(2*M_PI*sigma*sigma)) *
         exp(double(-(x - mu)*(x - mu)/(2*sigma*sigma))) ;
}

#endif /* _PYRAMID_LOWPASS_HXX_ */

