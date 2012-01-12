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
/// \file Rgb2Hsv.hxx
/// Implementation of the parameter class Rgb2Hsv.
/// \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
///     Jens-Malte Gottfried</a>
/// \date 02.11.2010

#ifndef _RGB2HSV_HXX_
#define _RGB2HSV_HXX_

#include "Rgb2Hsv.h"

template <typename T>
Rgb2Hsv<T>::Rgb2Hsv(const std::string& name) :
	TemplatedParameteredObject<T>("Rgb2Hsv", name,
		"convert RGB color images to HSV representation") {
	this->_addInputSlot (in,     "in",     "image input",  "CImgList<T>");
	this->_addOutputSlot(out,    "out",    "image output", "CImgList<T>");
}

template <typename T>
void Rgb2Hsv<T>::execute() {
	out = in;
	cimglist_for(out(), i)
		out()[i] = in()[i].get_RGBtoHSV();
}

#endif /* _RGB2HSV_HXX_ */
