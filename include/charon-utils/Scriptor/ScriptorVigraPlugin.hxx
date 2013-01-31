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
/** \file Scriptor_vigra1.hxx
 *  Implementation of the parameter class Scriptor_vigra1.
 *  \author <a href="mailto:de_mox@zoho.com">
 *      mox</a>
 *  \date 29.02.2012
 */

#ifndef _SCRIPTOR_VIGRA1_HXX_
#define _SCRIPTOR_VIGRA1_HXX_

#include "ScriptorVigraPlugin.h"

template <typename T>
ScriptorVigraPlugin<T>::ScriptorVigraPlugin(const std::string& name) :
	ScriptorVigra<T>(name,
					"ScriptorVigra",
					"Run a script on an image<br>"
					"This plugin will save an Image to disk, "
					"execute a script and read it back in.")
{
	ParameteredObject::_addParameter(
		scriptpath, "scriptpath",
		"path to the script to run",
		"FileOpen");
	ParameteredObject::_addParameter<std::string>(
		arguments, "arguments",
		"command line arguments to the script<br>"
		"These special arguments will be expanded:"
		"<ul><li>%vigraFilename:\t"
		"expand to the filename that is used for the vigra image</li></ul>",
		"", "string");
	ParameteredObject::_addParameter<std::string>(
		interpreter, "interpreter",
		"command to invoke the interpreter",
		"", "string");
	ParameteredObject::_addParameter<std::string>(
		vigraPathInFile, "vigraPathInFile",
		"path to the image data in the file",
		"/data", "string");
	ParameteredObject::_addParameter< std::string >(
		filename, "filename",
		"basename of the temporary image file",
		"scriptorTempImage.hdf5",
		"filewrite");
	ParameteredObject::_addParameter(
		useFileInsteadOfInput, "useFileInsteadOfInput",
		"If an input is connected and the temporary file already exists, use "
		"file on disk instead of input",
		false);
	ParameteredObject::_addParameter(
		exitOnScriptError, "exitOnScriptError",
		"Should the plugin raise an exeption and stop the workflow "
		"if the script crashes",
		true);
	ParameteredObject::_addParameter(
		exitOnGeneralError, "exitOnGeneralError",
		"Should the plugin raise an exeption and stop the workflow "
		"if an error occurs?",
		true);
	ParameteredObject::_addParameter(
		prependInterpreter, "prependInterpreter",
		"Call the script like: "
		"&lt;interpreter&gt; &lt;scriptname&gt; &lt;args&gt;<br>"
		"instead of &lt;scriptname&gt; &lt;args&gt;<br>"
		"<b>Remark:</b> prepending the interpreter shifts the arguments "
		"to the left, eg. the first argument becomes the second",
		true);
	ParameteredObject::_addParameter(
		searchPathForScript, "searchPathForScript",
		"If the script is not found in current dir and lib dirs<br>"
		"also search in $PATH for the script file",
		true);
	ParameteredObject::_addParameter(
		vigraNoSingletonDimensions, "vigraNoSingletonDimensions",
		"This will reduce the dimensions of the output array "
		"skipping singleton dimensions (i.e. such dimensions with size 1)",
		true);
}

template <typename T>
void ScriptorVigraPlugin<T>::_setupVariables()
{ 
		/// Set up all variables needed to run the script
		/**
		 * These variables have to be set:
		 * <ul>
		 * <li>QString _interpreter</li>
		 * <li>bool _prependInterpreter</li>
		 * <li>QString _script</li>
		 * <li>QString _cmdlineArgsString</li>
		 * <li>QString _workDir</li>
		 * <li>bool _exitOnGeneralError</li>
		 * <li>bool _exitOnScriptError</li>
		 * <li>bool _searchPathForScript</li>
		 * <li>std::string _tempImageFilenameVigra</li>
		 * <li>bool _useFileInsteadOfInput</li>
		 * <li>std::string _vigraPathInFile</li>
		 * <li>bool _vigraNoSingletonDimensions</li>
		 * </ul>
		 * */

	std::string infilename = filename();
	size_t dotpos = infilename.find_last_of(".");
	if(dotpos == std::string::npos)
	{
		dotpos = infilename.length();
	}

	// image input at vigra slot is saved as /path/to/file/filename_vigra.ext
	this->_tempImageFilenameVigra = infilename.substr(0, dotpos) + "_vigra" + infilename.substr(dotpos, std::string::npos);

	this->_script = QString::fromStdString(scriptpath());

	this->_cmdlineArgsString = QString::fromStdString(arguments());

	this->_workDir = QDir::currentPath();

	this->_searchPathForScript = searchPathForScript();

	this->_useFileInsteadOfInput = useFileInsteadOfInput();

	this->_exitOnScriptError = exitOnScriptError();

	this->_exitOnGeneralError = exitOnGeneralError();

	this->_prependInterpreter = prependInterpreter();

	this->_interpreter = QString::fromStdString(interpreter());

	this->_vigraNoSingletonDimensions = vigraNoSingletonDimensions();

	this->_vigraPathInFile = vigraPathInFile();
}

#endif /* _SCRIPTOR_VIGRA1_HXX_ */

