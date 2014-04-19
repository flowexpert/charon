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
/** \file ScriptorCimg.h
 *  Declaration of the parameter class ScriptorCimg.
 *  \author <a href="mailto:de_mox@zoho.com">
 *      mox</a>
 *  \date 29.02.2012
 */

#ifndef _SCRIPTOR_CIMG_H_
#define _SCRIPTOR_CIMG_H_

#ifdef _MSC_VER
#ifdef ScriptorCimg_EXPORTS
/// Visual C++ specific code
#define ScriptorCimg_DECLDIR __declspec(dllexport)
#else
#define ScriptorCimg_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define ScriptorCimg_DECLDIR
#endif

#include "ScriptorBase.h"
#include <QCoreApplication>
#include <charon-utils/CImg.h>

///	Read an image on input slot, run a script on it and output the image to a slot
/**
 * Derive from this class to create a Module that runs Your script
 *
 * Ususally, You should only need to overwrite  _setupVariables() and add the
 * parameters You need.
 * ScriptorCimgPlugin and ScriptorCimg2 are examples of this
 */
template <typename T>
class ScriptorCimg_DECLDIR ScriptorCimg : public ScriptorBase<T>
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
		 * The special string \%cimgFilename will be expanded to the actual
		 * name of the image file
		 * */
		QStringList _mangleCmdlineArgs();

	protected:
		/// Filename for the image connected to the cimgIn input
		std::string _tempImageFilenameCimg;

		/// The command line arguments to the script, separated by semicolons
		/// Each argument is handed over as an individual string
		QString _cmdlineArgsString;

		/// If there is a file on disk that is named like the temp file
		/// should it be used instead of the input image
		bool _useFileInsteadOfInput;

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
		virtual void _setupVariables()=0;

		/// default constructor
		/** parameters
		 *  \param name          Instance name
		 *  \param classname     Class name
		 *  \param doc           Documentation for object inspector
		 *  */
		ScriptorCimg(const std::string& name,
					  const std::string& classname = "ScriptorCimg",
					  const std::string& doc =
						  "Run a script or external program on an input cImg");

	public:
		/// image data as input slot
		InputSlot< cimg_library::CImgList<T> > cimgIn;

		/// output image
		OutputSlot< cimg_library::CImgList<T> > cimgOut;
};

#endif // _SCRIPTOR_CIMG_H_
