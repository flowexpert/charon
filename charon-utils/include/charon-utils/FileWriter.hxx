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
/// @file FileWriter.hxx
/// Implementation of the parameter class FileWriter.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
/// @date 11.04.2009

#ifndef _FILEWRITER_HXX_
#define _FILEWRITER_HXX_

#include "FileWriter.h"
#include <string>
#include <iomanip>

template<typename T>
FileWriter<T>::FileWriter(const std::string& name) :
	TemplatedParameteredObject<T>("filewriter", name,
			"write image file from image using cimg.<br>"
			"Native supported file types:<ul>"
			"<li>BMP</li>"
			"<li>HDR (Radiance)</li>"
			"<li>TXT/ASC (comma separated, one line per scanline)</li>"
			"<li>PPM/PGM</li>"
			"<li>CIMG</li>"
			"</ul><br>To support the following formats, additional libaries need to be linked:<ul>"
			"<li>PNG (libpng)</li>"
			"<li>TIFF (libtiff)</li>"
			"<li>JPEG/JPG(libjpeg)</li>"
			"</ul>"),
			// optional slots
			frameNumber(true,false),
			names(true,false)
{
	ParameteredObject::_setTags("charon-utils;CImg;DiskIO") ;

	ParameteredObject::_addParameter(
		filename, "filename", "filename to write image to", "filewrite");
	ParameteredObject::_addParameter(
		exitOnError, "exitOnError",
		"Should the plugin raise an exception and stop the workflow "
		"if an write error occurs?", true);

	ParameteredObject::_addInputSlot(
		in, "in", "image input", "CImgList<T>");
	ParameteredObject::_addInputSlot(
		frameNumber, "frameNumber",
		"optional frame number to append to the filename<br><br>"
		"This positive number is appended just before the file extension "
		"(i.e. before last dot in filename) and formatted with a fixed "
		"width of six digits.");
	ParameteredObject::_addInputSlot(
		names, "names",
		"image input names<br><br>"
		"If the input is a CImgList with multiple images (list dimension), "
		"names may be used to store them into different file names.<br>"
 	 	"The number of names has to match the number of list elements.<br>"
		"If names and frameNumber are given, fist the frame number and then "
		"the given name string are appended just before the file extension "
		"(see frameNumber).",
		"vector<string>");
}

template<typename T>
void FileWriter<T>::execute() {
	std::string fName = filename();
	// file extension starts with last occuring dot
	size_t extPos = fName.find_last_of('.');
	if (extPos == std::string::npos) {
		sout << "(WW) file no extension detected" << std::endl;
	}
	std::string fNameBase = fName.substr(0,extPos);
	std::string fNameExt  = fName.substr(extPos,std::string::npos);
	if (frameNumber.connected()) {
		// append given frame number to the file name base
		std::ostringstream fBaseStr;
		fBaseStr << fNameBase
				<< std::setw(6) << std::setfill('0') << frameNumber();
		fNameBase = fBaseStr.str();
		// use the new base to construct the file name
		fName = fNameBase + fNameExt;
	}
	try {
		if(!names.connected()) {
			in().save(fName.c_str());
		}
		else if(names().size() != in().size() ) {
			sout << "Number of given names != number of images" << std::endl;
			in().save(fName.c_str());
		}
		else {
			for(size_t i = 0; i < in().size(); i++) {
				in()[i].save((fNameBase+names()[i]+fNameExt).c_str() );
			}
		}
	}
	catch (const cimg_library::CImgException& err) {
		if (exitOnError()) {
			throw;
		}
		else {
			sout << "\tCould not write file \""
				<< filename() + "\":\n\t"
				<< err.what()
				<< std::endl;
		}
	}
}

#endif /* _FILEWRITER_HXX_ */
