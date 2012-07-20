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
/** \file ScriptorCimgPlugin.h
 *  Declaration of the parameter class ScriptorCimgPlugin.
 *  \author <a href="mailto:de_mox@zoho.com">
 *      mox</a>
 *  \date 29.02.2012
 */

#ifndef _SCRIPTOR_CIMG1_H_
#define _SCRIPTOR_CIMG1_H_

#ifdef _MSC_VER
#ifdef ScriptorCimg_EXPORTS
/// Visual C++ specific code
#define ScriptorCimgPlugin_DECLDIR __declspec(dllexport)
#else
#define ScriptorCimgPlugin_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define ScriptorCimgPlugin_DECLDIR
#endif

#include "ScriptorCimg.h"

///Run a script on an image, all settings are available as Parameters
/** 
 * This module demonstrates how to derive an actual module from Scriptor
 */
template <typename T>
class ScriptorCimgPlugin_DECLDIR ScriptorCimgPlugin : public ScriptorCimg<T>
{
	private:

		/// Set up all variables needed to run the script
		/** These variables have to be set:
		 *  <ul>
		 *  <li>QString _interpreter</li>
		 *  <li>bool _prependInterpreter</li>
		 *  <li>QString _script</li>
		 *  <li>QString _cmdlineArgsString</li>
		 *  <li>QString _workDir</li>
		 *  <li>bool _exitOnGeneralError</li>
		 *  <li>bool _exitOnScriptError</li>
		 *  <li>bool _searchPathForScript</li>
		 *  <li>std::string _tempImageFilenameCimg</li>
		 *  <li>bool _useFileInsteadOfInput</li>
		 *  </ul>
		 *  */
		virtual void _setupVariables();
	public:
		///constructor
		/**
		 * \param name instance name of the plugin
		 * */
		ScriptorCimgPlugin(const std::string& name);

		///If an input is connected and the temporary file already exists,
		///use file on disk instead of input
		Parameter<bool> useFileInsteadOfInput;

		///should the plugin continue or throw an exception when the script crashes
		Parameter<bool> exitOnScriptError;

		///should the plugin continue or throw an exception when an error occurs
		Parameter<bool> exitOnGeneralError;

		///Call the script like <interpreter> <scriptname> <args>
		///instead of <scriptname> <ars>
		Parameter<bool> prependInterpreter;

		/// path to the script to run
		Parameter< std::string > scriptpath;

		/// command to invoke the interpreter
		Parameter< std::string > interpreter;

		/// arguments to the script
		Parameter< std::string > arguments;

		/// Path to the temporary image
		Parameter< std::string > filename;

		/// Search the path for the script, too?
		Parameter<bool> searchPathForScript;
};

#include "ScriptorCimgPlugin.hxx"

#endif // _SCRIPTOR_CIMG1_H_
