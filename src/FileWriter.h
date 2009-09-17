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
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
/// @date 29.07.2009
///
/// \b Changelog:
/// -	<a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a> 2009-09-17:\n
///			use CImgList rather than CImg

#ifndef FILEWRITER_H_
#define FILEWRITER_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef filewriter_EXPORTS
///Visual C++ specific code
#define filewriter_DECLDIR __declspec(dllexport)
#else
#define filewriter_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define filewriter_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <CImg.h>

/// Can write a CImg image to a file.
template <typename T>
class filewriter_DECLDIR FileWriter : public TemplatedParameteredObject<T> {
public:
    /// filename to write image to
    Parameter<std::string> filename;

    /// image data as input slot
    InputSlot<cimg_library::CImgList<T> > in;

    /// create a new sample object
    /// @param name             Object name
    FileWriter(const std::string& name);

    /// Update object.
    virtual void execute();
};

#endif /* FILEWRITER_H_ */
