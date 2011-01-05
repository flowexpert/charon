/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file FileReaderHDF5.hxx
 *  Implementation of the parameter class FileReaderHDF5.
 *  \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried</a>
 *  \date 05.01.2011
 */

#ifndef _FILEWRITERHDF5_HXX_
#define _FILEWRITERHDF5_HXX_

#include "FileReaderHDF5.h"
#include <vigra/hdf5impex.hxx>

template<typename T>
FileReaderHDF5<T>::FileReaderHDF5(const std::string& name) :
	TemplatedParameteredObject<T>("FileReaderHDF5", name,
			"Read 5D vigra::MultiArray from a HDF5 file") {
	ParameteredObject::_addParameter(
			filename, "filename",
			"name of the hdf5 file to be read", "fileopen");
	ParameteredObject::_addParameter<std::string>(
			pathInFile, "pathInFile",
			"path to the dataset within the hdf5 file",
			"/data", "string");
	ParameteredObject::_addOutputSlot(
			out, "out", "data output", "vigraArray5<T>");
}

template<typename T>
void FileReaderHDF5<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	vigra::MultiArray<5,T>& o = out();

	vigra::HDF5ImportInfo info(filename().c_str(),pathInFile().c_str());
	vigra_precondition(info.numDimensions() == 5, "Dataset has to be 5D.");
	const vigra::ArrayVector<hsize_t>& is = info.shape();
	vigra_precondition(is.size() == 5, "infoShape has != 5 elements");
	vigra::MultiArrayShape<5>::type shape(is[0],is[1],is[2],is[3],is[4]);
	o.reshape(shape);
	vigra::readHDF5(info,o);

	sout << "\tRead data has shape " << is[0] << "x" << is[1] << "x"
			<< is[2] << "x" << is[3] << "x" << is[4] << std::endl;
}

#endif /* _FILEWRITERHDF5_HXX_ */
