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

#ifndef _CROP_HXX_
#define _CROP_HXX_

#include "Resize.h"

template <typename T>
Resize<T>::Resize(const std::string& name) :
		TemplatedParameteredObject<T>("resize", name, "resize images")
{
	this->_addInputSlot (
			roi, "roi",
			"new size (only width, height etc are considered)", "Roi<int>");
	this->_addInputSlot (in,    "in",    "image input",     "CImgList<T>");
	this->_addOutputSlot(out,   "out",   "image output",    "CImgList<T>");

	this->_addParameter(x, "x", "resize in x direction", true);
	this->_addParameter(y, "y", "resize in y direction", true);
	this->_addParameter(z, "z", "resize in z direction", false);
	this->_addParameter(t, "t", "resize in t direction", false);

	this->_addParameter(
			method, "method",
			"resize method <br><br>"
			"look at CImg::resize() documentation for provided methods", 1);
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

	float
		sig_x = size_x > 0 ? (float)in()[0].width()/(float)size_x/3.f : 0.f,
		sig_y = size_y > 0 ? (float)in()[0].height()/(float)size_y/3.f : 0.f,
		sig_z = size_y > 0 ? (float)in()[0].depth()/(float)size_z/3.f : 0.f;

	out() = in();
	const int m = method();
	cimglist_for(out(),k) {
		if(sig_x >= 1.f) out()[k].deriche(sig_x, 0, 'x');
		if(sig_y >= 1.f) out()[k].deriche(sig_y, 0, 'y');
		if(sig_z >= 1.f) out()[k].deriche(sig_z, 0, 'z');
		out()[k].resize(size_x, size_y, size_z, size_t, m);
	}
}
#endif // _CROP_HXX_
