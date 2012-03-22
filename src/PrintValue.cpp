/*  Copyright (C) 2012 Gerald Mwangi

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
/** \file PrintValue.cpp
 *  Implementation of parameter class PrintValue.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 22.02.2012
 */

#include <charon-core/ParameteredObject.hxx>
#include "../include/charon-utils/PrintValue.h"

PrintValue::PrintValue(const std::string& name) :
		ParameteredObject(
			"PrintValue", name,
			"<h2>Prints a value to stdout</h2><br>Prints a value to stdout"
		)
{

	ParameteredObject::_addInputSlot(
		value, "value",
		"value",
		"double");

}

void PrintValue::execute() {
	sout<<"######################################################"<<std::endl;
	sout<<getClassName()<<" "<<getName()<<": "<<value<<std::endl;
	sout<<"######################################################"<<std::endl;
}

// the following functions are needed
// for class PrintValue to work as a charon plugin.
extern "C" printvalue_DECLDIR ParameteredObject* create(
		const std::string& name, ParameteredObject::template_type) {
	return new PrintValue(name);
}

extern "C" printvalue_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" printvalue_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}
