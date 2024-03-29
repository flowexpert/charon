/*  Copyright (C) 2009 Jens-Malte Gottfried

    This file is part of Charon.

    Charon is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Charon is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/// @file Normalize.h
/// Declaration of the parameter class Normalize.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
/// @date 11.04.2009
///
/// \b Changelog:
/// -	<a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a> 2009-09-17:\n
///			use CImgList rather than CImg

#ifndef NORMALIZE_H
#define NORMALIZE_H

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef normalize_EXPORTS
///Visual C++ specific code
#define normalize_DECLDIR __declspec(dllexport)
#else
#define normalize_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define normalize_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>

/// Simple class to normalize images.
/** This class uses the cimg normalize command to normalize
 *  images linearly between two given values.
 *
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup image-manipulators
 */
template <typename T>
class normalize_DECLDIR Normalize : public TemplatedParameteredObject<T>
{
public:
    /// image data input slot
    InputSlot<cimg_library::CImgList<T> > in;
    /// image data output slot
    OutputSlot<cimg_library::CImgList<T> > out;

    /// lower bound of normalization
    Parameter<T> lower;
    /// upper bound of normalization
    Parameter<T> upper;

    /// create a new Normalize object
    /// @param name             Object name
	Normalize(const std::string& name = "");

protected:
	/// apply threshold to all given images
	/// \implements ParameteredObject::execute
    virtual void execute();
};

#endif // NORMALIZE_H
