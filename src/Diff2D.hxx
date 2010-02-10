/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/** \file Diff2D.hxx
 *  Implementation of the parameter class Diff2D.
 *  \author Jens-Malte Gottfried
 *  \date 10.02.2010
 */

#ifndef _DIFF2D_HXX_
#define _DIFF2D_HXX_

#include "Diff2D.h"

template <typename T>
Diff2D<T>::Diff2D(const std::string& name) :
		TemplatedParameteredObject<T>("Diff2D", name,
			"Calculate 2D derivatives. <br>  <br> This plugin serves to simplify calculations of 2D derivations. <br> Although derivatives may be calculated using class LinearFilter, this plugin <br> makes derivative calculations more compact. <br> Only the derivatives of connected output slots are calculated.")
{
	ParameteredObject::_addParameter (diffMaskXY, "diffMaskXY", "linear 1D filter mask used for derivation wrt x,y", );
	ParameteredObject::_addParameter (smoothMaskXY, "smoothMaskXY", "linear 1D filter mask applied orthogonal of derivation (wrt x,y)", );
	ParameteredObject::_addParameter (diffMaskT, "diffMaskT", "linear 1D filter mask used for derivation wrt t", );
	

	ParameteredObject::_addInputSlot(img, "img", "image input", "CImgList<T>"); 
	ParameteredObject::_addInputSlot(count, "count", "if connected, calculations are performed on count change only", "unsigned int"); 
	ParameteredObject::_addOutputSlot(dx, "dx", "derivative wrt x", "CImgList<T>"); 
	ParameteredObject::_addOutputSlot(dy, "dy", "derivative wrt y", "CImgList<T>"); 
	ParameteredObject::_addOutputSlot(dt, "dt", "derivative wrt t", "CImgList<T>"); 
	ParameteredObject::_addOutputSlot(dxx, "dxx", "second derivative wrt x", "CImgList<T>"); 
	ParameteredObject::_addOutputSlot(dxy, "dxy", "second derivative wrt x,y", "CImgList<T>"); 
	ParameteredObject::_addOutputSlot(dyy, "dyy", "second derivative wrt y", "CImgList<T>"); 
	
}

template <typename T>
void Diff2D<T>::execute() {
	ParameteredObject::execute();

	// your code goes here :-)
}

#endif /* _DIFF2D_HXX_ */



