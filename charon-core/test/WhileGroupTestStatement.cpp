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
/** \file WhileGroupTestStatement.cpp
 *  Implementation of parameter class WhileGroupTestStatement.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 27.11.2012
 */

#include <charon-core/ParameteredObject.hxx>
#include "WhileGroupTestStatement.h"

WhileGroupTestStatement::WhileGroupTestStatement(const std::string& name) :
		ParameteredObject(
			"WhileGroupTestStatement", name,
			"<h2>Test statement for whilegroup</h2><br>"
			"Test statement for whilegroup"
		)
{

	ParameteredObject::_addParameter< int >(
		num_iter, "num_iter",
		"num_iter",
		10, "int");
	ParameteredObject::_addOutputSlot(iteration,"iteration","current iteration","int");
	_iter=0;

}

void WhileGroupTestStatement::execute() {
	iteration=_iter;
	_iter++;
}

// the following functions are needed
// for class WhileGroupTestStatement to work as a charon plugin.
extern "C" whilegroupteststatement_DECLDIR ParameteredObject*
		create(const std::string& name, ParameteredObject::template_type) {
	return new WhileGroupTestStatement(name);
}

extern "C" whilegroupteststatement_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" whilegroupteststatement_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}

bool WhileGroupTestStatement::operator ()() const
{
	return _iter<num_iter();
}
