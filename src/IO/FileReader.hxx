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
/// @file FileReader.hxx
/// Implementation of the parameter class FileReader.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 13.07.2009

#include "FileReader.h"

template <typename T>
FileReader<T>::FileReader(const std::string& name) :
        ParameteredObject("filereader", name,
            "read image from image file using cimg")
{
    _addParameter (filename, "filename",
        "filename to read image from", "filename");
    _addOutputSlot(image, "image",
        "image output", "CImgList");
}

template <typename T>
void FileReader<T>::update() {
    ParameteredObject::update();

    image = cimg_library::CImgList<T>::get_load(filename().c_str());
    _outDataChanged(image);
}

template <typename T>
ParameteredObject* FileReader<T>::_newInstance(const std::string& name) const {
    return new FileReader(name);
}
