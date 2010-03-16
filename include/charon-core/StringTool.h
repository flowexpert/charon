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
/** @file   StringTool.h
 *  @brief  Declaration of StringTool methods.
 *  @date   11.08.2008
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @remark Based on StringTool.hxx by Daniel Kondermann
 */

#ifndef _StringTool_H
#define _StringTool_H

#ifdef WINDOWS
#ifdef DLLEX
#undef DLLEX
#endif // DLLEX
#ifdef CREATE_SHARED
#define DLLEX __declspec(dllexport)
#else  // CREATE_SHARED
#define DLLEX __declspec(dllimport)
#endif // CREATE_SHARED
#else  // WINDOWS
#define DLLEX
#endif // WINDOWS

#include <string>
#include <vector>

/// Useful methods when working with strings.
namespace StringTool {
	/** Remove (whitespace) characters from the tail of a string.
	 *  @param s            source string
	 *  @param t            characters to remove
	 *  @return             modified string (copy)
	 */
	std::string DLLEX trimRight(const std::string& s,
			const std::string& t = " \t\r\n");

	/** Remove (whitespace) characters from the head of a string.
	 *  @param s            source string
	 *  @param t            characters to remove
	 *  @return             modified string (copy)
	 */
	std::string DLLEX trimLeft(const std::string& s,
			const std::string& t = " \t\r\n");

	/** Remove (whitespace) characters from head and tail of a string.
	 *  @param s            source string
	 *  @param t            characters to remove
	 *  @return             modified string (copy)
	 */
	std::string DLLEX trim(const std::string& s,
			const std::string& t = " \t\r\n");

	/** Use delimiter to split the string in a list of substrings.
	 *  @param str          source string
	 *  @param delimiter    delimiting character
	 *  @param result       list containing the found substrings
	 */
	void DLLEX explode(std::string str, char delimiter,
			std::vector<std::string>& result);

	/** Convert a string to lowercase.
	 *  @param s            input string
	 *  @return             lowercase output string
	 */
	std::string DLLEX toLowerCase(std::string s);
}

#endif /** _StringTool_H */
