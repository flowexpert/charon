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
/** @file FileWriterHDF5.h
 *  Declaration of the parameter class FileWriterHDF5.
 *  \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried</a>
 *  \date 08.11.2010
 */

#ifndef _FILEWRITER_HDF5_H_
#define _FILEWRITER_HDF5_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef filewriterhdf5_EXPORTS
///Visual C++ specific code
#define filewriterhdf5_DECLDIR __declspec(dllexport)
#else
#define filewriterhdf5_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define filewriterhdf5_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <vigra/multi_array.hxx>

/// Write 5D vigra::MultiArray into a HDF5 file
/**
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup data-io
 */
template <typename T>
class filewriterhdf5_DECLDIR FileWriterHDF5 :
		public TemplatedParameteredObject<T> {
public:
	/// filename to write image to
	Parameter<std::string> filename;

	/// path to the image data within the hdf5 file
	Parameter<std::string> pathInFile;

	/// string that is written into the comment attribute of the
	/// dataset to be stored (no comment attribute is set if empty)
	Parameter<std::string> comment;

	/// Do not write singleton dimensions
	/** This will reduce the dimensions of the output array
	 *  skipping singleton dimensions (i.e. such dimensions with size 1).
	 *  \warning This will (currently) make the result array unreadable by
	 *      FileReaderHDF5 (which expects 5D), but may be needed for
	 *      postprocessing e.g. with vigranumpy.
	 */
	Parameter<bool> noSingletonDimensions;

	/// The vigra::MultiArray object to be written to a hdf5 file.
	InputSlot < vigra::MultiArray<5, T> > in;

	/// default constructor
	FileWriterHDF5(const std::string& name = "" /** [in] Instance name */);

	/// Update object.
	virtual void execute();

	/// write routine called in execute()
	/** This is provided separatly to be used e.g. from outside.
	 *  The new dataset is appended or replaced (if existing).
	 *  Note that replacing an existing dataset does not free the allocated
	 *  memory. Use h5repack to save disk space (also supports compression).
	 *  \param data                   data to write
	 *  \param filename               name of hdf5 file
	 *  \param dsetName               name of dataset in the file
	 *  \param noSingletonDimensions  skip empty dimension
	 *  \param comment                dataset comment attribute string
	 */
	static void writeToFile(
		const vigra::MultiArrayView<5,T>& data,
		const std::string& filename,
		const std::string& dsetName,
		const bool& noSingletonDimensions = false,
		const std::string& comment = "");
};

#endif /* _FILEWRITER_HDF5_H_ */
