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
/// @file FileWriter.hxx
/// Implementation of the parameter class FileWriter.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 13.07.2009

#include "FileWriter.h"

template <typename T>
FileWriter<T>::FileWriter(const std::string& name) :
        ParameteredObject("filewriter", name,
            "save image to file using cimg")
{
    _addParameter (filename, "filename",
        "filename to save image in", "filename");
    _addInputSlot(image, "image",
        "image input", "CImgList");
}

template <typename T>
void FileWriter<T>::update() {
    ParameteredObject::update();
    if (image().size == 1) //2D
	    image()[0].save(filename().c_str());
    else
	    image().save(filename().c_str());
}

template <typename T>
ParameteredObject* FileWriter<T>::_newInstance(const std::string& name) const {
    return new FileWriter(name);
}
