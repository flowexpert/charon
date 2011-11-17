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
/// @file Resize.hxx
/** Implementation of the parameter class Resize.
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date 28.04.2010
 */

#ifndef _RESIZE_HXX_
#define _RESIZE_HXX_

#include "Resize.h"

template <typename T>
Resize<T>::Resize(const std::string& name) :
		TemplatedParameteredObject<T>("resize", name, "resize images")
{
	this->_addInputSlot (
			roi, "roi",
			"new size (only width, height etc are considered)", "Roi<int>*");
	this->_addInputSlot (in,    "in",    "image input",     "CImgList<T>");
	this->_addOutputSlot(out,   "out",   "image output",    "CImgList<T>");

	this->_addParameter(x, "x", "resize in x direction", true);
	this->_addParameter(y, "y", "resize in y direction", true);
	this->_addParameter(z, "z", "resize in z direction", false);
	this->_addParameter(t, "t", "resize in t direction", false);

	this->_addParameter(
			method, "method",
			"resize method <br><br>"
			"-1 = no interpolation : raw memory resizing. "
				"when expanding, image will be filled with "
				"uninitialized values!<br>"
			"0 = no interpolation : additional space is filled "
				"according to border_condition. "
				"default is dirichlet -> filled with zeros<br>"
			"1 = nearest-neighbor interpolation.<br>"
			"2 = moving average interpolation.<br>"
			"3 = linear interpolation<br>."
			"4 = grid interpolation.<br>"
			"5 = bicubic interpolation.<br>"
			"6 = lanczos interpolation.<br>", 1);

	this->_addParameter(
			preBlurFactor, "preBlurFactor",
			"control blur strength on image size reduction", 0.4f);
}

template <typename T>
void Resize<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	assert(in().size() > 0);

	int
		size_x = x() ? roi()->getWidth()    : -100,
		size_y = y() ? roi()->getHeight()   : -100,
		size_z = z() ? roi()->getDepth()    : -100,
		size_t = t() ? roi()->getDuration() : -100;

	if(x() && size_x == 0) {
		sout << "\tleaving x dimension untouched" << std::endl;
		size_x = -100;
	}
	if(y() && size_y == 0) {
		sout << "\tleaving y dimension untouched" << std::endl;
		size_y = -100;
	}
	if(z() && size_z == 0) {
		sout << "\tleaving z dimension untouched" << std::endl;
		size_z = -100;
	}
	if(t() && size_t == 0) {
		sout << "\tleaving t dimension untouched" << std::endl;
		size_t = -100;
	}

	float sig_x = size_x > 0 ?
				(float)in()[0].width()/(float)size_x
					: 100.f/((float)-size_x);
	float sig_y = size_y > 0 ?
				(float)in()[0].height()/(float)size_y
					: 100.f/((float)-size_y);
	float sig_z = size_z > 0 ?
				(float)in()[0].depth()/(float)size_z
					: 100.f/((float)-size_z);

	sig_x *= preBlurFactor();
	sig_y *= preBlurFactor();
	sig_z *= preBlurFactor();

	bool blur_x = size_x >= 0 ? (in()[0].width()  > size_x) : (size_x > -100);
	bool blur_y = size_y >= 0 ? (in()[0].height() > size_y) : (size_y > -100);
	bool blur_z = size_z >= 0 ? (in()[0].depth()  > size_z) : (size_z > -100);

	out() = in();
	const int m = method();
	cimglist_for(out(),k) {
		if (blur_x) out()[k].deriche(sig_x, 0, 'x');
		if (blur_y) out()[k].deriche(sig_y, 0, 'y');
		if (blur_z) out()[k].deriche(sig_z, 0, 'z');
		out()[k].resize(size_x, size_y, size_z, size_t, m);
	}
}
#endif // _RESIZE_HXX_
