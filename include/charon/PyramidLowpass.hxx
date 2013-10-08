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
			"Module providing a <b>scale-space</b> approach for motion estimation.<br> "
			"It performs Gaussian <i>band-pass</i> filtering utilizing the band-pass "
			"within "
			"sigmas[level()] and sigmas[level()-1] or resp. 0, if level() = 0 . <br>"
			"Optionally, bilateral Gaussian blur can be performed, by connecting a "
			"blurMask (whereas 0 denotes borders and non-0 regions) and setting "
			"bilateralBlur = true . "
			"Additionally, bilateralBlurCount denotes the count of consecutive bilateral "
			"blur iterations, whereas bilateralBlurSeparable selects a separable "
			"and thus much more efficient blur <i>approximation</i>. "
		),
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
			bilateralBlurSeparable, "bilateralBlurSeparable", "if true, blur is executed approximately in a separable way",
			"bool");
	ParameteredObject::_addParameter(
			bilateralBlurCount, "bilateralBlurCount", "count of bilateral blur filtering steps", 1u);

	ParameteredObject::_setTags("charon-flow;MultiscaleApproaches;CImg");
}

template <typename T>
void PyramidLowpass<T>::execute() {
	const cimg_library::CImgList<T>& si = seqIn();
	cimg_library::CImgList<T>& so = seqOut();
	so = si;

	cimg_library::CImg<T> tmp = si[0];

	_width    = si[0].width();
	_height   = si[0].height();
	_depth    = si[0].depth();
	_spectrum = si[0].spectrum();

	unsigned int _blurRadius = 0;
	bool _bilateralBlur = bilateralBlur();
	int _bilateralBlurCount = bilateralBlurCount();
	bool _bilateralBlurSeparable = bilateralBlurSeparable();

	float blurFactor1 = 0.0;
	float blurFactor2 = 0.0;

	if (level() == 0) {
		blurFactor2 = sigmas()[level()];
		_blurRadius = floor(blurFactor2 * 1.5 + 0.5);
		cimglist_for(so,kk)
		{
			if (_bilateralBlur && blurFactor2) {
				tmp = si[kk];
				for (int i=0; i<_bilateralBlurCount; i++) {
				  if (_bilateralBlurSeparable) {
				    tmp = _blurSeparable(0, tmp, blurFactor2, _blurRadius);
				    tmp = _blurSeparable(1, tmp, blurFactor2, _blurRadius);
				  } else {
				    tmp = _blur(tmp, blurFactor2, _blurRadius);
				  }
				}
				so.at(kk) = tmp;
			} else if (blurFactor2) {
				so.at(kk).blur(blurFactor2);
			}
		}
	} else {
		cimg_library::CImgList<T> tmp1 = si;
		cimg_library::CImgList<T> tmp2 = si;
		blurFactor1 = sigmas()[level()-1];
		_blurRadius = floor(blurFactor1 * 1.5 + 0.5);
		cimglist_for(tmp1,kk)
		{
			if (_bilateralBlur && blurFactor1) {
				tmp = si[kk];
				for (int i=0; i<_bilateralBlurCount; i++) {
				  if (_bilateralBlurSeparable) {
				    tmp = _blurSeparable(0, tmp, blurFactor1, _blurRadius);
				    tmp = _blurSeparable(1, tmp, blurFactor1, _blurRadius);
				  } else {
				    tmp = _blur(tmp, blurFactor1, _blurRadius);
				  }
				}
				tmp1.at(kk) = tmp;
			} else if (blurFactor1) {
				tmp1.at(kk).blur(blurFactor1);
			}
		}
		blurFactor2 = sigmas()[level()];
		_blurRadius = floor(blurFactor2 * 1.5 + 0.5);
		cimglist_for(tmp2,kk)
		{
			if (_bilateralBlur && blurFactor2) {
				tmp = si[kk];
				for (int i=0; i<_bilateralBlurCount; i++) {
				  if (_bilateralBlurSeparable) {
				    tmp = _blurSeparable(0, tmp, blurFactor2, _blurRadius);
				    tmp = _blurSeparable(1, tmp, blurFactor2, _blurRadius);
				  } else {
				    tmp = _blur(tmp, blurFactor2, _blurRadius);
				  }
				}
				tmp2.at(kk) = tmp;
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
cimg_library::CImg<T> PyramidLowpass<T>::_blur( const cimg_library::CImg<T> si, T sigma, int radius )
{
	const cimg_library::CImgList<T>& _blurMask = blurMask();

	cimg_library::CImg<T> ret( _width, _height, _depth, _spectrum );

	int cx = 0, cy = 0;
	T retVal = T(0);

	std::vector< int > queueX;
	std::vector< int > queueY;
	for (int x=0; x<_width; x++)
	for (int y=0; y<_height; y++)
	for (int z=0; z<_depth; z++)
	for (int c=0; c<_spectrum; c++)  //  t
	{
		cimg_library::CImg<bool> visited( 2*radius+1, 2*radius+1, 1, 1, false );
		cimg_library::CImg<T> probabilities( 2*radius+1, 2*radius+1, 1, 1, T(0) );
		cimg_library::CImg<T> imgVals( 2*radius+1, 2*radius+1, 1, 1, T(0) );

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
			imgVals.atXYZC( radius + cx - x, radius + cy - y, 0, 0 ) = si.atXYZC( cx, cy, z, c );
			probabilities.atXYZC( radius + cx - x, radius + cy - y, 0, 0 ) =
			  _gauss( pow( pow(double(cx-x), 2.0) + pow(double(cy-y), 2.0), 0.5 ), T(0.0), sigma ) ;

			if (!visited.atXYZC(  radius + cx - x, radius + cy-1 - y, 0, 0 ) && 
			    _blurMask[0].atXYZC( cx, cy-1, z, c ) && 
			    ((cx   - x) * (cx   - x) <= radius*radius) &&
			    ((cy-1 - y) * (cy-1 - y) <= radius*radius) ) {
				// push back north neighbor
				queueX.push_back( cx );
				queueY.push_back( cy-1 );
			}

			if (!visited.atXYZC(  radius + cx+1 - x, radius + cy - y, 0, 0 ) &&
			    _blurMask[0].atXYZC( cx+1, cy, z, c ) &&
			    ((cx+1 - x) * (cx+1 - x) <= radius*radius) &&
			    ((cy   - y) * (cy   - y) <= radius*radius) ) {
				// push back east neighbor
				queueX.push_back( cx+1 );
				queueY.push_back( cy );
			}

			if (!visited.atXYZC(  radius + cx - x, radius + cy+1 - y, 0, 0 ) &&
			    _blurMask[0].atXYZC( cx, cy+1, z, c ) &&
			    ((cx   - x) * (cx   - x) <= radius*radius) &&
			    ((cy+1 - y) * (cy+1 - y) <= radius*radius) ) {
				// push back south neighbor
				queueX.push_back( cx );
				queueY.push_back( cy+1 );
			}

			if (!visited.atXYZC(  radius + cx-1 - x, radius + cy - y, 0, 0 ) &&
			    _blurMask[0].atXYZC( cx-1, cy, z, c ) &&
			    ((cx-1 - x) * (cx-1 - x) <= radius*radius) &&
			    ((cy   - y) * (cy   - y) <= radius*radius) ) {
				// push back west neighbor
				queueX.push_back( cx-1 );
				queueY.push_back( cy );
			}
		}

		T probabilitySum = T(0);
		for (int nx=0; nx<2*radius+1; nx++)
		for (int ny=0; ny<2*radius+1; ny++)
		{
			probabilitySum += probabilities.atXYZC( nx, ny, 0, 0 );
		}

		retVal = T(0);
		for (int nx=0; nx<2*radius+1; nx++)
		for (int ny=0; ny<2*radius+1; ny++)
		{
			retVal += imgVals.atXYZC( nx, ny, 0, 0 )
			        * probabilities.atXYZC( nx, ny, 0, 0 );
		}
		if (probabilitySum) {
			retVal /= probabilitySum;
		} else {
			retVal = T(0);
		}
		ret.atXYZC( x, y, z, c ) = retVal;
	}

	return ret;
}

template <typename T>
cimg_library::CImg<T> PyramidLowpass<T>::_blurSeparable( int dir, const cimg_library::CImg<T> si, T sigma, int radius )
{
	const cimg_library::CImgList<T>& _blurMask = blurMask();

	cimg_library::CImg<T> ret( _width, _height, _depth, _spectrum );

	int current = 0;
	T retVal = T(0);

	std::vector< int > queue;
	for (int x=0; x<_width; x++)
	for (int y=0; y<_height; y++)
	for (int z=0; z<_depth; z++)
	for (int c=0; c<_spectrum; c++)  //  t
	{
		cimg_library::CImg<bool> visited( 2*radius+1, 1, 1, 1, false );
		cimg_library::CImg<T> probabilities( 2*radius+1, 1, 1, 1, T(0) );
		cimg_library::CImg<T> imgVals( 2*radius+1, 1, 1, 1, T(0) );

		queue.clear();
		if (dir == 0) {
		  queue.push_back( x );
		} else {
		  queue.push_back( y );
		}
		while (!queue.empty()) {
			current = queue.back();
			queue.pop_back();

			if (dir == 0) {
			  visited.atXYZC( radius + current - x, 0, 0, 0 ) = true;
			  imgVals.atXYZC( radius + current - x, 0, 0, 0 ) = si.atXYZC( current, y, z, c );
			  probabilities.atXYZC( radius + current - x, 0, 0, 0 ) =
			    _gauss( pow(pow(double(current-x), 2.0),0.5), T(0.0), sigma ) ;
			} else {
			  visited.atXYZC( radius + current - y, 0, 0, 0 ) = true;
			  imgVals.atXYZC( radius + current - y, 0, 0, 0 ) = si.atXYZC( x, current, z, c );
			  probabilities.atXYZC( radius + current - y, 0, 0, 0 ) =
			    _gauss( pow(pow(double(current-y), 2.0),0.5), T(0.0), sigma ) ;
			}

			if (dir == 0) {
			  if (!visited.atXYZC( radius + current+1 - x, 0, 0, 0 ) &&
			      _blurMask[0].atXYZC( current+1, y, z, c ) &&
			      ((current+1 - x) * (current+1 - x) <= radius*radius) ) {
			    // push back east neighbor
			    queue.push_back( current+1 );
			  }
			  if (!visited.atXYZC( radius + current-1 - x, 0, 0, 0 ) &&
			      _blurMask[0].atXYZC( current-1, y, z, c ) &&
			      ((current-1 - x) * (current-1 - x) <= radius*radius) ) {
			    // push back west neighbor
			    queue.push_back( current-1 );
			  }
			} else {
			  if (!visited.atXYZC( radius + current+1 - y, 0, 0, 0 ) &&
			      _blurMask[0].atXYZC( x, current+1, z, c ) &&
			      ((current+1 - y) * (current+1 - y) <= radius*radius) ) {
			    // push back south neighbor
			    queue.push_back( current+1 );
			  }
			  if (!visited.atXYZC( radius + current-1 - y, 0, 0, 0 ) &&
			      _blurMask[0].atXYZC( x, current-1, z, c ) &&
			      ((current-1 - y) * (current-1 - y) <= radius*radius) ) {
			    // push back north neighbor
			    queue.push_back( current-1 );
			  }
			}
		}

		T probabilitySum = T(0);
		for (int n=0; n<2*radius+1; n++)
		{
			probabilitySum += probabilities.atXYZC( n, 0, 0, 0 );
		}

		retVal = T(0);
		for (int n=0; n<2*radius+1; n++)
		{
		  retVal += imgVals.atXYZC( n, 0, 0, 0 ) * probabilities.atXYZC( n, 0, 0, 0 );
		}
		if (probabilitySum) {
			retVal /= probabilitySum;
		} else {
			retVal = T(0);
		}
		ret.atXYZC( x, y, z, c ) = retVal;
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
