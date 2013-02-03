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
		"path to the image dataset in the file",
		"/data", "string");
	ParameteredObject::_addParameter<std::string>(
		vigraPathInFileAfter, "vigraPathInFileAfter",
		"path to the modified image dataset in the file "
		"(read back after execution)",
		"/result", "string");
	ParameteredObject::_addParameter< std::string >(
		filename, "filename",
		"name of the temporary image file, kept after execution",
		"scriptorTempImage.hdf5",
		"filewrite");
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
void ScriptorVigraPlugin<T>::_setupVariables() {
	this->_tempImageFilenameVigra = filename();
	this->_script = QString::fromStdString(scriptpath());
	this->_cmdlineArgsString = QString::fromStdString(arguments());
	this->_workDir = QDir::currentPath();
	this->_searchPathForScript = searchPathForScript();
	this->_exitOnScriptError = exitOnScriptError();
	this->_exitOnGeneralError = exitOnGeneralError();
	this->_prependInterpreter = prependInterpreter();
	this->_interpreter = QString::fromStdString(interpreter());
	this->_vigraNoSingletonDimensions = vigraNoSingletonDimensions();
	this->_vigraPathInFile = vigraPathInFile();
	this->_vigraPathInFileAfter = vigraPathInFileAfter();
}

#endif /* _SCRIPTOR_VIGRA1_HXX_ */

