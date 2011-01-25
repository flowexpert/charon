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
					"Read 5D vigra::MultiArray from a HDF5 file"),
			roi(true,false) // optional
{
	ParameteredObject::_addParameter(
			filename, "filename",
			"name of the hdf5 file to be read", "fileopen");
	ParameteredObject::_addParameter<std::string>(
			pathInFile, "pathInFile",
			"path to the dataset within the hdf5 file",
			"/data", "string");
	ParameteredObject::_addInputSlot(
			roi, "roi", "roi input to select region to read",
			"Roi<int>");
	ParameteredObject::_addOutputSlot(
			out, "out", "data output", "vigraArray5<T>");
}

template<typename T>
void FileReaderHDF5<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	vigra::MultiArray<5,T>& o = out();

	vigra::HDF5File file(filename().c_str(), vigra::HDF5File::Open);
	std::string dSet = pathInFile().c_str();
	vigra_precondition(
			file.getDatasetDimensions(dSet) == 5, "Dataset has to be 5D.");
	const vigra::ArrayVector<hsize_t>& shape = file.getDatasetShape(dSet);
	vigra_postcondition(shape.size() == 5, "infoShape has != 5 elements");
	if(roi.connected()) {
		// partial read of dataset
		const Roi<int>& r = *roi();
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
		o.reshape(blockShape);
		file.readBlock(dSet,blockOffset,blockShape,o);
	}
	else {
		// read whole dataset
		vigra::MultiArrayShape<5>::type tShape;
		for(unsigned int ii=0; ii<5; ii++)
			tShape[ii]=shape[ii];
		o.reshape(tShape);
		file.read(dSet,o);

		sout << "\tRead data has shape " << shape[0] << "x" << shape[1] << "x"
				<< shape[2] << "x" << shape[3] << "x" << shape[4] << std::endl;
	}
}

#endif /* _FILEWRITERHDF5_HXX_ */
