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
/** \file FileWriterHDF5.hxx
 *  Implementation of the parameter class FileWriterHDF5.
 *  \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried</a>
 *  \date 08.11.2010
 */

#ifndef _FILEWRITERHDF5_HXX_
#define _FILEWRITERHDF5_HXX_

#include "FileWriterHDF5.h"
#include <vigra/hdf5impex.hxx>

template<typename T>
FileWriterHDF5<T>::FileWriterHDF5(const std::string& name) :
	TemplatedParameteredObject<T>("FileWriterHDF5", name,
			"Write 5D vigra::MultiArray into a HDF5 file") {
	ParameteredObject::_addParameter(
			filename, "filename",
			"name of the hdf5 file to be written", "filewrite");
	ParameteredObject::_addParameter<std::string>(
			pathInFile, "pathInFile",
			"path to the dataset to be created within the hdf5 file",
			"/data", "string");
	ParameteredObject::_addParameter(
			comment, "comment",
			"string that is written into the comment attribute of the "
			"dataset to be stored (no comment attribute is set if empty)",
			"string");
	ParameteredObject::_addInputSlot(
			in, "in", "data input", "vigraArray5<T>");
}

template<typename T>
void FileWriterHDF5<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	const std::string& filePath = filename();
	vigra::writeHDF5(filePath.c_str(), pathInFile().c_str(), in());
	if (!comment().empty()) {
		const std::string& inPath = pathInFile();
		std::string::size_type pos = inPath.rfind("/");
		assert(pos != std::string::npos);
		std::string dName(inPath.begin()+pos+1, inPath.end());
		std::string gName(inPath.begin(), inPath.begin()+pos+1);
		// groups have trailing slashes
		assert(*(gName.rbegin()) == '/');
		// the only group that is allowed to start with a slash is the
		// root group
		assert(gName == "/" || gName[0] != '/');

		//create or open file
		hid_t file = H5Fopen(filePath.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
		assert(file >= 0);
		hid_t group = H5Gopen(file, gName.c_str(), H5P_DEFAULT);
		assert(group >= 0);

		H5LTset_attribute_string(
				group, dName.c_str(), "comment", comment().c_str());
		H5Gclose(group);
		H5Fclose(file);
	}
}

#endif /* _FILEWRITERHDF5_HXX_ */
