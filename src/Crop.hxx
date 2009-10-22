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
}

template <typename T>
void Crop<T>::execute()
{
    ParameteredObject::execute();

    // check roi ranges
    assert(roi());
    assert(roi()->xBegin() <= roi()->xEnd());
    assert(roi()->yBegin() <= roi()->yEnd());
    assert(roi()->zBegin() <= roi()->zEnd());
    assert(roi()->tBegin() <= roi()->tEnd());
    assert(roi()->vBegin() <= roi()->vEnd());

	unsigned int
		x0 = roi()->xBegin,
		y0 = roi()->yBegin,
		z0 = roi()->zBegin,
		t0 = roi()->tBegin,
		v0 = roi()->vBegin,
		x1 = roi()->xEnd-1,
		y1 = roi()->yEnd-1,
		z1 = roi()->zEnd-1,
		t1 = roi()->tEnd-1,
		v1 = roi()->vEnd-1;
	if(roi()->xBegin() == roi()->xEnd()) {
		sout << "\tleaving x dimension uncropped" << std::endl;
		x0 = 0; x1 = in()[0].dimx()-1;
	}
	if(roi()->yBegin() == roi()->yEnd()) {
		sout << "\tleaving y dimension uncropped" << std::endl;
		y0 = 0; y1 = in()[0].dimy()-1;
	}
	if(roi()->zBegin() == roi()->zEnd()) {
		sout << "\tleaving z dimension uncropped" << std::endl;
		z0 = 0; z1 = in()[0].dimz()-1;
	}
	if(roi()->tBegin() == roi()->tEnd()) {
		sout << "\tleaving t dimension uncropped" << std::endl;
		t0 = 0; t1 = in()[0].dimv()-1;
	}
	if(roi()->vBegin() == roi()->vEnd()) {
		sout << "\tleaving v dimension uncropped" << std::endl;
		v0 = 0; v1 = in().size - 1;
	}
    out = in().get_crop(v0, v1, x0, y0, z0, t0, x1, y1, z1, t1);
}
#endif // _CROP_HXX_
