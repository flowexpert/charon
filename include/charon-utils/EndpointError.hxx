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
/// \file EndpointError.hxx
/// Implementation of the parameter class EndpointError.
/// \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
///     Michael Baron</a>
/// \date 18.01.2012

#ifndef ENDPOINTERROR_HXX_
#define ENDPOINTERROR_HXX_

#include <cassert>
#include <vector>
#include <algorithm>
#include <charon-core/ParameteredObject.hxx>
#include "EndpointError.h"

template<typename T>
EndpointError<T>::EndpointError(const std::string& name) :
		TemplatedParameteredObject<T>("EndpointError", name,
				"Compute Endpoint Error between flow fields")
{
	ParameteredObject::_addInputSlot(
			flow, "flow",
	                "flow input", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			groundTruth, "groundTruth",
	                "ground truth input", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			out, "out", "hsv representation output [t](x,y,z,c)",
			"CImgList<T>");
}

template<typename T>
void EndpointError<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	// copy input image for manipulations
	const cimg_library::CImgList<T>& i1 = flow();
	const cimg_library::CImgList<T>& i2 = groundTruth();
	cimg_library::CImgList<T>& o = out();

	cimg_library::CImg<T> tmp( i1[0].width(), i1[0].height(), i1[0].depth(),
	                           i1[0].spectrum() );

	T sum;
	T delta;
	unsigned int pixelCnt = 0;
	T sumsum = T(0);
	cimg_forXYZC(tmp,x,y,z,t) {
		sum = T(0);
		for (unsigned int n=0; n<i1.size(); ++n) {
			delta = fabs( double(i1(n,x,y,z,t) - i2(n,x,y,z,t)) );
			if (delta > 1e9) delta = 0;
			sum += pow( double(delta), 2 );
		}
		tmp(x,y,z,t) = pow( double(sum), double(1.0/2) );
		sumsum += tmp(x,y,z,t);
		++pixelCnt;
	}

	sout << "(II) EndpointError :: Mean Endpoint Error = ";
	sout << ((1.0*sumsum) / pixelCnt) << std::endl;

	o = cimg_library::CImgList<T>( tmp );
}

#endif

