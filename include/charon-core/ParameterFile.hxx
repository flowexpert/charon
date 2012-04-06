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
/** @file ParameterFile.hxx
 *  Implementation of the template class ParameterFile.
 *  @author <a href="mailto:daniel.kondermann@iwr.uni-heidelberg.de">
 *      Daniel Kondermann</a>
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *
 *  @date 14.08.2008
 *
 *  This is only the implementation. Declarations are in the file
 *  ParameterFile.h.
 */
#ifndef _ParameterFile_HXX
#define _ParameterFile_HXX

#include "ParameterFile.h"
#include "SplitStream.h"
#include "StringTool.h"
#include "FileTool.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <cassert>

inline bool ParameterFile::isSet(std::string parameter) const {
	_toLower(parameter);
	return (_params.find(parameter) != _params.end());
}

template<class T>
inline void ParameterFile::set(std::string parameter, const T& value) {
	std::ostringstream str;
	str << value;
	_set(parameter, str.str());
}

template<class T>
inline void ParameterFile::set(std::string parameter,
		const std::vector<T>& value) {
	if (value.size()) {
		std::ostringstream str;
		str << value[0];
		for (unsigned int i = 1; i < value.size(); i++) {
			str << ";" << value[i];
		}
		_set(parameter, str.str());
	}
}

template<class T>
inline T ParameterFile::get(std::string parameter) const {
	_toLower(parameter);
	std::map<std::string, std::string>::const_iterator found;
	found = _params.find(parameter);
	if (found == _params.end())
		throw ParameterFile::Unset("Parameter " + parameter + " not set");

	std::istringstream strm(found->second);
	T ret;
	strm >> ret;
	return ret;
}

template<class T>
inline T ParameterFile::get(std::string parameter, T defaultValue) {
	if (isSet(parameter))
		return get<T> (parameter);
	else {
		if (_noFoundWarnings)
			sout << "(WW) setting unkown parameter '" << parameter << "' to '"
					<< defaultValue << "'" << std::endl;
		set<T> (parameter, defaultValue);
		return defaultValue;
	}
}

template<class T>
inline std::vector<T> ParameterFile::getList(std::string parameter) const {
	_toLower(parameter);
	std::vector<T> result;
	std::map<std::string, std::string>::const_iterator found;
	found = _params.find(parameter);
	if (found == _params.end())
		throw ParameterFile::Unset("Parameter " + parameter + " not set");

	std::string value = found->second;
	if (value != "" && value != "none") {
		std::vector<std::string> tmp;
		StringTool::explode(value, _delimiter, tmp);

		result.resize(tmp.size());
		for (unsigned int i = 0; i < tmp.size(); ++i) {
			std::istringstream strm(tmp[i]);
			T v;
			strm >> v;
			result[i] = v;
		}
	}
	return result;
}

template<class T>
inline std::vector<T> ParameterFile::getList(std::string parameter,
		std::string defaultValue) {
	if (isSet(parameter))
		return getList<T> (parameter);
	else {
		if (_noFoundWarnings)
			sout << "(WW) setting unkown parameter '"
					<< parameter << "' to '"
					<< defaultValue << "'" << std::endl;
		set(parameter, defaultValue);
		return getList<T> (parameter);
	}
}

inline std::string ParameterFile::parName(std::string name) {
	std::string::size_type pos = name.find(".");
	if (pos != std::string::npos)
		name = name.substr(pos + 1);
	return name;
}

inline std::string ParameterFile::objName(std::string name) {
	std::string::size_type pos = name.find(".");
	if (pos != std::string::npos)
		name = name.substr(0, pos);
	return name;
}

inline void ParameterFile::_toLower(std::string& input) const {
	std::transform(
		input.begin(), input.end(), input.begin(),
		(int(*)(int)) tolower);
}

// specialized versions

template<>
inline std::string ParameterFile::get(std::string parameter) const {
	_toLower(parameter);
	std::map<std::string, std::string>::const_iterator found;
	found = _params.find(parameter);
	if (found == _params.end())
		throw ParameterFile::Unset("Parameter " + parameter + " not set");

	// ensure that full string is returned
	return found->second;
}

template<>
inline bool ParameterFile::get(std::string parameter) const {
	_toLower(parameter);
	std::map<std::string, std::string>::const_iterator found;
	found = _params.find(parameter);
	if (found == _params.end())
		throw ParameterFile::Unset("Parameter " + parameter + " not set");

	std::string val = found->second;
	_toLower(val);
	if (val == "true")
		return true;
	if (val == "false")
		return false;

	// check for numbers
	std::istringstream strm(val);
	bool v;
	strm >> v;
	return v;
}

template<>
inline std::vector<std::string> ParameterFile::getList(
		std::string parameter) const {
	_toLower(parameter);
	std::vector<std::string> result;
	std::map<std::string, std::string>::const_iterator found;
	found = _params.find(parameter);
	if (found == _params.end())
		throw ParameterFile::Unset("Parameter " + parameter + " not set");
	std::string value = found->second;
	if (value != "" && value != "none") {
		StringTool::explode(value, _delimiter, result);

		if (_convertSlashes) {
			for (unsigned int i = 0; i < result.size(); ++i)
				FileTool::slashConvert(result[i]);
		}
	}
	return result;
}

#endif // _ParameterFile_HXX
