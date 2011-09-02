/*  This file is part of Charon.

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
/** @file   FileTool.h
 *  @brief  Declaration of FileTool methods.
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
 *  @date   11.08.2008
 *  @remark Based on FileTool.hxx by Daniel Kondermann.
 */

#ifndef _FileTool_H_
#define _FileTool_H_

#include <string>
#include <vector>
#include "DllEx.h"

/// Useful functions when working with files.
/** This wraps the operating system specific functions.
 */
namespace FileTool {
	/// Slash of current operating system ("/" or "\")
	extern const char charon_core_DLL_PUBLIC slash;

	/// This function creates all directories contained
	/// in path provided they do not already exist.
	/// \param path     path to create
	/// \returns result of final mkdir
	int charon_core_DLL_PUBLIC makePath(std::string& path);

	/// Create a new directory using MODE 711 (in unix)
	/// @param dir      name of the directory to create
	/// @return         result of mkdir
	int charon_core_DLL_PUBLIC makeDir(const std::string& dir);

	/// Change current working directory.
	/// @param dir      new working directory
	/// @return         result of chdir
	int charon_core_DLL_PUBLIC changeDir(const std::string& dir);

	/// Get current working directory.
	/// @return         string containing current working directory
	std::string charon_core_DLL_PUBLIC getCurrentDir();

	/// Search files with given suffix.
	/** Iterates through the files of the current working directory
	 *  and looks for files with the given suffix.
	 *  @param suffix   Suffix of the requested files
	 *  @return         Vector containing the file names
	 */
	std::vector<std::string> charon_core_DLL_PUBLIC getFilesWithSuffix(
		std::string suffix);

	/// Convert unix to windows paths and reversed.
	/// The direction is determined by the current running operating system.
	/// @param src      source string to convert (will be modified)
	void charon_core_DLL_PUBLIC slashConvert(std::string& src);

	/// Check if file exists.
	/// @param file     name of the file to look for
	/// @return         true if file exists, otherwise false
	bool charon_core_DLL_PUBLIC exists(const std::string& file);

	/// Remove file.
	/// @param file     name of the file to remove
	/// @return         result of unlink
	int charon_core_DLL_PUBLIC remove(const std::string& file);

	/// Rename file.
	/// @param oldFile  name of the file to rename
	/// @param newFile  new name
	/// @return         result of rename
	int charon_core_DLL_PUBLIC rename(
		const std::string& oldFile, const std::string& newFile);

	/// read file content into std::string
	/** Reads a file and writes it into a std::string.
	 *  @param fName    File to read
	 *  @return         Content of the file
	 */
	std::string charon_core_DLL_PUBLIC readFile(const std::string & fName);
}

#endif // _FileTool_H_
