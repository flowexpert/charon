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
#include <set>


// ====================   class AbstractParameter   =========================

inline AbstractParameter::AbstractParameter() :
    _parent(0) {
}

inline void AbstractParameter::init(ParameteredObject* parent,
        const std::string& name) {
    if(_parent)
        throw std::string("Parameter already assigned");
    if(!parent)
        throw std::string("Setting invalid parent object!");
    if(!name.length())
        throw std::string("Invalid name: length 0");

    _parent = parent;
    _name   = name;
}

inline AbstractParameter::~AbstractParameter() {
}

inline ParameteredObject& AbstractParameter::getParent() {
    return *_parent;
}

inline const ParameteredObject& AbstractParameter::getParent() const {
    return *_parent;
}

inline std::string AbstractParameter::getName() const {
    return _name;
}

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
    return TypeDetector::instance().type(typeid(T).name());
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
    if (_value != _defaultValue) {
        pf.set<T>(_parent->getName() + "." + _name, _value);
    }
}

template <typename T>
void Parameter<T>::load(const ParameterFile& pf) {
    if (pf.isSet(_parent->getName() + "." + _name))
        _value = pf.get<T>(_parent->getName() + "." + _name);
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
    return TypeDetector::instance().type(typeid(T).name()) + " list";
}

template <typename T>
void ParameterList<T>::save(ParameterFile& pf) const {
    if (_value == _defaultValue) {
        if (pf.isSet(_parent->getName() + "." + _name))
            pf.erase(_parent->getName() + "." + _name);
    }
    else
        pf.set<T>(_parent->getName() + "." + _name, _value);
}

template <typename T>
void ParameterList<T>::load(const ParameterFile& pf) {
    if(pf.isSet(_parent->getName() + "." + _name))
        _value = pf.getList<T>(_parent->getName() + "." + _name);
    else
        _value = _defaultValue;
}

template <typename T>
void ParameterList<T>::setDefault(const std::string& defVal) {
    _defaultValue = defVal;
}

template <typename T>
std::string ParameterList<T>::getDefaultString() {
    return _defaultValue;
}

#endif // _Parameter_HXX_
