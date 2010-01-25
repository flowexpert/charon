/*  Copyright (C) 2009 Daniel Kondermann

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
/// @file SplineResize.hxx
/// Implementation of the parameter class SplineResize.
/// @author Daniel Kondermann
/// @date 20.01.2010

#ifndef _SPLINERESIZE_HXX_
#define _SPLINERESIZE_HXX_

#include "SplineResize.h"

/// Resize image using B-spline interpolation. 
/// The function implements separable spline interpolation algorithm described in
/// M. Unser, A. Aldroubi, M. Eden, "B-Spline Signal Processing" IEEE Transactions on Signal Processing, vol. 41, no. 2, pp. 821-833 (part I), pp. 834-848 (part II), 1993.
/// to obtain optimal interpolation quality and speed. The implementation ensures that image values are interpolated rather than smoothed by first calling a recursive (sharpening) prefilter as described in the above paper.
/// The input image must have a size of at least 4x4, the destination of at least 2x2. The scaling factors are then calculated accordingly. If the source image is larger than the destination, it is smoothed (band limited) using a recursive exponential filter.
template <typename T>
SplineResize<T>::SplineResize(const std::string& name) :
        TemplatedParameteredObject<T>("SplineResize", name,
            "Resize image using B-spline interpolation.  <br> The function implements separable spline interpolation algorithm described in <br> M. Unser, A. Aldroubi, M. Eden, 'B-Spline Signal Processing' IEEE Transactions on Signal Processing, vol. 41, no. 2, pp. 821-833 (part I), pp. 834-848 (part II), 1993. <br> to obtain optimal interpolation quality and speed. The implementation ensures that image values are interpolated rather than smoothed by first calling a recursive (sharpening) prefilter as described in the above paper. <br> The input image must have a size of at least 4x4, the destination of at least 2x2. The scaling factors are then calculated accordingly. If the source image is larger than the destination, it is smoothed (band limited) using a recursive exponential filter.")
{

	T defaultFactor =(T)1.0;
    ParameteredObject::_addParameter (factorX, "factorX", "Factor for resizing. If zero, the parameters newDim* are used."); //1.0
    ParameteredObject::_addParameter (factorY, "factorY", "Factor for resizing. If zero, the parameters newDim* are used.");
    ParameteredObject::_addParameter (factorZ, "factorZ", "Factor for resizing. If zero, the parameters newDim* are used.");
    ParameteredObject::_addParameter (factorV, "factorV", "Factor for resizing. If zero, the parameters newDim* are used.");
    ParameteredObject::_addParameter (factorT, "factorT", "Factor for resizing. If zero, the parameters newDim* are used.");
	ParameteredObject::_addParameter (newDimX, "newDimX", "Used if factorX=0. In this case the parameter factorX is ignored.");
	ParameteredObject::_addParameter (newDimY, "newDimY", "Used if factorY=0. In this case the parameter factorY is ignored.");
	ParameteredObject::_addParameter (newDimZ, "newDimZ", "Used if factorZ=0. In this case the parameter factorZ is ignored.");
	ParameteredObject::_addParameter (newDimV, "newDimV", "Used if factorT=0. In this case the parameter factorT is ignored.");
	ParameteredObject::_addParameter (newDimT, "newDimT", "Used if factorV=0. In this case the parameter factorV is ignored.");
	ParameteredObject::_addParameter (splineOrder, "splineOrder", "The order of the spline. Minimum is 3, maximum is 10.");


    ParameteredObject::_addInputSlot(in, "in", "The source image.", "vigra::MultiArrayView<5, T>"); 
	ParameteredObject::_addOutputSlot(out, "out", "The resized image.", "vigra::MultiArrayView<5, T>"); 
	

}

template <typename T>
void SplineResize<T>::execute() {
	ParameteredObject::execute();

    typedef vigra::MultiArray<5, T>::difference_type Shape;
	unsigned int dimx, dimy, dimz, dimv, dimt;
	//READ THIS! *harhar* :)
	dimx =(factorX == 0) ? ((newDimX == 0) ? in().size(0) : newDimX) : in().size(0)*factorX+0.5f;
	dimy =(factorY == 0) ? ((newDimY == 0) ? in().size(1) : newDimY) : in().size(1)*factorY+0.5f;
	dimz =(factorZ == 0) ? ((newDimZ == 0) ? in().size(2) : newDimZ) : in().size(2)*factorZ+0.5f;
	dimv =(factorV == 0) ? ((newDimV == 0) ? in().size(3) : newDimV) : in().size(3)*factorV+0.5f;
	dimt =(factorT == 0) ? ((newDimT == 0) ? in().size(4) : newDimT) : in().size(4)*factorT+0.5f;
	Shape src(in().size(0), in().size(1), in().size(2), in().size(3), in().size(4));
	Shape dst(dimx, dimy, dimz, dimv, dimt);

	result.reshape(dst);
	unsigned int i =4;
	vigra::MultiArrayView<5, T> srcView =result.view();
	vigra::MultiArrayView<5, T> scaleView =result.view();
	/*
	while(in().size(i) == 1)
	{
		srcView.bindOuter(0);
		scaleView.bindOuter(0);
		i--;
	}*/
	out() =result.view();
    if(splineOrder == 3)
		vigra::resizeMultiArraySplineInterpolation(vigra::srcMultiArrayRange(srcView.bindOuter(0).bindOuter(0)), destMultiArrayRange(scaleView.bindOuter(0).bindOuter(0)), vigra::BSpline<3, double>());
	else if(splineOrder == 4)
		vigra::resizeMultiArraySplineInterpolation(vigra::srcMultiArrayRange(srcView), destMultiArrayRange(scaleView), vigra::BSpline<4, double>());
	else if(splineOrder == 5)
		vigra::resizeMultiArraySplineInterpolation(vigra::srcMultiArrayRange(srcView), destMultiArrayRange(scaleView), vigra::BSpline<5, double>());
	else if(splineOrder == 6)
		vigra::resizeMultiArraySplineInterpolation(vigra::srcMultiArrayRange(srcView), destMultiArrayRange(scaleView), vigra::BSpline<6, double>());
	else if(splineOrder == 7)
		vigra::resizeMultiArraySplineInterpolation(vigra::srcMultiArrayRange(srcView), destMultiArrayRange(scaleView), vigra::BSpline<7, double>());
	else if(splineOrder == 8)
		vigra::resizeMultiArraySplineInterpolation(vigra::srcMultiArrayRange(srcView), destMultiArrayRange(scaleView), vigra::BSpline<8, double>());
	else if(splineOrder == 9)
		vigra::resizeMultiArraySplineInterpolation(vigra::srcMultiArrayRange(srcView), destMultiArrayRange(scaleView), vigra::BSpline<9, double>());
	else if(splineOrder == 10)
		vigra::resizeMultiArraySplineInterpolation(vigra::srcMultiArrayRange(srcView), destMultiArrayRange(scaleView), vigra::BSpline<10, double>());
	else
		throw "spline order not supported!"; //@TODO: throw a useful exception!
}

#endif /* _SPLINERESIZE_HXX_ */



