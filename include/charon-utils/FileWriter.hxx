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
/// @file FileWriter.hxx
/// Implementation of the parameter class FileWriter.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
/// @date 11.04.2009

#ifndef _FILEWRITER_HXX_
#define _FILEWRITER_HXX_

#include "FileWriter.h"
#include <string>

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
			"</ul>"
			), names(true,false) {
	ParameteredObject::_setTags("charon-utils;CImg;DiskIO") ;

	this->_addParameter(
			filename, "filename", "filename to write image to", "filewrite");
	this->_addParameter(exitOnError, "exitOnError",
		"Should the plugin raise an exception and stop the workflow "
		"if an write error occurs?", true);
	this->_addInputSlot(in, "in", "image input", "CImgList<T>");
	this->_addInputSlot(names, "names", "image input names", "vector<string>");
}

template<typename T>
void FileWriter<T>::execute() {
	try {

		if(!this->names.connected())
		{
			in().save(filename().c_str());
		}
		else if(this->names().size() != this->in().size() )
		{
			sout << "Number of given names != number of images" << std::endl;
			in().save(filename().c_str());
		}
		else
		{
			std::string base = filename();
			std::string ext =  filename().substr( filename().find_last_of('.'),  filename().length());
			for(size_t i = 0; i < this->in().size(); i++)
			{
				in()[i].save( (filename()+this->names()[i]+ext).c_str() );
			}
		}

	}
	catch (const cimg_library::CImgException& err) {
		std::string error = "\tCould not write file \""
			+ filename() + "\":\n\t" + err.what();
		if (exitOnError()) {
			throw std::runtime_error(error);
		}
		else {
			sout << error << std::endl;
		}
	}
}

#endif /* _FILEWRITER_HXX_ */
