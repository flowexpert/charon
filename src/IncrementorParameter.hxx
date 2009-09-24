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
/**
 *  @file IncrementorParameter.hxx
 *  @brief implementation of class IncrementorParameter
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 31.08.2009
 */

#ifndef INCREMENTORPARAMETER_HXX_
#define INCREMENTORPARAMETER_HXX_

#include "IncrementorParameter.h"
#include <ParameteredObject.hxx>

template<typename T>
IncrementorParameter<T>::IncrementorParameter(const std::string& name) :
	TemplatedParameteredObject<T> ("incrementorparameter", name,
			"class for Incrementor Parameters")
{
	out = this;
	this->_addParameter(min, "min", "minimum of parameter", "T");
	this->_addParameter(max, "max", "maximum of parameter", "T");
	this->_addParameter(step, "step", "stepsize to increment parameter", "T");
	this->_addParameter(current, "current", "current incremented Parameter",
			"T");
	this->_addOutputSlot(out, "out", "list of Parameters for Incrementor",
			"IncrementorParameter<T>*");
}

template<typename T>
std::string& IncrementorParameter<T>::getName()
{
	return this->parameterName;
}
template<typename T>
void IncrementorParameter<T>::setName(std::string& newName)
{
	this->parameterName = newName;
}

template<typename T>
Parameter<T>& IncrementorParameter<T>::getMin()
{
	return this->min;
}

template<typename T>
void IncrementorParameter<T>::setMin(const T newMin)
{
	this->min = newMin;
}

template<typename T>
Parameter<T>& IncrementorParameter<T>::getMax()
{
	return this->max;
}

template<typename T>
void IncrementorParameter<T>::setMax(const T newMax)
{
	this->max = newMax;
}

template<typename T>
Parameter<T>& IncrementorParameter<T>::getStepSize()
{
	return this->step;
}

template<typename T>
void IncrementorParameter<T>::setStepSize(const T newStep)
{
	this->step = newStep;
}

template<typename T>
Parameter<T>& IncrementorParameter<T>::getCurrent()
{
	return this->current;
}

template<typename T>
void IncrementorParameter<T>::setCurrent(const T newCurrent)
{
	this->current = newCurrent;
}

#endif /* INCREMENTORPARAMETER_HXX_ */
