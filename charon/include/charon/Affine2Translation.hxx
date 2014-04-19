/*  Copyright (C) 2013
                  University of Heidelberg (IWR/HCI)

	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/// \file Affine2Translation.hxx
/// Implementation of the parameter class Affine2Translation.
/// \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
///   Michael Baron</a>
/// \date 11.02.2013

#ifndef AFFINE2TRANSLATION_HXX_
#define AFFINE2TRANSLATION_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "Affine2Translation.h"

template<typename T>
Affine2Translation<T>::Affine2Translation(const std::string& name) :
		TemplatedParameteredObject<T>("Affine2Translation", name,
				"This module converts the six affine motion parameters "
				"(four rotation parameters a11, ..., a22 and two translation "
				"parameters b1, b2) to two translation parameters u and v. "
				"Here, the computation is performed as described in the "
				"paper Over-Parameterized Variational Optical Flow (Tal Nir, 2007). "
				"Especially, this means, that all rotations are being carried "
				"out w.r.t. to the image center. "
				"This module expects a CImgList<T> with N=6 in the order "
				"a11, a12, a21, a22, b1, b2 and produces a CImgList with N=2 "
				"in the order u, v. "
				"The module is mainly used in order to obtain a parameter set "
				"suitable for the warping step, which relies on a pure translation.")
{
	ParameteredObject::_setTags("charon;CImg") ;

	ParameteredObject::_addInputSlot(
			parameters, "parameters", "parameter input", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			roi, "roi", "region of interest", "Roi<int>*");
	ParameteredObject::_addOutputSlot(
			flow, "flow", "flow output",
			"CImgList<T>");
}

template<typename T>
void Affine2Translation<T>::execute()
{
	const Roi<int>& _roi = *(this->roi());
	int _xBegin = _roi.xBegin();
	int _xEnd = _roi.xEnd();
	int _yBegin = _roi.yBegin();
	int _yEnd = _roi.yEnd();

	int _width = parameters()[0].width();
	int _height = parameters()[0].height();
	int _depth = parameters()[0].depth();
	int _spectrum = parameters()[0].spectrum();

	const cimg_library::CImgList<T> &_params = parameters();
	cimg_library::CImgList<T> &_flow = flow();
	_flow = cimg_library::CImgList<T>( 2, _width, _height, _depth, _spectrum );

	T rho = 0.858;
	T x0 = (_xEnd - _xBegin) / 2.0;
	T y0 = (_yEnd - _yBegin) / 2.0;
	T xn = 0.0;
	T yn = 0.0;
	cimg_forXY( _flow[0], x, y )
	{
		xn = rho * (double(x) - x0) / x0;
		yn = rho * (double(y) - y0) / y0;
		// u = a11 * xn + a12 * yn + b1
		_flow[0].atXY( x, y ) = _params[0].atXY(x,y) * xn + _params[1].atXY(x,y) * yn + _params[4].atXY(x,y);
		// v = a21 * xn + a22 * yn + b2
		_flow[1].atXY( x, y ) = _params[2].atXY(x,y) * xn + _params[3].atXY(x,y) * yn + _params[5].atXY(x,y);
	}
}

#endif // AFFINE2TRANSLATION_HXX_

