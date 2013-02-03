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
/** \file Scriptor_vigra.h
 *  Declaration of the parameter class Scriptor_vigra.
 *  \author <a href="mailto:de_mox@zoho.com">
 *      mox</a>
 *  \date 29.02.2012
 */

#ifndef _SCRIPTOR_VIGRA_H_
#define _SCRIPTOR_VIGRA_H_

#ifdef _MSC_VER
#ifdef ScriptorVigra_EXPORTS
/// Visual C++ specific code
#define ScriptorVigra_DECLDIR __declspec(dllexport)
#else
#define ScriptorVigra_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define ScriptorVigra_DECLDIR
#endif

#include "ScriptorBase.h"
#include <QCoreApplication>
#include <vigra/multi_array.hxx>

/// Read an image on input slot, run a script on it and output the image to a slot
/**
 * Derive from this class to create a Module that runs _Your_ script
 *
 * Ususally, You should only need to overwrite  _setupVariables() and add the
 * parameters You need.
 * ScriptorVigraPlugin is an example of this
 */
template <typename T>
class ScriptorVigra_DECLDIR ScriptorVigra : public ScriptorBase<T>
{
	private:
		/// Save input images to disk
		virtual void prepare();

		/// Read image from disk to output
		virtual void tidy();

		/// Process the command line arguments to the script
		/**
		 * Split the command line arguments to the script into individual
		 * substrings with one argument each.<br>
		 * The special string \%vigraFilename will be expanded to the actual
		 * name of the image file
		 * */
		QStringList _mangleCmdlineArgs();

	protected:
		/// Filename for the image connected to the vigraIn input
		std::string _tempImageFilenameVigra;

		/// The command line arguments to the script, separated by semicolons
		/// Each argument is handed over as an individual string
		QString _cmdlineArgsString;

		/// path to the image data within the hdf5 file
		/** this is the dataset the input slot data is written to */
		std::string _vigraPathInFile;

		/// path to the image data within the hdf5 file after modification
		/** this is the dataset that is read back after script execution */
		std::string _vigraPathInFileAfter;

		/// Do not write singleton dimensions
		/** This will reduce the dimensions of the output array
		 *  skipping singleton dimensions (i.e. such dimensions with size 1).
		 */
		bool _vigraNoSingletonDimensions;

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
		 * <li>std::string _vigraPathInFile</li>
		 * <li>std::string _vigraPathInFileAfter</li>
		 * <li>bool _vigraNoSingletonDimensions</li>
		 * </ul>
		 * */
		virtual void _setupVariables()=0;

		/// constructor
		/** parameters
		 *  \param name          Instance name
		 *  \param classname     Class name
		 *  \param doc           Documentation for object inspector
		 *  */
		ScriptorVigra(const std::string& name,
					  const std::string& classname = "Scriptor_vigra",
					  const std::string& doc =
						  "Run a script or external program on an input vigraImg");

	public:
		/// image data as input slot
		InputSlot < vigra::MultiArrayView<5, T> > vigraIn;

		/// output image
		OutputSlot< vigra::MultiArray<5, T> > vigraOut;
};

#endif // _SCRIPTOR_VIGRA_H_
