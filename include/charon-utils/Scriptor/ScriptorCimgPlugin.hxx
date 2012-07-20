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
/** \file ScriptorCimgPlugin.hxx
 *  Implementation of the parameter class ScriptorCimgPlugin.
 *  \author <a href="mailto:de_mox@zoho.com">
 *      mox</a>
 *  \date 29.02.2012
 */

#ifndef _SCRIPTOR_CIMG1_HXX_
#define _SCRIPTOR_CIMG1_HXX_

#include "ScriptorCimgPlugin.h"

template <typename T>
ScriptorCimgPlugin<T>::ScriptorCimgPlugin(const std::string& name) :
	ScriptorCimg<T>(name,
					"ScriptorCimg",
					"Run a script or external executable<br>"
					"This plugin will save an Image to disk, "
					"execute a script or program and then read the image back in.<br>"
					"The programs stdout will be printet to the process window<br>"
					"<h3>Python Script Example</h3><br>"
					"To run the python script /home/dude/script.py set the following parameters:<br>"
					"<ul><li>scriptpath: /home/dude/script.py</li>"
					"<li>arguments: %cimgFilename</li>"
					"<li>interpreter: python  (in windows you must add .exe)</li>"
					"<li>prependInterpreter: checked</li>"
					"</ul>this is equivalent to using the following command line: python /home/dude/scirpt.py &lt;path to image file&gt;<br>"
					"<h3>Windows Powershell Example:</h3><br>"
					"To run the powershell script script.ps1 (located in the same directory as the workflow wpr file) set the following parameters:<br>"
					"<ul><li>scriptpath: powershell.exe  (assumed that powershell.exe is included in your PATH environment variable</li>"
					"<li>arguments: -File;script.ps1;&lt;1. argument to script&gt;&lt;2. argument to script&gt;</li>"
					"<li>interpreter: keep empty</li>"
					"<li>filename: arbitrary, can be passed to script as command line arguemnt</li>"
					"<li>prependInterpreter: unchecked</li>"
					"<li>searchPathForScript: when unchecked the full path to the powershell executable must be provided as the scriptpath parameter</li>"
					"</ul>This is equivalent to using the following command line: powershell.exe -File script.ps1<br>")
{
	ParameteredObject::_addParameter(
		scriptpath, "scriptpath",
		"path to the script or executable to run",
		"FileOpen");
	ParameteredObject::_addParameter<std::string>(
		arguments, "arguments",
		"command line arguments to the script<br>"
		"Individual arguments must be separated by semicolon (;)<br>"
		"These special arguments will be expanded<br>"
		"<li>%cimgFilename:\t"
		"expand to the filename that is used for the cimg image</li>"
		"", "string");
	ParameteredObject::_addParameter<std::string>(
		interpreter, "interpreter",
		"command to invoke the interpreter",
		"", "string");
	ParameteredObject::_addParameter< std::string >(
		filename, "filename",
		"basename of the temporary image file",
		"scriptorTempImage.png",
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
		"Call the script like: &lt;interpreter&gt; &lt;scriptname&gt; &lt;args&gt;<br>"
		"instead of &lt;scriptname> &lt;args&gt;<br>"
		"<em>prepending the interpreter shifts the arguments to the left</em><br>"
		"<em>eg. the first argument becomes the second</em>",
		true);
	ParameteredObject::_addParameter(
		searchPathForScript, "searchPathForScript",
		"If the script is not found in current dir and lib dirs<br>"
		"also search in $PATH for the script file",
		true);
}

template <typename T>
void ScriptorCimgPlugin<T>::_setupVariables()
{
		/// Set up all variables needed to run the script
		/** These variables have to be set:
		 *  QString _interpreter;
		 *  bool _prependInterpreter;
		 *  QString _script;
		 *  QString _cmdlineArgsString;
		 *  QString _workDir;
		 *  bool _exitOnGeneralError;
		 *  bool _exitOnScriptError;
		 *  bool _searchPathForScript;
		 *  std::string _tempImageFilenameCimg;
		 *  bool _useFileInsteadOfInput;
		 *  */

	std::string infilename = filename();
	size_t dotpos = infilename.find_last_of(".");
	if(dotpos == std::string::npos)
	{
		dotpos = infilename.length();
	}

	// image input at cimg slot is saved as /path/to/file/filename_cimg.ext
	this->_tempImageFilenameCimg = infilename.substr(0, dotpos) + "_cimg" + infilename.substr(dotpos, std::string::npos);

	this->_script = QString::fromStdString(scriptpath());

	this->_cmdlineArgsString = QString::fromStdString(arguments());

	this->_workDir = QDir::currentPath();

	this->_searchPathForScript = searchPathForScript();

	this->_useFileInsteadOfInput = useFileInsteadOfInput();

	this->_exitOnScriptError = exitOnScriptError();

	this->_exitOnGeneralError = exitOnGeneralError();

	this->_prependInterpreter = prependInterpreter();

	this->_interpreter = QString::fromStdString(interpreter());
}

#endif /* _SCRIPTOR_CIMG1_HXX_ */
