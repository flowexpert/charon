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
			"MATLAB (imresize)-like Rescaling for pyramid-based flow-estimation algorithms. "
			"The idea is to offer a module, which works _exactly_ as the pyramid rescaling "
			"approach proposed by D. Sun within his Classic+NL optical flow method. "
			"It utilizes a sequence of successive Gaussian smoothing and sampling steps, "
			"whereas sampling furthermore utilizes the concept of bilinear interpolation. "
			"Still existing problems are a wrong border-handling within the smoothing step, "
			"and a still wrong bilinear interpolation."
		)
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
			"interpolation type (used for flow only! See CImg::resize() documentation)", 3);

	ParameteredObject::_setTags("charon-flow;MultiscaleApproaches;CImg");
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
	const int tx = _size.xEnd = std::floor( sx * shrink + 0.5);
	const int ty = _size.yEnd = std::floor( sy * shrink +0.5);

	// compute filter mask for blurring
	cimg_library::CImg<T> filterMask = _computeFilterMask( T(sigma()) );

	// rescale sequence
	cimg_library::CImgList<T> tmp, tmp2;
	tmp = si;
	for (unsigned int i=0; i<stepsDown; i++) {
		tmp2 = cimg_library::CImgList<T>( tmp.size(),
		                                  std::floor( tmp[0].width()*_scaleFactor +0.5), std::floor( tmp[0].height()*_scaleFactor + 0.5),
		                                  tmp[0].depth(), tmp[0].spectrum() );
		cimglist_for(tmp,kk) {
			tmp.at(kk).correlate(filterMask);
			cimg_forXYZC( tmp2.at(kk), x, y, z, c )
			{
				tmp2[kk].atXYZC( x, y, z, c )
				= _bilinearInterpolation( tmp[kk], x*_scaleInverse, y*_scaleInverse ) ;
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
		sout << "\t" << "scaling down flow to " << tx << "x" << ty << std::endl;
#endif
		fo = cimg_library::CImgList<T>( flowN, tx, ty, flowZ, flowC );
		cimglist_for(fo,kk) {
//			assert(fo.at(kk).is_sameXY(sx,sy));
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
		sout << "\t" << "scaling up flow to "
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

	T sum = T(0.0);
	cimg_forXY( mask, x, y )
	{
		mask.atXY(x,y) = _gauss( double(pow(double(pow(double(x-r),2.0)+pow(double(y-r),2)),0.5)), T(0.0), T(smooth_sigma) );
		sum += mask.atXY(x,y);
	}
	cimg_forXY( mask, x, y )
	{
		mask.atXY(x,y) /= sum;
	}

	return mask;
}

template<typename T>
T PyramidRescaleMatlab<T>::_gauss( T x, T mu, T sigma )
{
	return T(1.0/(sqrt(2.0*M_PI*sigma*sigma))
	  * exp(double(-(x - mu)*(x - mu)/(2.0*sigma*sigma)))) ;
}

template<typename T>
T PyramidRescaleMatlab<T>::_bilinearInterpolation( cimg_library::CImg<T> data, T x, T y )
{
	unsigned int x0 = floor(double(x));
	unsigned int x1 = ceil(double(x));
	unsigned int y0 = floor(double(y));
	unsigned int y1 = ceil(double(y));

	if (x0 == x1) x1++;
	if (y0 == y1) y1++;

	T Q_00 = data.atXY(x0,y0);
	T Q_01 = data.atXY(x0,y1);
	T Q_10 = data.atXY(x1,y0);
	T Q_11 = data.atXY(x1,y1);

	T q = Q_00 * (x1-x)*(y1-y)
	    + Q_01 * (x1-x)*(y-y0)
	    + Q_10 * (x-x0)*(y1-y)
	    + Q_11 * (x-x0)*(y-y0) ;

	return q;
}

#endif /* _PYRAMID_RESCALE_MATLAB_HXX_ */

