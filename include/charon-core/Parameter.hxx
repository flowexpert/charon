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
/** @file Parameter.hxx
 *  Implementation of the abstract class AbstractParameter and
 *  template classes Parameter and ParameterList.
 *  @author <a href="mailto:daniel.kondermann@iwr.uni-heidelberg.de">
 *      Daniel Kondermann</a>
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
 *
 *  @date 11.12.2008
 *
 *  This file does contain the implementations of the classes
 *  - AbstractParameter
 *  - Parameter
 *  - ParameterList
 *
 *  In most cases, you want not to include this file but ParameteredObject.hxx
 */

#ifndef _Parameter_HXX_
#define _Parameter_HXX_

#include "ParameteredObject.h"
#include "TypeDetector.h"
#include <sstream>
#include <set>

// =========================   class Parameter   ============================

template <typename T>
Parameter<T>::Parameter(T defaultValue) :
		_defaultValue(defaultValue),
		_value(defaultValue) {
}

template <typename T>
Parameter<T>::~Parameter() {
}

template <typename T>
T& Parameter<T>::operator=(const T& B) {
	_value = B;
	return _value;
}

template <typename T>
void Parameter<T>::setDefault(const T& defVal) {
	_defaultValue = defVal;
}

template <typename T>
Parameter<T>& Parameter<T>::operator=(const Parameter<T>& B) {
	_value = B._value;
	_defaultValue = B._defaultValue;
	return *this;
}

template <typename T>
Parameter<T>::operator T() const {
	return _value;
}

template <typename T>
std::string Parameter<T>::guessType() const {
	return TypeDetector::type(typeid(T).name());
}

template <typename T>
const T& Parameter<T>::operator()() const {
	return _value;
}

template <typename T>
T& Parameter<T>::operator()() {
	return _value;
}

template <typename T>
void Parameter<T>::save(ParameterFile& pf) const {
	std::string paramName = _parent->getName() + "." + _name;
	if(pf.isSet(paramName)) {
		if(pf.get<std::string>(paramName).substr(0,1) == "@")
			throw std::runtime_error(paramName
				+ " : Attempt to overwrite Reference.");
		else if(_value == _defaultValue)
			pf.erase(paramName);
		else
			pf.set<T>(paramName, _value);
	}
	else if (_value != _defaultValue)
		pf.set<T>(paramName, _value);
}

template <typename T>
void Parameter<T>::load(const ParameterFile& pf) {
	std::string paramName = _followLink(pf, _parent->getName() + "." + _name);
	if (pf.isSet(paramName))
		_value = pf.get<T>(paramName);
	else
		_value = _defaultValue;
}

template <typename T>
std::string Parameter<T>::getDefaultString() {
	std::ostringstream strStr;
	strStr << _defaultValue;
	return strStr.str();
}

template <typename T>
void Parameter<T>::intoStream(std::ostream & os) const {
	os << _value;
}

// =====================   class ParameterList   ============================
template <typename T>
ParameterList<T>::ParameterList(std::string defaultValue) {
	_defaultValue = defaultValue;
	ParameterFile pf;
	_value = pf.getList<T>("", defaultValue);
}

template <typename T>
ParameterList<T>::~ParameterList() {
}

template <typename T>
std::vector<T>& ParameterList<T>::operator=(const std::vector<T>& B) {
	_value = B;
	return _value;
}

template <typename T>
ParameterList<T>& ParameterList<T>::operator=(const ParameterList<T>& B) {
	_value = B._value;
	_defaultValue = B._defaultValue;
	return *this;
}

template <typename T>
ParameterList<T>::operator std::vector<T>() const {
	return _value;
}

template <typename T>
std::vector<T>& ParameterList<T>::operator()() {
	return _value;
}

template <typename T>
const std::vector<T>& ParameterList<T>::operator()() const {
	return _value;
}

template <typename T>
std::string ParameterList<T>::guessType() const {
	return TypeDetector::type(typeid(T).name()) + " list";
}

template <typename T>
void ParameterList<T>::save(ParameterFile& pf) const {
	std::stringstream stream;
	std::string paramName = _parent->getName() + "." + _name;
	this->intoStream(stream);

	if(pf.isSet(paramName)) {
		if(pf.get<std::string>(paramName).substr(0,1) == "@")
			throw std::runtime_error(paramName
					+ " : Attempt to overwrite Reference.");
		else if(stream.str() == _defaultValue)
			pf.erase(paramName);
		else
			pf.set<T>(paramName, _value);
	}
	else if (stream.str() != _defaultValue)
		pf.set<T>(paramName, _value);
}

template <typename T>
void ParameterList<T>::load(const ParameterFile& pf) {
	std::string paramName = _followLink(pf, _parent->getName() + "." + _name);
	if(pf.isSet(paramName))
		_value = pf.getList<T>(paramName);
	else {
		ParameterFile temp;
		temp.set("temp", _defaultValue);
		_value = temp.getList<T>("temp");
	}
}

template <typename T>
void ParameterList<T>::setDefault(const std::string& defVal) {
	_defaultValue = defVal;
}

template <typename T>
std::string ParameterList<T>::getDefaultString() {
	return _defaultValue;
}

template <typename T>
void ParameterList<T>::intoStream(std::ostream & os) const {
	if(_value.size()) {
		for(unsigned int i = 0; i < _value.size()-1; i++) {
			os << _value[i] << ";";
		}
		os << _value[_value.size()-1];
	}
}

template <typename T>
std::size_t ParameterList<T>::size() const {
	return _value.size();
}

template <typename T>
const T& ParameterList<T>::operator[](std::size_t pos) const {
	return _value[pos];
}

template <typename T>
T& ParameterList<T>::operator[](std::size_t pos) {
	return _value[pos];
}

#endif // _Parameter_HXX_
