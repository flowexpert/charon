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
/// @file Crop.h
/// Declaration of the parameter class Crop.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
/// @date 11.04.2009

#ifndef _CROP_H_
#define _CROP_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef crop_EXPORTS
///Visual C++ specific code
#define crop_DECLDIR __declspec(dllexport)
#else
#define crop_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define crop_DECLDIR
#endif

#include "ParameteredObject.hxx"
#include "CImg.h"
#include "Roi.h"

/// Simple class to crop images.
/// This class shrinks the image to given region.
template <typename T>
class crop_DECLDIR Crop : public TemplatedParameteredObject<T>
{
public:
    /// image data input slot
    InputSlot<cimg_library::CImg<T> >  inimage;
    /// image data output slot
    OutputSlot<cimg_library::CImg<T> > outimage;

    /// image region of interest
    InputSlot<Roi<int>*> roi;

    /// create a new Threshold object
    /// @param name             Object name
    Crop(const std::string& name);

	/// \implements ParameteredObject::execute
    virtual void execute();
};

#endif // _CROP_H_
