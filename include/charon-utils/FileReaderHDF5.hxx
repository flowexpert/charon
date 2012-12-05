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

#ifndef _FILEREADERHDF5_HXX_
#define _FILEREADERHDF5_HXX_

#include "FileReaderHDF5.h"
#include <vigra/hdf5impex.hxx>
#include "Roi.hxx"

template<typename T, int N>
vigra::MultiArray<5,T> charon_core_LOCAL readNDimHDF5(
	vigra::HDF5File& file,
	const vigra::ArrayVector<hsize_t>& shape,
	const std::string& pathInFile)
{
	vigra::MultiArray<N,T> tmp;
	//temporary shape
	typename vigra::MultiArrayShape<N>::type tShape;
	//result shape
	typename vigra::MultiArrayShape<5>::type rShape;
	
	sout << "\tData set has shape ";
	for(unsigned int ii= 0 ; ii < N ; ii++) {
		sout << shape[ii] << " x ";
		rShape[ii] = tShape[ii] = shape[ii];
	}
	sout << std::endl;
	for(unsigned int ii= N  ; ii < 5 ; ii++) {
		rShape[ii] = 1;
	}
	tmp.reshape(tShape) ;
	file.read(pathInFile,tmp) ;
	return vigra::MultiArray<5,T>(rShape,tmp.data());
}

template<typename T>
FileReaderHDF5<T>::FileReaderHDF5(const std::string& name) :
		TemplatedParameteredObject<T>("FileReaderHDF5", name,
				"Read data from a HDF5 file into a 5D vigra multiarray"
				"<br>Data with more than 5 dimensions is not supported."
				"<br>Loading a Region-of-interest(ROI) is only supported "
				"for Datasets which are 5D"
				),
		roi(true,false) // optional
{
	ParameteredObject::_setTags("charon-utils;Vigra;DiskIO") ;

	ParameteredObject::_addParameter(
			filename, "filename",
			"name of the hdf5 file to be read", "fileopen");
	ParameteredObject::_addParameter<std::string>(
			pathInFile, "pathInFile",
			"path to the dataset within the hdf5 file",
			"/data", "string");
	ParameteredObject::_addInputSlot(
			roi, "roi", "roi input to select region to read",
			"Roi<int>*");
	ParameteredObject::_addOutputSlot(
			out, "out", "data output", "vigraArray5<T>");
}

template<typename T>
void FileReaderHDF5<T>::readHdf5(
		vigra::MultiArray<5,T>& dst,
		const std::string& filename,
		const std::string& dSet,
		const Roi<int>* roi)
{
	vigra::HDF5File file(filename, vigra::HDF5File::Open);
	const vigra::ArrayVector<hsize_t>& shape = file.getDatasetShape(dSet);
	vigra_precondition(shape.size() > 0, "data set empty!");
	sout << "\tData set has shape " << shape[0];
	for (size_t ii=1; ii < shape.size(); ii++) {
		sout << "x" << shape[ii];
	}
	sout << " (" << shape.size() << " dimensions)" << std::endl;

	if(roi) {
		vigra_precondition(shape.size() == 5,
				"Block read for data sets with dim!=5 not supported!");

		// partial read of dataset
		const Roi<int>& r = *roi;
		vigra_precondition(
				r.xBegin >= 0 && r.yBegin >= 0 && r.zBegin >= 0 &&
				r.tBegin >= 0 && r.vBegin >= 0, "offset has to be positive!");
		vigra::MultiArrayShape<5>::type blockOffset(
				r.xBegin,r.yBegin,r.zBegin,r.tBegin,r.vBegin);
		vigra::MultiArrayShape<5>::type blockShape(
				r.xEnd>r.xBegin?r.getWidth():shape[0u]-r.xBegin,
				r.yEnd>r.yBegin?r.getHeight():shape[1u]-r.yBegin,
				r.zEnd>r.zBegin?r.getDepth():shape[2u]-r.zBegin,
				r.tEnd>r.tBegin?r.getDuration():shape[3u]-r.tBegin,
				r.vEnd>r.vBegin?r.getChannels():shape[4u]-r.vBegin);
		dst.reshape(blockShape);
		file.readBlock(dSet,blockOffset,blockShape,dst);
	}
	else {
		vigra_precondition(shape.size() <= 5,
				"Dataset is not supported (dim > 5)!");
		switch(shape.size()) {
		case 1:
			dst = readNDimHDF5<T,1>(file,shape,dSet);
			break;
		case 2:
			dst = readNDimHDF5<T,2>(file,shape,dSet);
			break;
		case 3:
			dst = readNDimHDF5<T,3>(file,shape,dSet);
			break;
		case 4:
			dst = readNDimHDF5<T,4>(file,shape,dSet);
			break;
		default: // 5
			{
				typename vigra::MultiArrayShape<5>::type rShape;
				for(unsigned int ii= 0 ; ii < 5 ; ii++)
				{
					rShape[ii] = shape[ii];
				}
				dst.reshape(rShape);
				file.read(dSet,dst);
				break;
			}
		}
	}
}

template<typename T>
void FileReaderHDF5<T>::execute() {
	readHdf5(out(),filename(),pathInFile(),(roi.connected()?roi():0));
}

#endif /* _FILEREADERHDF5_HXX_ */
