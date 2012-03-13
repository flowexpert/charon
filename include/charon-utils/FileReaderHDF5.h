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
/** @file FileReaderHDF5.h
 *  Declaration of the parameter class FileReaderHDF5.
 *  \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried</a>
 *  \date 05.01.2011
 */

#ifndef _FILEREADER_HDF5_H_
#define _FILEREADER_HDF5_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef filereaderhdf5_EXPORTS
///Visual C++ specific code
#define filereaderhdf5_DECLDIR __declspec(dllexport)
#else
#define filereaderhdf5_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define filereaderhdf5_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <vigra/multi_array.hxx>
#include <charon-utils/Roi.h>

/// Read 5D vigra::MultiArray from a HDF5 file
/** Read data from a HDF5 file into a 5D vigra multiarray.
 *  Data with more than 5 dimensions is not supported.
 *  Loading a Region-of-interest(ROI) is only supported
 *  for Datasets which are 5D.
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup data-io
 */
template <typename T>
class filereaderhdf5_DECLDIR FileReaderHDF5 :
		public TemplatedParameteredObject<T> {
public:
	/// filename to read from
	Parameter<std::string> filename;

	/// path to the data within the hdf5 file
	Parameter<std::string> pathInFile;

	/// roi input to select region to read (optional)
	InputSlot < Roi<int>* > roi;

	/// The vigra::MultiArray object to be read from a hdf5 file.
	OutputSlot < vigra::MultiArray<5, T> > out;

	/// default constructor
	FileReaderHDF5(const std::string& name = "" /** [in] Instance name */);

protected:
	/// Update object.
	virtual void execute();

	/// read routine called in execute()
	/** This is provided separatly to be used e.g. from outside.
	 *  Data with more than 5 dimensions is not supported.
	 *  Loading a Region-of-interest(ROI) is only supported
	 *  for Datasets which are 5D.
	 *  \param dst           destination multiarray
	 *  \param filename      name of hdf5 file
	 *  \param dsetName      name of dataset in the file
	 *  \param roi           region-of-interest for block-read (5D only!)
	 */
	static void readHdf5(
			vigra::MultiArray<5,T> dst,
			const std::string& filename,
			const std::string& dsetName,
			const Roi<int>* roi = 0);
};

#endif /* _FILEREADER_HDF5_H_ */
