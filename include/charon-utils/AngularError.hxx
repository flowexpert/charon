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
/// \file AngularError.hxx
/// Implementation of the parameter class AngularError.
/// \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
///     Michael Baron</a>
/// \date 03.04.2012

#ifndef ANGULARERROR_HXX_
#define ANGULARERROR_HXX_

#ifndef _USE_MATH_DEFINES
	#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <charon-core/ParameteredObject.hxx>
#include "AngularError.h"

template<typename T>
AngularError<T>::AngularError(const std::string& name) :
		TemplatedParameteredObject<T>("AngularError", name,
				"Compute Angular Error between two-dimensional flow fields")
{
	ParameteredObject::_addInputSlot(
			flow, "flow",
	                "flow input", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			groundTruth, "groundTruth",
	                "ground truth input", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			out, "out", "angular error output",
			"CImgList<T>");
}

template<typename T>
void AngularError<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	// copy input image for manipulations
	const cimg_library::CImgList<T>& i1 = flow();
	const cimg_library::CImgList<T>& i2 = groundTruth();
	cimg_library::CImgList<T>& o = out();

	cimg_library::CImg<T> tmp( i1[0].width(), i1[0].height(), i1[0].depth(),
	                           i1[0].spectrum() );

	T angle1, angle2;
	T angularError;

	T sum = T(0);
	unsigned int pixelCnt = 0;
	cimg_forXY(tmp,x,y) {

		angle1 = atan2(fabs(double(i1(1,x,y,0,0))), fabs(double(i1(0,x,y,0,0))));
		angle2 = atan2(fabs(double(i2(1,x,y,0,0))), fabs(double(i2(0,x,y,0,0))));
		angularError  = fabs(double(angle1 - angle2));

		angularError /= M_PI;
		angularError *= T(180);
		tmp(x,y,0,0) = angularError;
		sum += angularError;
		++pixelCnt;
	}

	sout << "(II) AngularError :: Mean Angular Error = ";
	sout << (sum / pixelCnt) << std::endl;

	o = cimg_library::CImgList<T>( tmp );
}

#endif

