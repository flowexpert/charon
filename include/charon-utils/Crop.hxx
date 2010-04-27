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
/// @file Crop.hxx
/// Implementation of the parameter class Threshold.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
/// @date 11.04.2009

#ifndef _CROP_HXX_
#define _CROP_HXX_

#include "Crop.h"

template <typename T>
Crop<T>::Crop(const std::string& name) :
		TemplatedParameteredObject<T>("crop", name, "crop images")
{
	this->_addInputSlot (roi,   "roi",   "region to crop",  "Roi<int>");
	this->_addInputSlot (in,    "in",    "image input",     "CImgList<T>");
	this->_addOutputSlot(out,   "out",   "image output",    "CImgList<T>");

	this->_addParameter(x, "x", "crop in x direction", true);
	this->_addParameter(y, "y", "crop in y direction", true);
	this->_addParameter(z, "z", "crop in z direction", true);
	this->_addParameter(t, "t", "crop in t direction", true);
	this->_addParameter(v, "v", "crop in v direction", true);
}

template <typename T>
void Crop<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	// check roi ranges
	assert(roi());
	assert(roi()->xBegin() <= roi()->xEnd());
	assert(roi()->yBegin() <= roi()->yEnd());
	assert(roi()->zBegin() <= roi()->zEnd());
	assert(roi()->tBegin() <= roi()->tEnd());
	assert(roi()->vBegin() <= roi()->vEnd());

	unsigned int
		x0 = x() ? roi()->xBegin() : 0u,
		y0 = y() ? roi()->yBegin() : 0u,
		z0 = z() ? roi()->zBegin() : 0u,
		t0 = t() ? roi()->tBegin() : 0u,
		v0 = v() ? roi()->vBegin() : 0u,
		x1 = x() ? roi()->xEnd()-1 : in()[0].width()-1u,
		y1 = y() ? roi()->yEnd()-1 : in()[0].height()-1u,
		z1 = z() ? roi()->zEnd()-1 : in()[0].depth()-1u,
		t1 = t() ? roi()->tEnd()-1 : in()[0].spectrum()-1u,
		v1 = v() ? roi()->vEnd()-1 : in().size()-1u;

	if(x() && roi()->xBegin() == roi()->xEnd()) {
		sout << "\tleaving x dimension uncropped" << std::endl;
		x0 = 0u; x1 = in()[0].width()-1u;
	}
	if(y() && roi()->yBegin() == roi()->yEnd()) {
		sout << "\tleaving y dimension uncropped" << std::endl;
		y0 = 0u; y1 = in()[0].height()-1u;
	}
	if(z() && roi()->zBegin() == roi()->zEnd()) {
		sout << "\tleaving z dimension uncropped" << std::endl;
		z0 = 0u; z1 = in()[0].depth()-1u;
	}
	if(t() && roi()->tBegin() == roi()->tEnd()) {
		sout << "\tleaving t dimension uncropped" << std::endl;
		t0 = 0u; t1 = in()[0].spectrum()-1u;
	}
	if(v() && roi()->vBegin() == roi()->vEnd()) {
		sout << "\tleaving v dimension uncropped" << std::endl;
		v0 = 0u; v1 = in().size() - 1u;
	}
	out() = in().get_images(v0, v1);
	cimglist_for(out(),k)
		out()[k].crop(x0, y0, z0, t0, x1, y1, z1, t1);
}
#endif // _CROP_HXX_
