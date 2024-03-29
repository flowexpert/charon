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
#include <charon-utils/CImg.h>

/// Can write a CImg image to a file.
/** See \ref cimg_files_io "Files IO in CImg" for supported formats.
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup data-io
 */
template <typename T>
class filewriter_DECLDIR FileWriter : public TemplatedParameteredObject<T> {
public:
	/// filename to write image to
	Parameter<std::string> filename;

	///should the plugin continue or throw an exception when an error occurs
	Parameter<bool> exitOnError ;

	/// image data as input slot
	InputSlot<cimg_library::CImgList<T> > in;

	/// optional frame number to append to the filename
	/** This positive number is appended just before the file extension
	 *  (i.e. before last dot in filename) and formatted with a fixed
	 *  width of six digits.
	 */
	InputSlot<unsigned int> frameNumber;

	/// list of optional names
 	/** If the input is a CImgList with multiple images (list dimension),
 	 *  names may be used to store them into different file names.
 	 *  The number of names has to match the number of list elements.
 	 *  If names and frameNumber are given, fist the frame number and then
 	 *  the given name string are appended just before the file extension
 	 *  (see frameNumber).
 	 */
	InputSlot< std::vector<std::string> > names;

	/// create a new sample object
	/// @param name             Object name
	FileWriter(const std::string& name = "");

protected:
	/// Update object.
	virtual void execute();
};

#endif /* FILEWRITER_H_ */
