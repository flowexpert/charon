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
			"path to the dataset to be created within the hdf5 file<br>"
			"examples: /data group1/dataset1",
			"/data", "string");
	ParameteredObject::_addParameter(
			comment, "comment",
			"string that is written into the comment attribute of the "
			"dataset to be stored (no comment attribute is set if empty)",
			"string");
	ParameteredObject::_addParameter(
			noSingletonDimensions, "noSingletonDimensions",
			"Do not write singleton dimensions<br>"
			"This will reduce the dimensions of the output array "
			"skipping singleton dimensions "
			"(i.e. such dimensions with size 1).<br>"
			"<span style=\"color:red\">Warning:</span> "
			"This will (currently) make the result array unreadable by "
			"FileReaderHDF5 (which expects 5D), "
			"but may be needed for postprocessing "
			"e.g. with vigranumpy.",
			false
	);

	ParameteredObject::_addInputSlot(
			in, "in", "data input", "vigraArray5<T>");
}

template<typename T, int N>
void writeHdf5NoSingletons(
		const std::vector<vigra::MultiArrayIndex>& tShape,
		vigra::HDF5File& file,
		const std::string& pathInFile, T* data) {
	assert(tShape.size() == N);
	typename vigra::MultiArrayShape<N>::type shape;
	for (int i=0; i<N; i++) {
		shape[i] = tShape[i];
	}
	typename vigra::MultiArray<N,T> res(shape,data);
	sout << "\tOutput data has shape ";
	for(int i=0; i<N-1; i++) {
		sout << tShape[i] << "x";
	}
	sout << tShape[N-1] << std::endl;
	file.write(pathInFile.c_str(), res);
}

template<typename T>
void FileWriterHDF5<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	vigra::MultiArrayShape<5>::type shape = in().shape();
	sout << "\tInput data has shape " << shape[0] << "x" << shape[1] << "x"
			<< shape[2] << "x" << shape[3] << "x" << shape[4] << std::endl;

	vigra::HDF5File file(filename().c_str(),vigra::HDF5File::Open);

	if(noSingletonDimensions()) {
		std::vector<vigra::MultiArrayIndex> tShape;
		for(vigra::MultiArrayIndex ii=0; ii<5; ii++) {
			if(shape[ii]>1) {
				tShape.push_back(shape[ii]);
			}
		}
		int curDim = tShape.size();
		sout << "\tTruncating to " << curDim << " dimensions." << std::endl;
		switch(curDim) {
		case 5:
			sout << "\tNo truncating needed!" << std::endl;
			file.write(pathInFile().c_str(), in());
			break;
		case 4:
			writeHdf5NoSingletons<T,4>(tShape,file,pathInFile(),in().data());
			break;
		case 3:
			writeHdf5NoSingletons<T,3>(tShape,file,pathInFile(),in().data());
			break;
		case 2:
			writeHdf5NoSingletons<T,2>(tShape,file,pathInFile(),in().data());
			break;
		case 1:
			writeHdf5NoSingletons<T,1>(tShape,file,pathInFile(),in().data());
			break;
		default:
			sout << "output with zero dims not possible." << std::endl;
			break;
		}
	}
	else {
		file.write(pathInFile().c_str(), in());
	}
	if (!comment().empty()) {
#if (VIGRA_VERSION_MAJOR == 1) && (VIGRA_VERSION_MINOR < 8)
		// set attribute method renamed from vigra 1.7.1 -> 1.8.0
		file.setAttribute(pathInFile(),"comment",comment().c_str());
#else
		file.writeAttribute(pathInFile(),"comment",comment().c_str());
#endif // vigra < 1.8.0
	}
}

#endif /* _FILEWRITERHDF5_HXX_ */
