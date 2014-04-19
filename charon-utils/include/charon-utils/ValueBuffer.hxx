/*  Copyright (C) 2012 Gerald Mwangi

	This file is part of Charon.

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
/** \file ValueBuffer.hxx
 *  Implementation of the parameter class ValueBuffer.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 27.03.2012
 */

#ifndef _VALUEBUFFER_HXX_
#define _VALUEBUFFER_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "ValueBuffer.h"

template <typename T>
ValueBuffer<T>::ValueBuffer(const std::string& name) :
		TemplatedParameteredObject<T>(
			"ValueBuffer", name,
			"<h2>Buffers a value for use in Whilegroups</h2><br>"
			"Buffers a value for use in Whilegroups"
		)
{
	ParameteredObject::_setTags("charon-utils") ;

	ParameteredObject::_addInputSlot(
		value, "value",
		"value to be buffered",
		"T");

	ParameteredObject::_addOutputSlot(
		buffered_value, "buffered_value",
		"buffered value",
		"T");

}

template <typename T>
void ValueBuffer<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	buffered_value=value;
}

#endif /* _VALUEBUFFER_HXX_ */
