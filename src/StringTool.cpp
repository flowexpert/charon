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
/**	@file	StringTool.cpp
 *	@brief	Implementation of class StringTool.
 *	@date	11.08.2008
 *	@author	<a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *	@remark	Based on StringTool.hxx by Daniel Kondermann
 */

#include "StringTool.h"
#include <sstream>
#include <algorithm>

std::string StringTool::trimRight(const std::string& s, const std::string& t) {
	std::string d(s);
	std::string::size_type i(d.find_last_not_of(t));
	if (i == std::string::npos)
		return "";
	else
		return d.erase(d.find_last_not_of(t) + 1);
}

std::string StringTool::trimLeft(const std::string& s, const std::string& t) {
	std::string d(s);
	return d.erase(0, s.find_first_not_of(t));
}

std::string StringTool::trim(const std::string& s, const std::string& t) {
	std::string d(s);
	return trimLeft(trimRight(d, t), t);
}

void StringTool::explode(std::string str, char delimiter,
		std::vector<std::string>& result) {
	std::istringstream strm(str);
	std::string p;
	while (std::getline(strm, p, delimiter)) {
		result.push_back(p);
	}
}

std::string StringTool::toLowerCase(std::string s) {
	transform(s.begin(), s.end(), s.begin(),
				(int(*)(int)) tolower);
	return s;
}
