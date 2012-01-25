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
/** \file Residual.hxx
 *  Implementation of the parameter class Residual.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 09.01.2012
 */

#ifndef _RESIDUAL_HXX_
#define _RESIDUAL_HXX_

#include "Residual.h"

#define EPS_INVERSE 1e9

template<typename T>
Residual<T>::Residual(const std::string& name) :
	TemplatedParameteredObject<T>(
			"Residual", name,
			"calculates residual<br><br>"
			"Takes two input images and both motion components "
			"and computes the motion-compensated intensity "
			"difference.")
{
	this->_addInputSlot(img1, "img1", "image 1 input", "CImgList<T>");
	this->_addInputSlot(img2, "img2", "image 2 input", "CImgList<T>");
	this->_addInputSlot(motU, "motU", "horizontal motion", "CImgList<T>");
	this->_addInputSlot(motV, "motV", "vertical motion", "CImgList<T>");
	this->_addOutputSlot(out,  "out",   "residual output", "CImgList<T>");

	this->_addParameter(inverseResidual, "inverseResidual",
	                    "invert residual output", false);
}

template<typename T>
void Residual<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	bool inv = inverseResidual();

	const cimg_library::CImgList<T>& _img1 = img1();
	const cimg_library::CImgList<T>& _img2 = img2();
	const cimg_library::CImgList<T>& _motU = motU();
	const cimg_library::CImgList<T>& _motV = motV();

	cimg_library::CImg<T> outimg( _img1[0].width(), _img1[0].height(),
	                              _img1[0].depth(), _img1[0].spectrum() );
	cimg_library::CImgList<T> _out( outimg );

	T tmp, mU, mV;
	for (int j=0; j<_img1[0].width(); ++j)
	for (int i=0; i<_img1[0].height(); ++i)
	{
		mU = _motU.atNXYZC(0, j, i, 0, 0);
		mV = _motV.atNXYZC(0, j, i, 0, 0);
		tmp = _img2.atNXYZC(0, j+mU, i+mV, 0, 0)
		    - _img1.atNXYZC(0, j, i, 0, 0);
		if (inv) {
//			if (tmp) {
//				_out.atNXYZC(0, j, i, 0, 0) = 1/(tmp*tmp);
//			} else {
//				_out.atNXYZC(0, j, i, 0, 0) = EPS_INVERSE;
//			}
			_out.atNXYZC(0, j, i, 0, 0) = exp( double(-(tmp*tmp)) );
		} else {
			_out.atNXYZC(0, j, i, 0, 0) = tmp*tmp;
		}
	}

	out() = _out;
}

#endif /* _RESIDUAL_HXX_ */

