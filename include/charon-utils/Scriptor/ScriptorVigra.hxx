/*  Copyright (C) 2012 mox

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
/** \file Scriptor_vigra.hxx
 *  Implementation of the parameter class Scriptor_vigra.
 *  \author <a href="mailto:de_mox@zoho.com">
 *      mox</a>
 *  \date 29.02.2012
 */

#ifndef _SCRIPTOR_VIGRA_HXX_
#define _SCRIPTOR_VIGRA_HXX_

#include <vigra/hdf5impex.hxx>
#include "ScriptorVigra.h"

#include <charon-utils/FileWriterHDF5.h>
#include <charon-utils/FileReaderHDF5.h>

template <typename T>
ScriptorVigra<T>::ScriptorVigra(const std::string& name,
								  const std::string& classname,
								  const std::string& doc):
	ScriptorBase<T>(name, classname, doc),
	vigraIn(true, false)
{
	ParameteredObject::_addInputSlot(
		vigraIn, "vigraIn",
		"input image, will be saved as HDF5 file",
		"vigraArray5<T>");

	ParameteredObject::_addOutputSlot(
		vigraOut, "vigraOut",
		"output image",
		"vigraArray5<T>");
}

template <typename T>
void ScriptorVigra<T>::prepare()
{
	// setup everything we need to run the script
	_setupVariables();

	// check if the filename is set at all
	if(_tempImageFilenameVigra.empty())
	{
		sout<<"(WW) No filename for temporary image file set" << std::endl;
		_tempImageFilenameVigra = ("vigratempfile_" + this->getName() + ".hdf5");
	}

	// make sure the temp image gets a type externsion
	size_t dotpos = _tempImageFilenameVigra.find_last_of(".");
	if(dotpos == std::string::npos)
	{
		sout<<"(WW) No file extension for temporary image file set" << std::endl;
		_tempImageFilenameVigra.append(".hdf5");
	}

	QDir tempdir(QFileInfo(QString::fromStdString(_tempImageFilenameVigra)).absoluteDir());

	// check if directory for temporary image exists
	if(!tempdir.exists())
	{
		ParameteredObject::raise("Cannot run script because temporay directory "
								"does not exist");
	}
	else if(!tempdir.isReadable())
	{
		ParameteredObject::raise("Cannot run script because temporay directory "
								"is not readable");
	}

	// set the command line arguments
	this->_cmdlineArgs << _mangleCmdlineArgs();

	if(vigraIn.connected())
	{
		//If temp file already exists && _useFileInsteadOfInput,
		//don't save the file, else save
		if((! QFile(QString(_tempImageFilenameVigra.c_str())).exists()) ||
				(! _useFileInsteadOfInput))
		{
			// save input file
			FileWriterHDF5<T>::writeToFile(vigraIn(),
										   _tempImageFilenameVigra,
										   _vigraPathInFile,
										   _vigraNoSingletonDimensions);
		}
		else
		{
			sout << _tempImageFilenameVigra <<
				 " already exists, using file on disk instead" << std::endl;
		}
	}
}

template <typename T>
void ScriptorVigra<T>::tidy()
{
	// load output
	FileReaderHDF5<T>::readHdf5(vigraOut(),
								_tempImageFilenameVigra,
								_vigraPathInFile);
	// remove temporary file
	if(std::remove(_tempImageFilenameVigra.c_str()) != 0)
	{
		ParameteredObject::raise("Error deleting file " + _tempImageFilenameVigra);
	}
}

template <typename T>
QStringList ScriptorVigra<T>::_mangleCmdlineArgs()
{
	QStringList args = _cmdlineArgsString.split(";", QString::SkipEmptyParts);

	bool vigraImageExists = QFile(QString::fromStdString(_tempImageFilenameVigra)).exists();

	if(_cmdlineArgsString.contains("\%vigraFilename"))
	{
		QFileInfo vigraFile(QString::fromStdString(_tempImageFilenameVigra));
		QString _vigraFilename = vigraFile.absoluteFilePath();

		if((vigraIn.connected()) || (vigraImageExists))
		{
			args.replaceInStrings("\%vigraFilename", _vigraFilename);
		}
		else
			ParameteredObject::raise("Cannot expand %vigraFilename because "
				"neither vigraIn is connected, nor is there a suitable File"
				" on disk");
	}
	return args;
}

#endif /* _SCRIPTOR_VIGRA_HXX_ */
