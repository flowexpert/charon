/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

    This file is part of Charon.

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
/** \file Value.hxx
 *  Implementation of the parameter class Value.
 *  \author Michael Baron
 *  \date 06.08.2012
 */

#ifndef _VALUE_HXX_
#define _VALUE_HXX_

#include "Value.h"

template <typename T>
Value<T>::Value(const std::string& name) :
		TemplatedParameteredObject<T>("Value", name,
			"Module returning a value. <br>"
			"This module returns the value set by its parameter. "
			"This can be useful, to extract a single pyramid or "
			"scale-space level. "
		)
{
	ParameteredObject::_addOutputSlot(out, "out",
		"output slot for value");
	ParameteredObject::_addParameter(value, "value", "value to be returned by output slot", "uint");
}

template <typename T>
void Value<T>::execute() {
	out() = value();
}

#endif /* _VALUE_HXX_ */

