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
    assert(roi()->left  < roi()->right);
    assert(roi()->top   < roi()->bottom);
    assert(roi()->front < roi()->back);
    out = in().get_crop(0, in().size-1, roi()->left, roi()->top, roi()->front,
        roi()->right-1, roi()->bottom-1, roi()->back-1);
}
#endif // _CROP_HXX_
