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
/** \file ScriptorBase.hxx
 *  Implementation of the parameter class ScriptorBase.
 *  \author <a href="mailto:de_mox@zoho.com">
 *      mox</a>
 *  \date 29.02.2012
 */

#ifndef _SCRIPTOR_BASE_HXX_
#define _SCRIPTOR_BASE_HXX_

#include "ScriptorBase.h"
#include <QRegExp>
#include <QFileInfo>
#include <QCoreApplication>
#include "ScriptorExecuter.h"
#include <QDir>
#include <charon-utils/CImg.h>
#include <charon-core/ParameteredObject.hxx>

template <typename T>
ScriptorBase<T>::ScriptorBase(const std::string& name,
								const std::string& classname,
								const std::string& doc):
	TemplatedParameteredObject<T>(classname, name, doc)
{}


template <typename T>
void ScriptorBase<T>::execute()
{

	try
	{
		//clear all variables
		_interpreter.clear();
		_script.clear();
		_cmdlineArgs.clear();
		_workDir.clear();

		// Set up all variables and save the input to disk
		prepare();

		// check if _script is specified
		if(_script.isEmpty())
		{
			ParameteredObject::raise("no script specified, scriptor will not do anything");
		}

		// Find and save the script
		QString absoluteScriptName = findFile(_script, _searchPathForScript);

		// make sure the script is executable
		if(!QFileInfo(absoluteScriptName).isExecutable())
		{
			ParameteredObject::raise("specified script is not executable");
		}

		// make sure we can call the script
		if(_prependInterpreter)
		{
			// check if _script is specified
			if(_script.isEmpty())
			{
				ParameteredObject::raise("no script specified, scriptor will not do anything");
			}
			_cmdlineArgs.insert(0, absoluteScriptName);

			//if os==win && !interpreter ends with .exe
			//append .exe to interpreter
			//if os!=win && interpreter ends with .exe
			//append .exe to interpreter
			// if OS is Windows
			#ifdef Q_OS_WIN32
			if(!_interpreter.endsWith(".exe",Qt::CaseInsensitive))
			{
					_interpreter.append(".exe");
			}
			#else // OS is not Windows
			if(_interpreter.endsWith(".exe",Qt::CaseInsensitive))
			{
					_interpreter.chop(4);
			}
			#endif /* Q_OS_WIN32*/
			_script = findFile(_interpreter, _searchPathForScript);

			// make sure the interpreter is executable
			if(!QFileInfo(_script).isExecutable())
			{
				ParameteredObject::raise("specified interpreter is not executable");
			}
		}
		
		// validate the working directory
		QFileInfo workingDirectory(_workDir);
		if((!workingDirectory.exists()) || (!workingDirectory.isDir()))
		{
			ParameteredObject::raise("specified working directory is invalid");
		}

//##########run the script######################################################

		ScriptorExecuter proc(absoluteScriptName,
							   _cmdlineArgs,
							   _workDir);

		if(proc.run())
		{
			std::ostringstream msg;
			msg << "Script " << _script.toStdString() << " crashed.\n\tExit code: " << proc.exitCode();
			ParameteredObject::raise(msg.str());
		}
		else
		{
			sout << "Script execution finished. Exit Code: " << proc.exitCode() << std::endl ;
		}
//\#########run the script######################################################

		// load the edited image from disk to output
		tidy();
	}
	catch
		(const cimg_library::CImgException& err)
	{
		if(_exitOnGeneralError)
		{
			throw;
		}
		else
		{
			sout << "(EE) " << err.what() << std::endl;
		}
	}

	catch
		(std::runtime_error& err)
	{
		//check if this exception belongs to the script
		std::string errstring(err.what());
		if((errstring.find("Script ") != std::string::npos) &&
				(errstring.find(" crashed.\n\tExit code: ") != std::string::npos))
		{
			if(_exitOnScriptError)
			{
				throw;
			}
			else
			{
				sout << "(EE) " << err.what() << std::endl;
			}
		}
		else
		{
			if(_exitOnGeneralError)
			{
				throw;
			}
			else
			{
				sout << "(EE) " << err.what() << std::endl;
			}
		}
	}

}

template <typename T>
QString ScriptorBase<T>::findFile(QString filename, bool alsoSearchPath)
{
	QFileInfo file(filename);
//	sout << "(DD) original filename "
//	<< file.absoluteFilePath().toStdString() << std::endl;

	if(!file.exists()) // file not found by it's own path
	{
		//discard path to file, extract only the nanme
		QString qFileName = file.fileName();
		file = QFileInfo(qFileName);
//		sout << "(DD) current dir filename "
//		<< file.absoluteFilePath().toStdString() << std::endl;

		if(!file.exists()) // file not in current dir
		{
			//iterate over the dirs of the dynamically loading libs
			QStringList libPaths = QCoreApplication::libraryPaths();
			QStringList::const_iterator constIterator;
			for(constIterator = libPaths.constBegin();
					((constIterator != libPaths.constEnd()) && (!file.exists()));
					++constIterator) // file not found yet
			{
				QString qfile = *constIterator;
//				sout << "(DD) libPaths "<< qfile.toStdString() << std::endl;
				qfile += "/";
				qfile += qFileName;
				file = QFileInfo(qfile);
//				sout << "(DD) lib dir filename "
//				<< file.absoluteFilePath().toStdString() << std::endl;
			}
			if((!file.exists()) && (alsoSearchPath))
			// file not found in current, binary or lib dir
			{
				QString PATH(qgetenv("PATH"));
//				sout << "PATH " << PATH.toStdString() << std::endl;
				QStringList paths ;
				#ifdef WINDOWS
					paths= PATH.split(';', QString::SkipEmptyParts);
				#else
					paths= PATH.split(':', QString::SkipEmptyParts);
				#endif
				QStringList::const_iterator constIterator;
				for(constIterator = paths.constBegin();
						((constIterator != paths.constEnd()) && (!file.exists()));
						++constIterator) // file not found yet
				{
					QString qfile = *constIterator;
//					sout << "libPaths "<< qfile.toStdString() << std::endl;
					qfile += "/";
					qfile += qFileName;
					file = QFileInfo(qfile);
//					sout << "(DD) path dir filename "
//					<< file.absoluteFilePath().toStdString() << std::endl;
				}

			}
			if(!file.exists()) // file not found in current, binary or lib dir
			{
				throw std::runtime_error(filename.toStdString() +
			 	" is neither a valid path, nor can it be found in current dir, "
				"binary dir or the dirs of the dynamically loading libraries");
			}
		}
	}
	return file.absoluteFilePath();
}

#endif /* _SCRIPTOR_BASE_HXX_ */
