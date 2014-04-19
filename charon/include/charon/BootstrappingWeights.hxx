/*  Copyright (C) 2013
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
/** \file BootstrappingWeights.hxx
 *  Implementation of the parameter class BootstrappingWeights.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 20.03.2013
 */

#ifndef _BOOTSTRAPPINGWEIGHTS_HXX_
#define _BOOTSTRAPPINGWEIGHTS_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "BootstrappingWeights.h"

#include <stdlib.h>
#include <time.h>

template <typename T>
BootstrappingWeights<T>::BootstrappingWeights(const std::string& name) :
		TemplatedParameteredObject<T>(
			"BootstrappingWeights", name,
			"This module computes so called bootstrapping weights.<br>"
			"Detailed documentation can be found within the paper<br>"
			"<b>Bootstrap Optical Flow Confidence and Uncertainty Measure</b><br>"
			"by <i>Jan Kybic</i> and <i>Claudia Nieuwenhuis</i> (2011).<br>"
			"This module expects the image volume dimensions (width, heigth and depth). "
			"It returns a CImgList<T> of weighting images (weighting masks) whereas N=iterations. "
			"The idea behind bootstrapping is to start with a weighting image that is zero "
			"everywhere and then add a weight of 1.0 and at most 1.0 to randomly sampled positions within "
			"each iteration. Here, the parameter samples corresponds to the number of randomly chosen "
			"samples, whereas iterations denotes the total bootstrapping iterations being performed. "
			"After each iteration, its corresponding weighting mask is being stored in the returned CImgList<T>. "
			"Typically, this module is used in connection with the SelectFromList module "
			"and appropriate ChannelConverter_s, such that individual masks can be used as "
			"inputs for the lambdaMask input slots within the stencils."
		)
{
	ParameteredObject::_addOutputSlot(
		weights, "weights",
		"bootstrapping weights",
		"CImgList<T>");

	ParameteredObject::_addParameter< unsigned int >(
		width, "width",
		"mask width",
		0, "uint");
	ParameteredObject::_addParameter< unsigned int >(
		height, "height",
		"mask height",
		0, "uint");
	ParameteredObject::_addParameter< unsigned int >(
		depth, "depth",
		"mask depth",
		0, "uint");
	ParameteredObject::_addParameter< unsigned int >(
		samples, "samples",
		"number of samples",
		0, "uint");
	ParameteredObject::_addParameter< unsigned int >(
		iterations, "iterations",
		"number of iterations",
		0, "uint");

}

template <typename T>
void BootstrappingWeights<T>::execute()
{
	unsigned int _width = width();
	unsigned int _height = height();
	unsigned int _depth = depth();
	unsigned int _samples = samples();
	unsigned int _iterations = iterations();

	// create output image
	cimg_library::CImgList<T> &_weights = weights();
	_weights = cimg_library::CImgList<T>( _iterations, _width, _height, _depth, 1, T(0.0) );

	unsigned int rand_x = 0, rand_y = 0, rand_z = 0;
	srand(time(NULL));

	cimg_library::CImgList<bool> visited( 1, _width, _height, _depth, 1, false );
	for (unsigned int i=0; i<_iterations; i++ ) {
		// reset visited array
		cimg_forXYZC( visited[0], x, y, z, c ) {
			visited.atNXYZC(0,x,y,z,0) = false;
		}

		if (i>0) {
			cimg_forXYZC( _weights[i], x, y, z, c ) {
				_weights.atNXYZC(i,x,y,z,c) = _weights.atNXYZC(i-1,x,y,z,c);
			}
		}

		for (unsigned int s=0; s<_samples; ) {
			rand_x = floor( ((double)rand() / (double)RAND_MAX) * _width);
			rand_y = floor( ((double)rand() / (double)RAND_MAX) * _height);
			rand_z = floor( ((double)rand() / (double)RAND_MAX) * _depth);

			if (!visited.atNXYZC( 0, rand_x, rand_y, rand_z, 0 )) {
				visited.atNXYZC( 0, rand_x, rand_y, rand_z, 0 ) = true;
				_weights.atNXYZC( i, rand_x, rand_y, rand_z, 0 ) += T(1.0);
				s++;
			}
		}
	}
}

#endif /* _BOOTSTRAPPINGWEIGHTS_HXX_ */

