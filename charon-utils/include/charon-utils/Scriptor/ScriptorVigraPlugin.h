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
/** \file Scriptor_vigra1.h
 *  Declaration of the parameter class Scriptor_vigra1.
 *  \author <a href="mailto:de_mox@zoho.com">
 *      mox</a>
 *  \date 29.02.2012
 */

#ifndef _SCRIPTOR_VIGRA1_H_
#define _SCRIPTOR_VIGRA1_H_

#ifdef _MSC_VER
#ifdef scriptor_vigra1_EXPORTS
/// Visual C++ specific code
#define scriptor_vigra1_DECLDIR __declspec(dllexport)
#else
#define scriptor_vigra1_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define scriptor_vigra1_DECLDIR
#endif

#include "ScriptorVigra.h"

/// Run a script on an image, all settings are available as Parameters
/** 
 * This module demonstrates how to derive an actual module from Scriptor
 */
template <typename T>
class ScriptorVigra_DECLDIR ScriptorVigraPlugin : public ScriptorVigra<T>
{
	protected:
		virtual void _setupVariables();

	public:

		/// constructor
		/** \param name instance name of the plugin */
		ScriptorVigraPlugin(const std::string& name);

		/// should the plugin continue or throw an exception when the script crashes
		Parameter<bool> exitOnScriptError;

		/// should the plugin continue or throw an exception when an error occurs
		Parameter<bool> exitOnGeneralError;

		/// Call the script like <interpreter> <scriptname> <args>
		/// instead of <scriptname> <ars>
		Parameter<bool> prependInterpreter;

		/// path to the script to run
		Parameter< std::string > scriptpath;

		/// path to the image data within the hdf5 file
		/** this is the dataset the input slot data is written to */
		Parameter< std::string > vigraPathInFile;

		/// path to the image data within the hdf5 file after modification
		/** this is the dataset that is read back after script execution */
		Parameter< std::string > vigraPathInFileAfter;

		/// command to invoke the interpreter
		Parameter< std::string > interpreter;

		/// arguments to the script
		Parameter< std::string > arguments;

		/// Path to the temporary image
		Parameter< std::string > filename;

		/// Search the path for the script, too?
		Parameter<bool> searchPathForScript;

		/// Should singleton dimensions be skipped on writing
		/** This will reduce the dimensions of the output array
		 *  skipping singleton dimensions (i.e. such dimensions with size 1).
		 */
		Parameter<bool> vigraNoSingletonDimensions;
};

#endif // _SCRIPTOR_VIGRA1_H_
