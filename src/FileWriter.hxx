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

template<typename T>
FileWriter<T>::FileWriter(const std::string& name) :
	TemplatedParameteredObject<T>("filewriter", name,
			"write image file from image using cimg") {
	this->_addParameter(filename, "filename", "filename to write image to",
			"filename");
	this->_addInputSlot(image, "image", "image input", "CImg<T>");
}

template<typename T>
void FileWriter<T>::execute() {
	ParameteredObject::execute();

	image().save(filename().c_str());
}

#endif /* _FILEWRITER_HXX_ */
