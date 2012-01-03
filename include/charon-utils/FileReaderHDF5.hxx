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
					"Read data from a HDF5 file into a 5D vigra multiarray"
					"<br>Data with more than 5 dimensions is not supported."
					"<br>Loading a Region-of-interest(ROI) is only supported "
					" for Datasets which are 5D"
					"<br> ROIs for Datasets with lower dimensionality will be "
					"ignored and loading will be slower."
					),
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
			"Roi<int>*");
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
	const vigra::ArrayVector<hsize_t>& shape = file.getDatasetShape(dSet);
	
	if(roi.connected() && shape.size() == 5) {
		sout << "\tData set has shape " << shape[0] << "x" << shape[1] << "x"
				<< shape[2] << "x" << shape[3] << "x" << shape[4] << std::endl;

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
		vigra::MultiArrayShape<5>::type tShape;
		for(unsigned int ii = 0 ;ii<5; ii++)
			tShape[ii]= 1;
		sout << "\tData set has shape " ;
		for(unsigned int ii = 0 ; ii < shape.size() ; ii++)
		{	
			sout << shape[ii] << " x " ;
			tShape[ii] = shape[ii] ;
		}
		sout << std::endl ;
		o.reshape(tShape) ;
		//create temporary MultiArray for each dimensionality and
		//copy the data to the 5D target array
		// *I know this is dump, but there is no obvious other way*
		if(shape.size() == 1)
		{
			vigra::MultiArray<1,T> tmp ;
			vigra::MultiArrayShape<1>::type rShape;
			for(unsigned int ii= 0 ; ii < 1 ; ii++)
				rShape[ii] = shape[ii] ;
			tmp.reshape(rShape) ;
			file.read(dSet,tmp) ;

			#pragma omp parallel for
			for(int xx = 0 ; xx < rShape[0] ; xx++)
				o(xx,0,0,0,0) = tmp(xx) ;
			
		}
		else if(shape.size() == 2)
		{
			vigra::MultiArray<2,T> tmp ;
			vigra::MultiArrayShape<2>::type rShape;
			for(unsigned int ii= 0 ; ii < 2 ; ii++)
				rShape[ii] = shape[ii] ;
			tmp.reshape(rShape) ;
			file.read(dSet,tmp) ;

			#pragma omp parallel for 
			for(int xx = 0 ; xx < rShape[0] ; xx++)
				for(int yy = 0 ; yy < rShape[1] ; yy++)
						o(xx,yy,0,0,0) = tmp(xx,yy) ;
		}
		else if(shape.size() == 3)
		{
			vigra::MultiArray<3,T> tmp ;
			vigra::MultiArrayShape<3>::type rShape;
			for(unsigned int ii= 0 ; ii < 3 ; ii++)
				rShape[ii] = shape[ii] ;
			tmp.reshape(rShape) ;
			file.read(dSet,tmp) ;

			#pragma omp parallel for 
			for(int xx = 0 ; xx < rShape[0] ; xx++)
				for(int yy = 0 ; yy < rShape[1] ; yy++)
					for(int zz = 0 ; zz < rShape[2] ; zz++)
						o(xx,yy,zz,0,0) = tmp(xx,yy,zz) ;
		}
		else if(shape.size() == 4)
		{
			vigra::MultiArray<4,T> tmp ;
			vigra::MultiArrayShape<4>::type rShape;
			for(unsigned int ii= 0 ; ii < 4 ; ii++)
				rShape[ii] = shape[ii] ;
			tmp.reshape(rShape) ;
			file.read(dSet,tmp) ;

			#pragma omp parallel for 
			for(int xx = 0 ; xx < rShape[0] ; xx++)
				for(int yy = 0 ; yy < rShape[1] ; yy++)
					for(int zz = 0 ; zz < rShape[2] ; zz++)
						for(int vv = 0 ; vv < rShape[2] ; vv++)
							o(xx,yy,zz,vv,0) = tmp(xx,yy,zz,vv) ;
		}
		else if(shape.size() == 5)
		{
			file.read(dSet,o) ;
		}

	}
}

#endif /* _FILEWRITERHDF5_HXX_ */
