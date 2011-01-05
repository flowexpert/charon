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

	/// The vigra::MultiArray object to be written to a hdf5 file.
	InputSlot < vigra::MultiArrayView<5, T> > in;

	/// default constructor
	FileWriterHDF5(const std::string& name = "" /** [in] Instance name */);

	/// Update object.
	virtual void execute();
};

#endif /* _FILEWRITER_HDF5_H_ */
