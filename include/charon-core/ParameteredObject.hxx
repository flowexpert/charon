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
/** @file ParameteredObject.hxx
 *  Convenience file to iclude all ParameteredObject dependencies and
 *  Template functions.
 *  Include this file when you want to derive from ParameteredObject.
 *
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
 *
 *  @date 11.12.2008
 *
 */

#ifndef _PARAMETERED_OBJECT_HXX_
#define _PARAMETERED_OBJECT_HXX_

#include "ParameteredObject.h"
#include "ParameterFile.hxx"
#include "Parameter.hxx"
#include "Slots.hxx"

template<class T>
void ParameteredObject::_addParameter(Parameter<T>& param,
		const std::string& name, const std::string& doc, const T& defVal,
		const std::string& type) {
	param.setDefault(defVal);
	param() = defVal;
	ParameteredObject::_addParameter(param, name, doc, type.c_str());
}

template <typename T>
void ParameteredObject::setParameter(std::string name, T value) {
	if(_parameters.find(name) != _parameters.end()) {
		Parameter<T>& p = dynamic_cast<Parameter<T>&>(*_parameters[name]);
		p.operator T() = value;
	}
	else
		throw std::invalid_argument("parameter not found");
}

template<typename T>
TemplatedParameteredObject<T>::TemplatedParameteredObject(
		const std::string& className, const std::string& name,
		const std::string& doc) :
	ParameteredObject(className, name, doc) {
		_addSomething(
				"parameters", "templatetype", "template type",
				"{int;float;double}", "double");
}

template<typename T>
const std::string TemplatedParameteredObject<T>::getTemplateType() const {
	return TypeDetector::instance().type(typeid(T).name());
}

#endif // _PARAMETERED_OBJECT_HXX_
