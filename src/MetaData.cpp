/*  Copyright (C) 2009 Jens-Malte Gottfried

	This file is part of Tuchulcha.

	Tuchulcha is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Tuchulcha is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file   MetaData.cpp
 *  @brief  Implementation of class ParameterFileModel.
 *  @date   10.10.2008
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
#include "MetaData.h"
#include "ParameterFile.hxx"
#include <set>
#include <cassert>

/// transform std::string into lowercase
/** \param[in,out] input  string to transform */
inline std::string& _toLower(std::string& input) {
	std::transform(
		input.begin(), input.end(), input.begin(),
		(int(*)(int)) tolower);
	return input;
}

MetaData::MetaData(const std::string& fileName) :
		_data(0),
		_valid(false) {
	if(fileName.size() > 0) {
		_data = new ParameterFile(fileName);
		_check();
	}
}

MetaData::~MetaData() {
	if(_data)
		delete _data;
}

bool MetaData::valid() const {
	return _valid;
}

void MetaData::_check() {
	std::set<std::string> keys; // contains all keys in _metadata
	std::vector<std::string> keyList = _data->getKeyList();
	keys.insert(keyList.begin(), keyList.end());
	std::set<std::string> classes; // contains all classes in _metadata
	std::set<std::string>::const_iterator keyIter;

	// each key needs a separator (classes cannot have a default value)
	for(keyIter = keys.begin(); keyIter != keys.end(); keyIter++) {
		std::string::size_type pos = keyIter->find(".");
		if (pos == std::string::npos)
			throw "Invalid key (separator missing): " + *keyIter;
		classes.insert(keyIter->substr(0, pos));
	}

	// create/update class vector
	_classes = std::vector<std::string>(classes.begin(), classes.end());

	// check required parameters
	std::set<std::string>::const_iterator cIter;
	for(cIter = classes.begin(); cIter != classes.end(); cIter++) {
		// this vector will contain all parameters and slots of this class
		std::vector<std::string> pList;
		std::vector<std::string> temp;

		// add inputs
		if(keys.find(*cIter + ".inputs") == keys.end())
			throw "parameter \"inputs\" missing in class " + *cIter;
		temp = _data->getList<std::string>(*cIter+".inputs");
		pList.insert(pList.end(), temp.begin(), temp.end());

		// add outputs
		if(keys.find(*cIter + ".outputs") == keys.end())
			throw "parameter \"outputs\" missing in class " + *cIter;
		temp = _data->getList<std::string>(*cIter+".outputs");
		pList.insert(pList.end(), temp.begin(), temp.end());

		// add parameters
		if(keys.find(*cIter + ".parameters") == keys.end())
			throw "parameter \"parameters\" missing in class " + *cIter;
		temp = _data->getList<std::string>(*cIter+".parameters");
		pList.insert(pList.end(), temp.begin(), temp.end());

		// check if parameters are given
		// (i.e. "className.parName.type" is set)
		std::vector<std::string>::const_iterator pIter;
		for(pIter = pList.begin(); pIter != pList.end(); pIter++) {
			std::string test ="incrementorparameter.stepsize.type";
			std::string p =*pIter;
			_toLower(p);
			if(keys.find(*cIter + "." + p + ".type") == keys.end())
				throw "parameter " + p
					+ " missing in class " + *cIter;
		}
	}
	_valid = true;
}

const std::vector<std::string>& MetaData::getClasses() const {
	return _classes;
}

std::vector<std::string> MetaData::getOutputs (
										  const std::string& className) const {
	return _data->getList<std::string>(className + ".outputs");
}

std::vector<std::string> MetaData::getInputs
										 (const std::string& className) const {
	return _data->getList<std::string>(className + ".inputs");
}

std::vector<std::string> MetaData::getParameters
										 (const std::string& className) const {
	return _data->getList<std::string>(className + ".parameters");
}

std::string MetaData::getType(std::string parName,
										  const std::string& className) const {
	parName = ParameterFile::parName(parName);
	std::string par = className + "." + parName + ".type";
	std::string result = "string";
	if(_data->isSet(par))
		result = _data->get<std::string>(par);
	return result;
}

std::string MetaData::getDefault(std::string parName,
										  const std::string& className) const {
	parName = ParameterFile::parName(parName);
	std::string par = className + "." + parName;
	std::string result;
	if(_data->isSet(par))
		result = _data->get<std::string>(par);
	return result;
}

std::string MetaData::getDocString(std::string parName,
										  const std::string& className) const {
	parName = ParameterFile::parName(parName);
	std::string par;
	if(parName.size())
		par = className + "." + parName + ".doc";
	else
		par = className + ".doc";

	std::string result;
	if(_data->isSet(par))
		result = _data->get<std::string>(par);
	return result;
}

std::string MetaData::getDocFile(std::string parName,
										  const std::string& className) const {
	parName = ParameterFile::parName(parName);
	std::string par;
	if(parName.size())
		par = className + "." + parName + ".docfile";
	else
		par = className + ".docfile";

	std::string result;
	if(_data->isSet(par))
		result = _data->get<std::string>(par);
	return result;
}

bool MetaData::isParameter(std::string name,
										  const std::string& className) const {
	name = ParameterFile::parName(name);
	std::vector<std::string> paramList = getParameters(className);
	return (std::find(paramList.begin(), paramList.end(), name) !=
					 paramList.end());
}

bool MetaData::isInputSlot(std::string name,
										  const std::string& className) const {
	name = ParameterFile::parName(name);
	_toLower(name);
	std::vector<std::string> inputList = getInputs(className);
	std::transform(
			inputList.begin(), inputList.end(), inputList.begin(),
			_toLower);
	return (std::find(inputList.begin(), inputList.end(), name) !=
					 inputList.end());
}

bool MetaData::isOutputSlot(std::string name,
										  const std::string& className) const {
	name = ParameterFile::parName(name);
	_toLower(name);
	std::vector<std::string> outputList = getOutputs(className);
	std::transform(
			outputList.begin(), outputList.end(), outputList.begin(),
			_toLower);
	return (std::find(outputList.begin(), outputList.end(), name) !=
					 outputList.end());
}

bool MetaData::isOptionalSlot(std::string slotName,
										  const std::string& className) const {
	slotName = ParameterFile::parName(slotName);
	bool slotIsIn = isInputSlot(slotName, className);
	if (!_data->isSet(className + "." + slotName + ".optional"))
		return !slotIsIn;
	return _data->get<bool>(className + "." + slotName + ".optional");
}

bool MetaData::isMultiSlot(std::string slotName,
										  const std::string& className) const {
	slotName = ParameterFile::parName(slotName);
	bool slotIsIn = isInputSlot(slotName, className);
	if (!_data->isSet(className + "." + slotName + ".multi"))
		return !slotIsIn;
	return _data->get<bool>(className + "." + slotName + ".multi");
}
