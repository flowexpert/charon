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
/** \file PyramidRescaleMatlab.hxx
 *  Implementation of the parameter class PyramidRescaleMatlab.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 02.08.2012
 */

#ifndef _PYRAMID_RESCALE_MATLAB_HXX_
#define _PYRAMID_RESCALE_MATLAB_HXX_

#include "PyramidRescaleMatlab.h"
#include <charon-utils/Convolution.h>
#include <charon-utils/Roi.hxx>
#include <iomanip>

template <typename T>
PyramidRescaleMatlab<T>::PyramidRescaleMatlab(const std::string& name) :
		TemplatedParameteredObject<T>("PyramidRescaleMatlab", name,
			"MATLAB (imresize)-like Rescaling for pyramid-based flow-estimation algorithms.")
{
	ParameteredObject::_addInputSlot(
			seqIn, "seqIn", "sequence input", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			flowIn, "flowIn", "flow input", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			level, "level", "level select (from small to larger scales)");
	ParameteredObject::_addOutputSlot(
			seqOut, "seqOut", "sequence output", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			flowOut, "flowOut", "flow output", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			size, "size", "current size", "Roi<int>*");
	ParameteredObject::_addParameter (
			scaleFactor, "scaleFactor", "scale factor", 0.5);
	ParameteredObject::_addParameter (
			levels, "levels", "scale levels", 5u);
	ParameteredObject::_addParameter (
			sigma, "sigma", "sigma used to blur before downsampling", 0.7);
	ParameteredObject::_addParameter (
			interpolation, "interpolation",
			"interpolation type (see CImg::resize() documentation)", 3);
}

template <typename T>
void PyramidRescaleMatlab<T>::execute() {
	size() = &_size;

	cimg_library::CImgList<T> si = seqIn();
	cimg_library::CImgList<T>& so = seqOut();

	cimg_library::CImgList<T> fi = flowIn();
	cimg_library::CImgList<T>& fo = flowOut();

	const int flowN = fi.size();
	const int flowZ = fi[0].depth();
	const int flowC = fi[0].spectrum();

	const unsigned int curL = level();
	const unsigned int endL = levels();
	const unsigned int maxL = endL-1;
	const unsigned int stepsDown = maxL-curL;

	if(curL > maxL) {
		std::ostringstream msg;
		msg << "current level too large: cur=" << curL << "; max=" << maxL;
		ParameteredObject::raise(msg.str());
	}

	// input sequence remains unscaled over time and is the pyramid base
	const int sx = si[0].width(), sy = si[0].height();

	// target sizes
	const double _scaleFactor = scaleFactor();
	const double _scaleInverse = T(1.0) / _scaleFactor;
	const double shrink = std::pow(scaleFactor(),(double)stepsDown);
	const int tx = _size.xEnd = sx * shrink;
	const int ty = _size.yEnd = sy * shrink;

	// compute filter mask for blurring
	cimg_library::CImg<T> filterMask = _computeFilterMask( sigma() );

	// rescale sequence
	cimg_library::CImgList<T> tmp, tmp2;
	tmp = si;
	for (unsigned int i=0; i<stepsDown; i++) {
		tmp2 = cimg_library::CImgList<T>( tmp.size(),
		                                  tmp[0].width()*_scaleFactor, tmp[0].height()*_scaleFactor,
		                                  tmp[0].depth(), tmp[0].spectrum() );
		cimglist_for(tmp,kk) {
			tmp.at(kk).correlate(filterMask);
			cimg_forXYZC( tmp2.at(kk), x, y, z, c )
			{
				tmp2[kk].atXYZC( x, y, z, c )
				= tmp[kk].atXYZC( int((double(x+0.5))*_scaleInverse),
				                  int((double(y+0.5))*_scaleInverse), z, c );
			}
		}
		tmp = tmp2;
	}
	so = tmp;

	// rescale flow
	fo = fi;
	if(fo.is_sameXY(si)) {
		// scale down (initial guess)
#ifndef NDEBUG
		sout << "\t" << "scaling down to " << tx << "x" << ty << std::endl;
#endif
		fo = cimg_library::CImgList<T>( flowN, tx, ty, flowZ, flowC );
		cimglist_for(fo,kk) {
			assert(fo.at(kk).is_sameXY(sx,sy));
			fi.at(kk).correlate(filterMask);
			cimg_forXYZC( so.at(kk), x, y, z, c )
			{
				fo[kk].atXYZC( x, y, z, c ) = fi[kk].atXYZC( int((double(x)+0.5)*_scaleInverse),
				                                             int((double(y)+0.5)*_scaleInverse), z, c );
			}
			fo.at(kk) *= shrink;
		}
	}
	else {
		// scale up last result
#ifndef NDEBUG
		sout << "\t" << "scaling up to "
				<< tx << "x" << ty << ": got "
				<< fo[0].width() << "x" << fo[0].height()
				<< " expected: "
				<< std::fixed << std::setprecision(0)
				<< tx*scaleFactor() << "x" << ty*scaleFactor()
				<< std::endl;
#endif
		cimglist_for(fo,kk) {
			fo.at(kk).resize(tx,ty,-100,-100,interpolation());
			fo.at(kk) /= scaleFactor();
		}
	}
}

template<typename T>
cimg_library::CImg<T> PyramidRescaleMatlab<T>::_computeFilterMask( T smooth_sigma )
{
	int d = 2*int(1.5*smooth_sigma + 0.5) + 1;
	int r = d/2;
	cimg_library::CImg<T> mask( d, d, 1, 1 );

	T sum = 0.0;
	cimg_forXY( mask, x, y )
	{
		mask.atXY(x,y) = _gauss( double(pow(double(pow(double(x-r),2)+pow(double(y-r),2)),0.5)), T(0.0), T(smooth_sigma) );
		sum += mask.atXY(x,y);
	}
	cimg_forXY( mask, x, y )
	{
		mask.atXY(x,y) /= sum;
	}

	mask = cimg_library::CImg<T>( 1, 1, 1, 1 );
	mask(0,0,0,0) = double(1);
	mask.save("mask.cimg");
	return mask;
}

template<typename T>
T PyramidRescaleMatlab<T>::_gauss( T x, T mu, T sigma )
{
	return 1/(sqrt(2*M_PI*sigma*sigma))
	  * exp(double(-(x - mu)*(x - mu)/(2*sigma*sigma))) ;
}

#endif /* _PYRAMID_RESCALE_MATLAB_HXX_ */

