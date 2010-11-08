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
/// @file Threshold.h
/// Declaration of the parameter class Threshold.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
/// @date 11.04.2009
///
/// \b Changelog:
/// -	<a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a> 2009-09-17:\n
///			use CImgList rather than CImg

#ifndef THRESHOLD_H
#define THRESHOLD_H

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef threshold_EXPORTS
///Visual C++ specific code
#define threshold_DECLDIR __declspec(dllexport)
#else
#define threshold_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define threshold_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// simple class to threshold images
///
/// This class uses the cimg threshold command to threshold
/// images at the given value. Soft thresholding and
/// strict threshold can be enabled.
/// \see cimg_library::CImg::threshold()
template <typename T>
class threshold_DECLDIR Threshold : public TemplatedParameteredObject<T>
{
public:
    /// image data input slot
    InputSlot<cimg_library::CImgList<T> >  in;
    /// image data output slot
    OutputSlot<cimg_library::CImgList<T> > out;

    /// Threshold value.
    Parameter<T> value;
    /// Enable soft thresholding.
    Parameter<bool> soft;
    /// Tells if the threshold is strict.
    Parameter<bool> strict;

    /// create a new Threshold object
    /// @param name             Object name
	Threshold(const std::string& name = "");

	/// \implements ParameteredObject::execute
    virtual void execute ();
};

#endif // THRESHOLD_H
