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
/// @file Resize.h
/** Declaration of the parameter class Resize.
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date 11.04.2009
 */

#ifndef _RESIZE_H_
#define _RESIZE_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef resize_EXPORTS
///Visual C++ specific code
#define resize_DECLDIR __declspec(dllexport)
#else
#define resize_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define resize_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>
#include "Roi.h"

/** Simple class to resize images.
 *
 *  This class resizes the image to given size.
 *  If you want to resize only in certain dimensions, leave
 *  the values of the unneeded ranges at [0:0].
 *
 *  Absolute values of the given roi are of no interest.
 *  Only their differences (i.e. width, height, etc) matter.
 *
 *  Channels (i.e. list element index) are not resized.
 */
template <typename T>
class resize_DECLDIR Resize : public TemplatedParameteredObject<T>
{
public:
	/// image data input slot
	InputSlot<cimg_library::CImgList<T> >  in;
	/// image data output slot
	OutputSlot<cimg_library::CImgList<T> > out;

	/// new size (only width, height etc are considered)
	InputSlot<Roi<int>*> roi;

	/// \name select dimensions to resize
	//  \{
	Parameter<bool> x,y,z,t;
	//  \}

	/// Interpolation method
	/** Look at CImg documentation to see which interpolation methods
	 *  are provided.
	 *  \see cimg_library::CImg<T>::resize()
	 */
	Parameter<int> method;

	/// control blur strength on image size reduction
	Parameter<float> preBlurFactor;

	/// standard constructor
	/// @param name             Object name
	Resize(const std::string& name = "");

	/// \implements ParameteredObject::execute
	virtual void execute();
};

#endif // _RESIZE_H_
