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
///
/// \b Changelog:
/// -	<a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a> 2009-09-17:\n
///			use CImgList rather than CImg

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

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>
#include "Roi.h"

/** Simple class to crop images.
 *
 *  This class shrinks the image to given region.
 *  If you want to crop only in certain dimensions, leave
 *  the values of the unneeded ranges at [0:0].
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup image-manipulators
 */
template <typename T>
class crop_DECLDIR Crop : public TemplatedParameteredObject<T>
{
public:
	/// image data input slot
	InputSlot<cimg_library::CImgList<T> >  in;
	/// image data output slot
	OutputSlot<cimg_library::CImgList<T> > out;

	/// image region of interest
	InputSlot<Roi<int>*> roi;

	/// \name select dimensions to crop
	//  \{
	Parameter<bool> x,y,z,t,v;
	//  \}

	/// create a new Threshold object
	/// @param name             Object name
	Crop(const std::string& name = "");

	/// \implements ParameteredObject::execute
	virtual void execute();
};

#endif // _CROP_H_
