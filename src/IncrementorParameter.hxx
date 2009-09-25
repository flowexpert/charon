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
	this->_addParameter(stepSize, "stepSize", "stepsize to increment parameter", "T");
	this->_addParameter(current, "current", "current incremented Parameter",
			"T");
	this->_addOutputSlot(out, "out", "list of Parameters for Incrementor",
			"IncrementorParameter<T>*");
}

#endif /* INCREMENTORPARAMETER_HXX_ */
