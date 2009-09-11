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

#include "ParameterFile.hxx"

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
	_toLower(parameter);
	if (isSet(parameter))
		_setParams["[mod] " + parameter] = value;
	else {
		_setParams["      " + parameter] = value;
		_parameterLines.push_back(parameter);
	}
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
		throw "ERROR: Parameter file '" + fileName
				+ "' could not be saved. IO error.";
	} else {
		toStream(file);
		file.close();
		return true;
	}
}

bool ParameterFile::load(std::string fileName) {
	if (!FileTool::exists(fileName))
		throw "File " + fileName + " does not exist.";
	std::ifstream file;
	file.open(fileName.c_str());
	if (file.bad()) {
		throw "Parameter file '" + fileName
				+ "' could not be opened. IO error. (file.bad())";
	} else {
		clear();
		fromStream(file);
		file.close();
		return true;
	}
}

std::vector<std::string> ParameterFile::getKeyList(std::string beginsWith) const {
	std::vector<std::string> result;

	// use parameterLines to preserve order
	std::vector<std::string>::const_iterator i = _parameterLines.begin();
	while (i != _parameterLines.end()) {
		std::string key = *i;
		if (key.length() && ((!beginsWith.length()) || key.substr(0,
				beginsWith.length()) == beginsWith))
			result.push_back(key);
		++i;
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
		unsigned int pos = key.find_first_of('.');
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
	while (!strm.eof()) {
		std::string key, value, line;
		if (strm.peek() == '\n')
			_parameterLines.push_back(""); //preserve empty lines
		strm >> key;
		key = StringTool::trim(key);
		if (key != "") {
			if (key.substr(0, 1) != "#") {
				line = "";
				bool cont = true;
				do {
					//allow spaces and empty strings as parameter value
					getline(strm, value, '\n');
					if (StringTool::trim(value) != "") {
						std::string test = value.substr(value.length() - 2,
								value.length() - 1);
						//allow to continue line with this char sequence
						cont = (test == " \\");
						value = StringTool::trim(value);
						if (cont && value.size() > 2)
							value = value.substr(0, value.length() - 2);
						else if (cont && value.size() <= 2)
							value = "";
						for (unsigned int i = 0; i < value.size(); ++i) {
							//allow for comments after input
							if (value[i] == '#') {
								//conserve space before #
								if (i && cont && value[i - 1] == ' ')
									value
											= StringTool::trim(value.substr(0,
													i)) + " ";
								else
									value
											= StringTool::trim(value.substr(0,
													i));
								break;
							}
						}
						line += value;
					} else
						cont = false;
				} while (cont);

				_toLower(key);
				_params[key] = line;
				_parameterLines.push_back(key);
			} else {
				//ignore whole line (when comment line)
				getline(strm, value, '\n');
			}
		}
	}
}

void ParameterFile::erase(std::string parameter) {
	_toLower(parameter);
	if (!isSet(parameter)) {
		throw "In erase(" + parameter + "): Parameter not set!";
	}

	// Add delete information into log list
	_setParams["[del] " + parameter] = get<std::string> (parameter, "");

	// delete parameter from parameter store and parameterLines
	_params.erase(parameter);

	std::vector<std::string>::iterator pos = _parameterLines.begin();
	while (pos != _parameterLines.end()) {
		if (*pos == parameter)
			break;
		pos++;
	}
	_parameterLines.erase(pos);
}

