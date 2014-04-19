/*  This file is part of Charon.

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
/**	@file ParameterFile.cpp
 *	Implementation of the non-template-parts of class ParameterFile.
 *	@author <a href="mailto:daniel.kondermann@iwr.uni-heidelberg.de">
 *		Daniel Kondermann</a>
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *
 *	Original file written by Daniel Kondermann, modified by Jens-Malte
 *	Gottfried.
 *
 *	@date 25.09.2008
 */

#include <charon-core/ParameterFile.hxx>
#include <set>

ParameterFile::IoError::IoError(const std::string& msg) :
		std::runtime_error(msg.c_str()) {
}

ParameterFile::Unset::Unset(const std::string& msg) :
		std::invalid_argument(msg) {
}

ParameterFile::ParameterFile() :
	_noFoundWarnings(false), _convertSlashes(true), _delimiter(';') {
}

ParameterFile::ParameterFile(std::string fileName) :
	_noFoundWarnings(false), _convertSlashes(true), _delimiter(';') {
	load(fileName);
}

ParameterFile::~ParameterFile() {
}

void ParameterFile::setDelimiter(char delimiter) {
	_delimiter = delimiter;
}

void ParameterFile::setConvertSlashes(bool convertSlashes) {
	_convertSlashes = convertSlashes;
}

void ParameterFile::_set(std::string parameter, std::string value) {
	if (isSet(parameter))
		_setParams["[mod] " + parameter] = value;
	else {
		_setParams["      " + parameter] = value;
		_parameterLines.push_back(parameter);
	}
	_toLower(parameter);
	_params[parameter] = value;
}

void ParameterFile::clear() {
	_parameterLines.clear();
	_params.clear();
	_setParams.clear();
}

void ParameterFile::resetSetParams() {
	_setParams.clear();
}

void ParameterFile::setNotFoundWarningsOn(bool noFoundWarnings) {
	_noFoundWarnings = noFoundWarnings;
}

bool ParameterFile::save(std::string fileName) const {
	std::ofstream file(fileName.c_str(), std::ios::trunc);
	if (file.bad()) {
		throw ParameterFile::IoError(
				"Parameter file \"" + fileName + "\" could not be saved. "
				"(file.bad())");
	} else {
		toStream(file);
		file.close();
		return true;
	}
}

bool ParameterFile::load(std::string fileName) {
	if (!FileTool::exists(fileName))
		throw ParameterFile::IoError(
				"Parameter file \"" + fileName + "\": "
				"does not exist.");
	std::ifstream file;
	file.open(fileName.c_str());
	if (file.fail()) {
		throw ParameterFile::IoError(
				"Parameter file \"" + fileName + "\" could not be opened. "
				"(file.fail())");
	} else {
		clear();
		fromStream(file);
		file.close();
		return true;
	}
}

std::vector<std::string> ParameterFile::getKeyList(std::string beginsWith) const {
	std::vector<std::string> result;
	_toLower(beginsWith);

	// use parameterLines to preserve order and case
	std::vector<std::string>::const_iterator iter;
	std::string key, keyL;
	for (iter=_parameterLines.begin(); iter!=_parameterLines.end(); iter++) {
		if (iter->empty()) {
			continue;
		}
		key = keyL = *iter;
		_toLower(keyL);
		if (beginsWith.empty() ||
				((keyL.length() >= beginsWith.length()) &&
					(keyL.substr(0,beginsWith.length()) == beginsWith))) {
			result.push_back(key);
		}
	}
	return result;
}

std::vector<std::string> ParameterFile::getEveryParameter(
		const std::string & param) const {
	std::vector<std::string> result;

	// use parameterLines to preserve order
	std::vector<std::string>::const_iterator it;

	for (it = _parameterLines.begin(); it != _parameterLines.end(); it++) {
		std::string key = *it;
		size_t pos = key.find_first_of('.');
		if (key.length() >= pos + param.length() && (key.substr(pos + 1,
				param.length()) == param))
			result.push_back(key);
	}
	return result;
}

void ParameterFile::showSetParams() const {
	std::map<std::string, std::string>::const_iterator i = _setParams.begin();
	int p = 0;
	unsigned int maxSize = 0;
	while (i != _setParams.end()) {
		std::string key = i->first;
		if (key.size() > maxSize)
			maxSize = (unsigned int) key.size();
		++i;
	}

	i = _setParams.begin();
	while (i != _setParams.end()) {
		std::string key = i->first;
		std::string value = i->second;
		std::string fillUp;
		if ((p++ % 2) != 0) {
			fillUp = std::string(maxSize - key.size(), ' ');
			sout << key << " " << fillUp << "  " << value << std::endl;
		} else {
			fillUp = std::string(maxSize - key.size(), '-');
			sout << key << " " << fillUp << "> " << value << std::endl;
		}
		++i;
	}
}

void ParameterFile::toStream(std::ostream& strm) const {
	std::vector<std::string>::const_iterator i;
	for (i = _parameterLines.begin(); i != _parameterLines.end(); i++) {
		std::string key = *i;
		if (key != "") {
			std::string value = get<std::string> (key);
			strm << key << "\t\t" << value << std::endl;
		} else
			strm << std::endl;
	}
}

void ParameterFile::fromStream(std::istream& strm) {
	std::set<std::string> processedKeys;
	while (strm.good()) {
		std::string key, line, value;
		if (strm.peek() == '\n')
			_parameterLines.push_back(""); //preserve empty lines
		strm >> key;
		key = StringTool::trim(key);
		if (!key.empty()) {
			if (key.substr(0, 1) != "#") {
				value = "";
				bool cont = true;
				while (cont) {
					// allow empty strings as parameter value
					getline(strm, line);
					line = StringTool::trim(line);
					if (line.empty()) {
						// stop continuing
						cont = false;
					}
					else {
						// strip comments (after input)
						size_t cPos = line.find("#");
						line = StringTool::trim(line.substr(0, cPos));
						// handle line breaks (ending with '\')
						const char& test = line.at(line.length()-1);
						if ((cont = (test == '\\'))) {
							line = line.substr(0, line.length()-1);
						}
						value += line;
					}
				}
				if (processedKeys.find(key) != processedKeys.end()) {
					sout << "(WW) fromStream: "
						<< "Duplicate Key in Parameter File: "
						<< key << std::endl;
				}
				_set(key,value);
				processedKeys.insert(key);
			} else {
				//ignore whole line (when comment line)
				getline(strm, line);
			}
		}
	}
}

void ParameterFile::erase(std::string parameter) {
	if (!isSet(parameter))
		throw ParameterFile::Unset("Parameter " + parameter + " not set");

	// Add delete information into log list
	_setParams["[del] " + parameter] = get<std::string>(parameter);

	// delete parameter from parameter store and parameterLines
	_toLower(parameter);
	_params.erase(parameter);

	std::vector<std::string>::iterator pos = _parameterLines.begin();
	std::string cur;
	while (pos != _parameterLines.end()) {
		cur = *pos;
		_toLower(cur);
		if (cur == parameter)
			break;
		pos++;
	}
	_parameterLines.erase(pos);
}

