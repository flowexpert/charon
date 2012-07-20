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
/** \file ScriptorBase.h
 *  Declaration of the parameter class ScriptorBase.
 *  \author <a href="mailto:de_mox@zoho.com">
 *      mox</a>
 *  \date 29.02.2012 */

#ifndef _SCRIPTOR_BASE_H_
#define _SCRIPTOR_BASE_H_

#ifdef _MSC_VER
#ifdef ScriptorBase_EXPORTS
/// Visual C++ specific code
#define ScriptorBase_DECLDIR __declspec(dllexport)
#else
#define ScriptorBase_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define ScriptorBase_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <QStringList>


/// Run a script in a charon workflow
/**
 * The classes derived from this will be able to execute a script or an external
 * program on an image
 * */
template <typename T>
class ScriptorBase_DECLDIR ScriptorBase : public TemplatedParameteredObject<T>
{
	protected:
		/// Command to invoke the interpreter
		QString _interpreter;
		/// Filename of the sript to be run
		QString _script;
		/// Command line arguments to the script
		/// Each argument is handed over as an individual string
		QStringList _cmdlineArgs;
		/// Directory to call the script from
		QString _workDir;
		/// Throw an exception if something goes wrong, or send a warning to console
		bool _exitOnGeneralError;
		/// Throw an exception if script crashes, or send a warning to console
		bool _exitOnScriptError;
		/// Should the plugin search the $PATH if the script is not found?
		bool _searchPathForScript;
		/// Should the script be called like <INTERPRETER> <SCRIPTNAME> <ARGS>
		/// or simply like <SCRIPTNAME> <ARGS>
		/// When prepending the interpreter, the arguments to the script are shifted
		/// eg. the first argument becomes the second
		bool _prependInterpreter;

		/// Save input images to disk
		virtual void prepare() = 0;

		/// Read image from disk to output
		virtual void tidy() = 0;

		/// constructor
		/** \param name          Instance name
		 *  \param classname     Class name
		 *  \param doc           Documentation for object inspector
		 * */
		ScriptorBase(const std::string& name,
					  const std::string& classname = "ScriptorBase",
					  const std::string& doc =
						  "Run a script or external program on an input image");
	public:

		/// Update object.
		void execute();

		/// search the filesystem for a file
		/**
		 * <ul>
		 * <li>0) check if file is found by it's name (which can include a path)</li>
		 * <li>1) search the file in current dir (where the parameterfile is)</li>
		 * <li>2) if the file is not in current dir, search in the plugin path,
		 * it includes the binary dir</li>
		 * <li>3) if the file does not even show up there, throw an exception</li>
		 *	</ul>
		 * */
		static QString findFile(QString filename, bool alsoSearchPath);
};

#include "ScriptorBase.hxx"

#endif // _SCRIPTOR_BASE_H_
