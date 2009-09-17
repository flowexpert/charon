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
/// @file ImageDisplay.hxx
/// Implementation of the parameter class ImageDisplay.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
/// @date 11.04.2009

#ifndef _IMAGEDISPLAY_HXX_
#define _IMAGEDISPLAY_HXX_

#include <stdexcept>
#include "ImageDisplay.h"

template <typename T>
ImageDisplay<T>::ImageDisplay(const std::string& name) :
        TemplatedParameteredObject<T>("imagedisplay", name,
            "read image from image file using cimg"),
        title("Display")
{
    this->_addParameter(frame,  "frame",  "select frame to display", 0u);
    this->_addParameter(wait,   "wait",   "display wait time (in milliseconds)", 0u);
    this->_addParameter(width,  "width",  "display width (0=auto)", 0u);
    this->_addParameter(height, "height", "display height (0=auto)", 0u);
    this->_addParameter(title,  "title",  "display title");
    this->_addInputSlot(in,     "in",     "image input", "CImgList<T>");
}

template <typename T>
void ImageDisplay<T>::execute() {
    ParameteredObject::execute();

    // set desired image size
    if(frame() >= in().size)
        throw std::out_of_range("ImageDisplay: selected frame number is "
                "larger than number of images");
    if(!width() || !height())
        _display.assign(in()[frame()].dimx(), in()[frame()].dimy(), title().c_str(), 1);
    else
        _display.assign(width, height, title().c_str(), 1);

    // show image
    _display << in()[frame()];
    _display.show();

    // and wait, if necessary
    if(wait != 0) {
        _display.wait(wait);
    }
    else
        _display.wait();
}

#endif /* _IMAGEDISPLAY_HXX_ */
