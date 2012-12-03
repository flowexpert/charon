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
/// \file ColorMask.hxx
/// Implementation of the parameter class ColorMask.
/// \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
///     Jens-Malte Gottfried</a>
/// \date 27.11.2010

#ifndef COLORMASK_HXX_
#define COLORMASK_HXX_

#include "ColorMask.h"
#include <charon-core/ParameteredObject.hxx>

template<typename T>
ColorMask<T>::ColorMask(const std::string& name) :
		TemplatedParameteredObject<T>(
				"ColorMask", name,
				"Convert gray values into false colors using a color mask"
				"<br><br>"
				"Assigns red to negative values and blue to positive values")
{
	ParameteredObject::_setTags("charon-utils;CImg") ;

	ParameteredObject::_addParameter<std::string>(
				colorMask, "colorMask", "color mask selection",
				"RB","{RB;Rainbow}");
	ParameteredObject::_addParameter(
			fullVal, "fullVal", "values with abs larger than fullVal are "
			"assigned to red/blue (0=auto)", "T");
	ParameteredObject::_addInputSlot(
			in, "in", "image data input (gray values) [t](x,y,z,1)",
			"CImgList<T>");
	ParameteredObject::_addOutputSlot(
			out, "out", "RGB data output [t](x,y,z,c)",
			"CImgList<T>");

	_gamma = new unsigned short[2048];
	for(unsigned int i = 0; i < 2048; i++) {
		float v = (float)i/2048.f;
		v = v*v*v*6;
		_gamma[i] = v*6*256;
	}
}

template<typename T>
ColorMask<T>::~ColorMask() {
	delete _gamma;
}

template<typename T>
void ColorMask<T>::execute() {
	if (in().size() == 0)
		throw std::invalid_argument("Empty sequence given!");
	if(in()[0].spectrum() != 1u)
		throw std::invalid_argument("Input data have to be gray values!");
	const cimg_library::CImgList<T>& i = in();

	// value to be considered as full red/blue
	_max = i.get_append('c').get_abs().max();
	if(fullVal() > 0)
		_max = fullVal();

	cimg_library::CImgList<T>& o = out();
	o.assign(i.size(),i[0].width(), i[0].height(), i[0].depth(), 3u);

	if (colorMask() == "RB") {
		// red/blue color mask
		cimg_library::CImg<double> col(1u,1u,1u,3u,0);
		cimglist_for(i,t) {
			cimg_forXYZ(i[t],x,y,z) {
				_applyMask(i(t,x,y,z), col);
				o(t,x,y,z,0u) = col(0u,0u,0u,0u);
				o(t,x,y,z,1u) = col(0u,0u,0u,1u);
				o(t,x,y,z,2u) = col(0u,0u,0u,2u);
			}
		}
	}
	else if (colorMask() == "Rainbow") {
		// rainbow color mask
		if (typeid(T) != typeid(int)) {
			sout << "(WW) rainbow color mask optimized for "
					"10 bit int values!" << std::endl;
		}
		cimg_library::CImg<unsigned char> col(1u,1u,1u,3u,0);
		cimglist_for(i,t) {
			cimg_forXYZ(i[t],x,y,z) {
				_rainbowMask(i(t,x,y,z), col);
				o(t,x,y,z,0u) = col(0u,0u,0u,0u);
				o(t,x,y,z,1u) = col(0u,0u,0u,1u);
				o(t,x,y,z,2u) = col(0u,0u,0u,2u);
			}
		}
	}
	else {
		ParameteredObject::raise("invalid color mask given: " + colorMask());
	}
}

template<typename T>
void ColorMask<T>::_applyMask(T val, cimg_library::CImg<double>& res) const {
	assert(res.is_sameXYZC(1u,1u,1u,3u));

	double v = 1. - std::abs((double)val/(double)_max);
	if (v < 0.) v = 0.;

	// if val=0, this will be white, otherwise decreasing to
	// red (if val<=-_max) or blue (if val>=_max)
	res(0u,0u,0u,0u) = val>0 ? v : 1.;
	res(0u,0u,0u,1u) =         v;
	res(0u,0u,0u,2u) = val<0 ? v : 1;
	res *= 255.;
}

template<typename T>
void ColorMask<T>::_rainbowMask(
		unsigned short val, cimg_library::CImg<unsigned char> &res) const {
	assert(res.is_sameXYZC(1u,1u,1u,3u));

	// pseudo color coding
	// this has been taken from OpenKinect OpenGL example
	// see http://openkinect.org/wiki/C%2B%2B_GL_Example
	// only the lower 10 bits are considered
	if (val >= 0x7ff) {
		val = 0x7ff;
	}
	unsigned short pval = _gamma[val];
	unsigned char lb = pval & 0xff;
	switch (pval>>8) {
	case 0:
		res(0u,0u,0u,0u) = 255;
		res(0u,0u,0u,1u) = 255-lb;
		res(0u,0u,0u,2u) = 255-lb;
		break;
	case 1:
		res(0u,0u,0u,0u) = 255;
		res(0u,0u,0u,1u) = lb;
		res(0u,0u,0u,2u) = 0;
		break;
	case 2:
		res(0u,0u,0u,0u) = 255-lb;
		res(0u,0u,0u,1u) = 255;
		res(0u,0u,0u,2u) = 0;
		break;
	case 3:
		res(0u,0u,0u,0u) = 0;
		res(0u,0u,0u,1u) = 255;
		res(0u,0u,0u,2u) = lb;
		break;
	case 4:
		res(0u,0u,0u,0u) = 0;
		res(0u,0u,0u,1u) = 255-lb;
		res(0u,0u,0u,2u) = 255;
		break;
	case 5:
		res(0u,0u,0u,0u) = 0;
		res(0u,0u,0u,1u) = 0;
		res(0u,0u,0u,2u) = 255-lb;
		break;
	default:
		res(0u,0u,0u,0u) = 0;
		res(0u,0u,0u,1u) = 0;
		res(0u,0u,0u,2u) = 0;
		break;
	}
}

#endif // COLORMASK_HXX_

