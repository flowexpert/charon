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
/// @file ImageBlur.h
/// Declaration of the parameter class ImageBlur.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
/// @date 11.04.2009
///
/// \b Changelog:
/// -	<a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a> 2009-09-17:\n
///			use CImgList rather than CImg

#ifndef _IMAGEBLUR_H_
#define _IMAGEBLUR_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef imageblur_EXPORTS
///Visual C++ specific code
#define imageblur_DECLDIR __declspec(dllexport)
#else
#define imageblur_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define imageblur_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>
#include "Roi.h"

/// Simple class to blur images.
/** This class blurs the given image region.
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup image-manipulators
 */
template <typename T>
class imageblur_DECLDIR ImageBlur : public TemplatedParameteredObject<T>
{
private:
	/// Stores the blurred image
	cimg_library::CImgList<T> outimage;
public:
    /// image data input slot
    InputSlot<cimg_library::CImgList<T> >  in;
    /// image data output slot
    OutputSlot<cimg_library::CImgList<T> > out;

    /// blur strength
    Parameter<float> strength;

    /// image region of interest
    InputSlot<Roi<int>*> roi;

    /// create a new Threshold object
    /// @param name             Object name
	ImageBlur(const std::string& name = "");

protected:
	/// \implements ParameteredObject::execute
    virtual void execute();
};

#endif // _IMAGEBLUR_H_
