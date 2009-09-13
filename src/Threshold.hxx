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
/// @file Threshold.hxx
/// Implementation of the parameter class Threshold.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
/// @date 11.04.2009

#ifndef _THRESHOLD_HXX_
#define _THRESHOLD_HXX_

#include "Threshold.h"

template <typename T>
Threshold<T>::Threshold(const std::string& name) :
        TemplatedParameteredObject<T>("threshold", name,
            "threshold images using cimg"),
        soft(false),
        strict(false)
{
    this->_addParameter (value,      "value",      "Threshold value");
    this->_addParameter (soft,       "soft",       "Enable soft thresholding");
    this->_addParameter (strict,     "strict",     "Enable strict thresholding");
    this->_addInputSlot (inimage,    "inimage",    "image input",  "CImg<T>");
    this->_addOutputSlot(outimage,   "outimage",   "image output", "CImg<T>");
}


template <typename T>
void Threshold<T>::execute()
{
    ParameteredObject::execute();

    outimage = inimage().get_threshold(value, soft, strict);
}

#endif /* _THRESHOLD_HXX_ */
