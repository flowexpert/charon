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
/** \file Statement.cpp
 *  Implementation of parameter class Statement.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 29.10.2012
 */

#include <charon-core/ParameteredObject.hxx>
#include "../include/charon-core/Statement.h"

Statement::Statement(const std::string& name) :
		ParameteredObject(
			"Statement", name,
			"<h2>Statement module to be used in loops</h2><br>"
			"Statement module to be used in loops. This plugin is intended "
			"for loops (e.g. WhileGroup). It interfaces the loop  with the "
			"bool statement connected to it."
		)
{
	ParameteredObject::_setTags("charon-core") ;

	ParameteredObject::_addInputSlot(
		statement, "statement",
		"Statement to interface to the external loop",
		"bool");

	_statement=true;
}

void Statement::execute() {
	// your code goes here :-)
	_statement=statement();
}

// the following functions are needed
// for class Statement to work as a charon plugin.
extern "C" statement_DECLDIR ParameteredObject*
		create(const std::string& name, ParameteredObject::template_type) {
	return new Statement(name);
}

extern "C" statement_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" statement_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}

bool Statement::operator ()() const
{
	return _statement;
}
