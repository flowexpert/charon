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
/// @file FileWriter.h
/// Declaration of the parameter class FileWriter.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 13.07.2009

#ifndef _FILEWRITER_H_
#define _FILEWRITER_H_

#include <ParameteredObject.hxx>
#include "../CImg.h"

/// Simple class to read image files.
/// This class is able to read an image from some image file
/// and provide it as output slot.
template <typename T>
class FileWriter : public ParameteredObject {
protected:
    virtual ParameteredObject* _newInstance(const std::string& name) const;

public:
    /// filename to read image from
    Parameter<std::string> filename;

    /// image data as output slot
    InputSlot<cimg_library::CImgList<T> > image;

    /// create a new sample object
    /// @param name             Object name
    FileWriter(const std::string& name = "");

    /// Update object.
    /// Reload image and put new data into the output slot.
    virtual void update();
};

#endif // _FILEWRITER_H_
