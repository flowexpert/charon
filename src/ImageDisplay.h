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
/// @file ImageDisplay.h
/// Declaration of the parameter class ImageDisplay.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
/// @date 11.04.2009

#ifndef _IMAGE_DISPLAY_H_
#define _IMAGE_DISPLAY_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef imagedisplay_EXPORTS
///Visual C++ specific code
#define imagedisplay_DECLDIR __declspec(dllexport)
#else
#define imagedisplay_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define imagedisplay_DECLDIR
#endif

#include "ParameteredObject.hxx"
#include "CImg.h"

/// Simple class to display images.
/// This class displays the image read from the input slot
/// to the user using cimg display commands.
template <typename T>
class imagedisplay_DECLDIR ImageDisplay : public TemplatedParameteredObject<T> {
private:
    /// Display window
    cimg_library::CImgDisplay _display;

public:
    /// image data as input slot
    InputSlot<cimg_library::CImg<T> > image;

    /// Display time (in milliseconds).
    /// Set this to zero to wait for click.
    Parameter<unsigned int> wait;
    /// Display width.
    /// Set this to zero for auto size.
    Parameter<unsigned int> width;
    /// Display height.
    /// Set this to zero for auto size.
    Parameter<unsigned int> height;
    /// Display title.
    Parameter<std::string> title;

    /// create a new sample object
    /// @param name             Object name
    ImageDisplay(const std::string& name);

    virtual void execute();
};

#endif // _IMAGE_DISPLAY_H_
