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
/// @file FileReader.hxx
/// Implementation of the parameter class FileReader.
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
/// @date 20.08.2009

#ifndef _FILEREADER_HXX_
#define _FILEREADER_HXX_

#include "FileReader.h"

template <typename T>
FileReader<T>::FileReader(const std::string& name) :
		TemplatedParameteredObject<T>("filereader", name,
			"read image from image file using cimg")
{
	this->_addParameter (filename, "filename",
		"filename to read image from", "fileopen");
#ifdef QT_CORE_LIB
	this->_addParameter (watchable, "watchable",
		"if set, file is being monitored for changes", false);
#endif
	this->_addOutputSlot(out, "out",
		"image output", "CImgList<T>");

#ifdef QT_CORE_LIB
	fileReaderWatcher = new FileReaderWatcher( this );
#endif
}

template <typename T>
void FileReader<T>::execute() {
#ifdef QT_CORE_LIB
	fileReaderWatcher->setFilename( this->filename() );
#endif
	try
	{
		out().load(filename().c_str());
	}
	catch(const cimg_library::CImgException& err)
	{
		ParameteredObject::raise(
					std::string("Could not read file\n\t") + err.what());
	}

}

#endif /* _FILEREADER_HXX_ */

