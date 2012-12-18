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
/** \file FileReader.h
 *  Declaration of the parameter class FileReader.
 *  \author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">
 *      Cornelius Ratsch</a>
 *  \date 20.08.2009
 *
 *  \b Changelog:
 *  - <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a> 2009-09-17:\n
 *    use CImgList rather than CImg
 */

#ifndef _FILEREADER_H_
#define _FILEREADER_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef filereader_EXPORTS
///Visual C++ specific code
#define filereader_DECLDIR __declspec(dllexport)
#else
#define filereader_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define filereader_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/FileReaderWatcher.h>
#include <charon-utils/CImg.h>

/// Simple class to read image files.
/** This class is able to read an image from some image file
 *  and provide it as output slot.
 *  See \ref cimg_files_io "Files IO in CImg" for supported formats.
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup data-io
 */
template <typename T>
class filereader_DECLDIR FileReader : public TemplatedParameteredObject<T> {
public:
	/// filename/pattern to read image from
	Parameter<std::string> filename;

	/// list of image files
	/** If this is not empty, the list will be added to the CImgList
	 *  according to the paramter value of frameDimension.
	 *  Mixes with filename.
	 */
	ParameterList<std::string> fileList;

	/// if watchable is set, files are monitored for changes
	Parameter< bool > watchable;

	/// store frames in z, v or list dimension?
	/** Depending on application conventions the list of frames
	 *  will be stored in the CImgList-entries or one of the last
	 *  two dimensions of a single CImg.
	 */
	Parameter<std::string> frameDimension;

	/// is filename a pattern?
	/** Currently only a simple pattern with one integer is allowed,
	 *  such as "image-%6.pgm".
	 *  Pattern Syntax is QRegExp("(.*)%([0-9]+)(.*)").
	 *  Prefix and tail are kept, the number after the percent sign
	 *  is the field width, fillup character is zero.
	 */
	Parameter<bool> fileNameIsPattern;

	/// pattern parameter start
	Parameter<unsigned int> start;

	/// pattern parameter step
	Parameter<unsigned int> step;

	/// pattern parameter end (i < end)
	Parameter<unsigned int> end;

	/// use only the red channel of png files
	/** Due to an CImg error, png files are always read as 3-channel RGB image,
	 *  even if they are grayscale.
	 *  Since all channels of gray images are filled with the same value,
	 *  arbitrarily selecting the red channel yields the wanted grayscale image.
	 */
	Parameter<bool> cropPngChannels ;

	/// image data as output slot
	OutputSlot<cimg_library::CImgList<T> > out;

	/// create a new sample object
	/// \param name             Object name
	FileReader(const std::string& name = "");
	virtual ~FileReader();

protected:
	/// Update object.
	/** (re)load image(s) and put data into the output slot */
	virtual void execute();

private:
	/// append image to output slot handling dim switch
	void _append(const cimg_library::CImgList<T>& tmp);
	/// print current memory consumption of output slot
	void _mem();
	///helper function for reading the radiance hdr image file format	
	cimg_library::CImgList<T> _readHDR(const std::string& filename) ;
	/// watch for file changes
	FileReaderWatcher* _fileReaderWatcher;
};

#endif // _FILEREADER_H_
