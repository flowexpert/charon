/*  Copyright (C) 2011 Julian Coordts

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
/** \file GammaCorrection.hxx
 *  Implementation of the class GammaCorrection.
 *  \author Julian Coordts
 *  \date 13.01.2012
 */

#ifndef _GAMMACORRECTION_HXX_
#define _GAMMACORRECTION_HXX_

#include "GammaCorrection.h"

template <typename T>
GammaCorrection<T>::GammaCorrection(const std::string& name) :
        TemplatedParameteredObject<T>("gammacorrection", name,
			"Adjust gamma curve of input image.<br>"
			"output = input^gamma * (maxColorVal^(gamma - 1).")
{
	ParameteredObject::_setTags("charon-utils;CImg") ;

	ParameteredObject::_addParameter (_maxColorValue, "maxcolorvalue", 
		"the color value range of the input<br>"
		"When set to zero, this will be set to the maximum of the input image",
		255.0) ;
	
	ParameteredObject::_addParameter (_gamma, "gamma", 
		"gamma (set to 1 for passthrough)", 0.5) ;
	
	ParameteredObject::_addInputSlot(
				_input, "input", "input image", "CImgList<T>") ;
	
	ParameteredObject::_addOutputSlot(_output, "output", 
		"output image (dimensions are unchanged)", "CImgList<T>"); 
}

template <typename T>
void GammaCorrection<T>::execute() {
	using namespace cimg_library ;

	const CImgList<T>& input = _input() ; 
	CImgList<T>& output = _output() ; //similiar for output slots
	double gamma= _gamma();
	double maxColorVal = _maxColorValue();
	
	//find maximum pixel value
	if(maxColorVal == 0.0) {
		cimglist_for(input,l) {
			double cMax = input(l).max() ;
			if(l == 0)
				maxColorVal = cMax ;
			maxColorVal = (cMax > maxColorVal ? cMax : maxColorVal) ;
		}
	}

	output.assign(input);

	cimglist_for(output,l) {
		output[l].pow(gamma);
		output[l] *= pow(maxColorVal, 1.0-gamma); //scale the image brighter
	}
}

#endif /* _GammaCorrection_HXX_ */
