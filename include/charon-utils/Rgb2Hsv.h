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
/// \file Rgb2Hsv.h
/// Declaration of the parameter class Rgb2Hsv.
/// \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
///     Jens-Malte Gottfried</a>
/// \date 02.11.2010

#ifndef RGB2HSV_H
#define RGB2HSV_H

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef rgb2hsv_EXPORTS
///Visual C++ specific code
#define rgb2hsv_DECLDIR __declspec(dllexport)
#else
#define rgb2hsv_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define rgb2hsv_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// simple class to convert rgb images to hsv format
///
/// This class uses the cimg RGBtoHSV command to convert color images
/// in RGB format to the HSV representation. Images have to be in the
/// standart CImg color representation, i.e. dimensions are ordered
/// x,y,z,c (with c=3 color channels).
///
/// If multiple image list items are present, the conversion is
/// performed on each item seperately.
/// \see cimg_library::CImg::RGBtoHSV()
template <typename T>
class rgb2hsv_DECLDIR Rgb2Hsv : public TemplatedParameteredObject<T>
{
public:
    /// image data input slot
    InputSlot<cimg_library::CImgList<T> >  in;
    /// image data output slot
    OutputSlot<cimg_library::CImgList<T> > out;

	/// default constructor
	Rgb2Hsv(const std::string& name = "" /** [in] instance name*/);

	/// \implements ParameteredObject::execute
    virtual void execute ();
};

#endif // RGB2HSV_H
