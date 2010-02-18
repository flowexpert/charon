/*  Copyright (C) 2009 Jens-Malte Gottfried <jmgottfried@web.de>

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
/** \file SequenceGenerator.hxx
 *  Implementation of the parameter class SequenceGenerator.
 *  \author Jens-Malte Gottfried <jmgottfried@web.de>
 *  \date 02.02.2010
 */

#ifndef _SEQUENCEGENERATOR_HXX_
#define _SEQUENCEGENERATOR_HXX_

#define _USE_MATH_DEFINES
#include "SequenceGenerator.h"
#include <cmath>

template <typename T>
SequenceGenerator<T>::SequenceGenerator(const std::string& name) :
		TemplatedParameteredObject<T>("SequenceGenerator", name,
			"Generate simple test sequences with ground truth <br><br>"
			"Currently, only sine sequences may be generated, but if needed, "
			"you can implement more. <br>"
			"Dimension and frequencies may be provided."),
		size("20;20;1;2"),
		wavelength("10;10"),
		velocities("0.4;-0.8")
{
	ParameteredObject::_addParameter (size, "size",
		"test sequence dimensions (x;y;z;t)");
	ParameteredObject::_addParameter (wavelength, "wavelength",
		"sine wavelength along dimensions (in pixel) (lx;ly[;lz])");
	ParameteredObject::_addParameter (velocities, "velocities",
		"constant flow velocity (u;v[;w])");
	ParameteredObject::_addParameter<std::string>(operation, "operation",
		"operation type of processing (add, mul)", "add", "string");
	ParameteredObject::_addOutputSlot(sequence, "sequence",
		"generated test sequence", "CImgList<T>");
	ParameteredObject::_addOutputSlot(flow, "flow",
		"ground truth", "CImgList<T>");
}

template <typename T>
void SequenceGenerator<T>::execute() {
	ParameteredObject::execute();

	// check preconditions
	assert(size().size() == 4u);
	assert(size()[0] >= 1u);
	assert(size()[1] >= 1u);
	assert(size()[2] >= 1u);
	assert(size()[3] >= 2u);
	assert(wavelength().size() >= 2u);
	assert(velocities().size() >= 2u);
	const bool isAdd = (operation() == "add");
	assert(isAdd || operation() == "mul");

	// determine parameters
	sequence().assign(1u,size()[0],size()[1],size()[2],size()[3]);
	double wx = 2*M_PI/wavelength()[0];
	double wy = 2*M_PI/wavelength()[1];
	double wz = 0.;
	double u  = velocities()[0];
	double v  = velocities()[1];
	double w  = 0.;

	// handle 3D test sequences
	if(size()[2] > 1u) {
		assert(velocities().size() == 3u);
		assert(wavelength().size() == 3u);
		wz = 2*M_PI/wavelength()[2];
		w  = velocities()[2];
		flow().assign(3u,size()[0],size()[1],size()[2],size()[3]-1);
	}
	else {
		assert(velocities().size() == 2u);
		assert(wavelength().size() == 2u);
		flow().assign(2u,size()[0],size()[1],1u,size()[3]-1);
	}

	// fill flow and sequence
	cimglist_for(flow(), kk)
		flow()[kk].fill(velocities()[kk]);
	cimg_forXYZC(sequence()[0],x,y,z,t)
			sequence()[0](x,y,z,t) = isAdd ?
				T(sin(wx*(x-u*t)) + sin(wy*(y-v*t)) + sin(wz*(z-w*t))) :
				// using cos for z-component to avoid zero flow in 2D case
				T(sin(wx*(x-u*t)) * sin(wy*(y-v*t)) * cos(wz*(z-w*t))) ;
}

#endif /* _SEQUENCEGENERATOR_HXX_ */



