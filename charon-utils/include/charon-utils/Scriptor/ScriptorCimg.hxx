/*  Copyright (C) 2012 mox

	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file ScriptorCimg.hxx
 *  Implementation of the parameter class ScriptorCimg.
 *  \author <a href="mailto:de_mox@zoho.com">
 *      mox</a>
 *  \date 29.02.2012
 */

#ifndef _SCRIPTOR_CIMG_HXX_
#define _SCRIPTOR_CIMG_HXX_

#include "ScriptorCimg.h"
#include <QString>
//#include <QDir>

template <typename T>
ScriptorCimg<T>::ScriptorCimg(const std::string& name,
								const std::string& classname,
								const std::string& doc):
	ScriptorBase<T>(name, classname, doc),
	cimgIn(true, false)
{
	ParameteredObject::_addInputSlot(
		cimgIn, "cimgIn",
		"input image, format determined by ext. of the filename",
		"CImgList<T>");

	ParameteredObject::_addOutputSlot(
		cimgOut, "cimgOut",
		"output image",
		"CImgList<T>");
}

template <typename T>
void ScriptorCimg<T>::prepare()
{
	// setup everything we need to run the script
	_setupVariables();

	// check if the filename is set at all
	if(_tempImageFilenameCimg.empty())
	{
		sout<<"(WW) No filename for temporary image file set" << std::endl;
		_tempImageFilenameCimg = ("cimgtempfile_" + this->getName() + ".png");
	}

	// make sure the temp image gets a type externsion
	size_t dotpos = _tempImageFilenameCimg.find_last_of(".");
	if(dotpos == std::string::npos)
	{
		sout<<"(WW) No file extension for temporary image file set" << std::endl;
		_tempImageFilenameCimg.append(".png");
	}

	QDir tempdir(QFileInfo(QString::fromStdString(_tempImageFilenameCimg)).absoluteDir());

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

	if(cimgIn.connected())
	{
		//If temp file already exists && _useFileInsteadOfInput,
		//don't save the file, else save
		if((! QFile(QString::fromStdString(_tempImageFilenameCimg)).exists()) ||
				(! _useFileInsteadOfInput))
		{
			// save input file
			cimgIn().save(_tempImageFilenameCimg.c_str());
		}
		else
		{
			sout << _tempImageFilenameCimg <<
				 " already exists, using file on disk instead" << std::endl;
		}
	}
}

template <typename T>
void ScriptorCimg<T>::tidy()
{
	// load output
	cimgOut().load(_tempImageFilenameCimg.c_str());
	// remove temporary file
	if(std::remove(_tempImageFilenameCimg.c_str()) != 0)
	{
		ParameteredObject::raise("Error deleting file " + _tempImageFilenameCimg);
	}
}

template <typename T>
QStringList ScriptorCimg<T>::_mangleCmdlineArgs()
{
	QStringList args = _cmdlineArgsString.split(";", QString::SkipEmptyParts);

	bool cimgImageExists = QFile(QString::fromStdString(_tempImageFilenameCimg)).exists();

	if(_cmdlineArgsString.contains("%cimgFilename"))
	{
		QFileInfo cimgFile(QString::fromStdString(_tempImageFilenameCimg));
		QString _cimgFilename = cimgFile.absoluteFilePath();

		if((cimgIn.connected()) || (cimgImageExists))
		{
			args.replaceInStrings("%cimgFilename", _cimgFilename);
		}
		else
			ParameteredObject::raise("Cannot expand %cimgFilename because "
				"neither cimgIn is connected, nor is there a suitable File"
			    " on disk");
	}
	return args;
}

#endif /* _SCRIPTOR_CIMG_HXX_ */
