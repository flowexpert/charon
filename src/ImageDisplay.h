/*
	Copyright (C) 2009 Jens-Malte Gottfried

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
///
/// \b Changelog:
/// -	<a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a> 2009-09-17:\n
///			use CImgList rather than CImg

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

#include <charon-core/ParameteredObject.hxx>
#include <CImg.h>

/// Simple class to display images.
/** This class displays the image read from the input slot
 *  to the user using cimg display commands.
 *  it does not perform any image conversion, but you can select slices
 *  (3rd CImg dimension, dimz), channels (4th CImg dimension,
 *  dimv) and frames (element number in the image list).
 *
 *  To display color images, it could be necessary to reorder dimensions
 *  (e.g. using the ChannelConverter plugin), if the color channels are
 *  arranged in the 5th dimension.
 */
template <typename T>
class imagedisplay_DECLDIR ImageDisplay : public TemplatedParameteredObject<T> {
public:
	/// image data as input slot
	InputSlot<cimg_library::CImgList<T> > in;

	/// select slice to display
	Parameter<unsigned int> slice;
	/// select channel to display
	/** Set this to -1 to display all channels (e.g. color images)*/
	Parameter<int> channel;
	/// select frame to display
	Parameter<unsigned int> frame;
	/// Display time (in milliseconds).
	/** Set this to zero to wait for click.*/
	Parameter<unsigned int> wait;
	/// Display width.
	/** Set this to zero for auto size.*/
	Parameter<unsigned int> width;
	/// Display height.
	/** Set this to zero for auto size.*/
	Parameter<unsigned int> height;
	/// Display title.
	Parameter<std::string> title;

	/// create a new sample object
	/** @param name             Object name*/
	ImageDisplay(const std::string& name);

	/// \implements ParameteredObject::execute
	virtual void execute();
};

#endif // _IMAGE_DISPLAY_H_
