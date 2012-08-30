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
#include <typeinfo>
#include <vigra/hdf5impex.hxx>

template<typename T>
FileWriterHDF5<T>::FileWriterHDF5(const std::string& name) :
	TemplatedParameteredObject<T>("FileWriterHDF5", name,
			"Write 5D vigra::MultiArray into a HDF5 file") {
	ParameteredObject::_addParameter(
			filename, "filename",
			"Name of the hdf5 file to be written", "filewrite");
	ParameteredObject::_addParameter< std::string >(
			fileDataType, "fileDataType",
			"Data type to which the array data is cast before"
			"the file is written.", "ignore",
			"{char;unsigned char;short;unsigned short;int;"
			"unsigned int;float;double;ignore}");
	ParameteredObject::_addParameter<std::string>(
			pathInFile, "pathInFile",
			"Path to the dataset to be created within the hdf5 file<br>"
			"examples: /data group1/dataset1",
			"/data", "string");
	ParameteredObject::_addParameter(
			comment, "comment",
			"String that is written into the comment attribute of the "
			"dataset to be stored (no comment attribute is set if empty)");
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
void charon_core_LOCAL writeHdf5NoSingletons(
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

/// cast data to T2 before writing
template<typename T, int N, typename T2>
void charon_core_LOCAL writeHdf5NoSingletonsC(
		const std::vector<vigra::MultiArrayIndex>& tShape,
		vigra::HDF5File& file,
		const std::string& pathInFile, T* data) {
	size_t size = 1;
	for (int i=0; i<N; i++) {
		size *= tShape[i];
	}

	sout << "(DD) Casting (without rounding!) type to fileDataType, "
		<< "EXTRA memory(" << sizeof(T2)*size << " bytes) allocated!"
		<< std::endl;
	T2* data2 = new T2[size];
	if (data2 == 0) {
		throw std::runtime_error(
				"HDF5Writer error: could not allocate memory.");
	}
	for (size_t i =0; i < size; ++i) {
		data2[i] =(T2) data[i];
	}
	writeHdf5NoSingletons<T2,N>(tShape,file,pathInFile,data2);
	delete[] data2;
}

/// select data type to cast to
template<typename T, int N>
void charon_core_LOCAL writeHdf5NoSingletonsT(
		const std::vector<vigra::MultiArrayIndex>& tShape,
		vigra::HDF5File& file,
		const std::string& pathInFile, T* data, std::string type) {
	if(type == "char")
		writeHdf5NoSingletonsC<T,N,char>(tShape,file,pathInFile,data);
	else if(type == "unsigned char")
		writeHdf5NoSingletonsC<T,N,unsigned char>(tShape,file,pathInFile,data);
	else if(type == "short")
		writeHdf5NoSingletonsC<T,N,short>(tShape,file,pathInFile,data);
	else if(type == "unsigned short")
		writeHdf5NoSingletonsC<T,N,unsigned short>(tShape,file,pathInFile,data);
	else if(type == "int")
		writeHdf5NoSingletonsC<T,N,int>(tShape,file,pathInFile,data);
	else if(type == "unsigned int")
		writeHdf5NoSingletonsC<T,N,unsigned int>(tShape,file,pathInFile,data);
	else if(type == "float")
		writeHdf5NoSingletonsC<T,N,float>(tShape,file,pathInFile,data);
	else if(type == "double")
		writeHdf5NoSingletonsC<T,N,double>(tShape,file,pathInFile,data);
	else if(type == "ignore")
		writeHdf5NoSingletons<T,N>(tShape,file,pathInFile,data);
	else {
		throw std::runtime_error(
			std::string("HDF5Writer error: fileDataType ")
				+ type + " is not supported!");
	}
}

template<typename T>
void FileWriterHDF5<T>::writeToFile (
			const vigra::MultiArrayView<5,T>& data,
			const std::string& fName,
			const std::string& sName,
			const bool& noSingletonDimensions,
			const std::string& comment,
			const std::string& dType)
{
	vigra::HDF5File file(fName, vigra::HDF5File::Open);
	vigra::MultiArrayShape<5>::type shape = data.shape();
	T* dPtr = data.data();

	if(noSingletonDimensions) {
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
			writeHdf5NoSingletonsT<T,5>(tShape,file,sName,dPtr,dType);
			break;
		case 4:
			writeHdf5NoSingletonsT<T,4>(tShape,file,sName,dPtr,dType);
			break;
		case 3:
			writeHdf5NoSingletonsT<T,3>(tShape,file,sName,dPtr,dType);
			break;
		case 2:
			writeHdf5NoSingletonsT<T,2>(tShape,file,sName,dPtr,dType);
			break;
		case 1:
			writeHdf5NoSingletonsT<T,1>(tShape,file,sName,dPtr,dType);
			break;
		default:
			sout << "(WW) output with zero dims not possible." << std::endl;
			break;
		}
	}
	else {
		std::vector<vigra::MultiArrayIndex> tShape;
		for(vigra::MultiArrayIndex ii=0; ii<5; ii++) {
			tShape.push_back(shape[ii]);
		}
		writeHdf5NoSingletonsT<T,5>(tShape,file,sName,dPtr,dType);
	}
	if (!comment.empty()) {
#if (VIGRA_VERSION_MAJOR == 1) && (VIGRA_VERSION_MINOR < 8)
		// set attribute method renamed from vigra 1.7.1 -> 1.8.0
		file.setAttribute(sName,"comment",comment);
#else
		file.writeAttribute(sName,"comment",comment);
#endif // vigra < 1.8.0
	}
}

template<typename T>
void FileWriterHDF5<T>::execute() {
	vigra::MultiArrayShape<5>::type shape = in().shape();
	sout << "\tInput data has shape " << shape[0] << "x" << shape[1] << "x"
			<< shape[2] << "x" << shape[3] << "x" << shape[4] << std::endl;

	writeToFile(
				in(),filename(),pathInFile(),
				noSingletonDimensions(),comment(),
				fileDataType());
}

#endif /* _FILEWRITERHDF5_HXX_ */
