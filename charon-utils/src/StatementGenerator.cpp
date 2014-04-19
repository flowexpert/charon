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
/** \file StatementGenerator.cpp
 *  Implementation of parameter class StatementGenerator.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 20.01.2012
 */

#include <charon-core/ParameteredObject.hxx>
#include "../include/charon-utils/StatementGenerator.h"

StatementGenerator::StatementGenerator(const std::string& name) :
		ParameteredObject(
			"StatementGenerator", name,
			"<h2>Takes a bool as parameter and outputs it to a slot</h2><br>"
			"Takes a bool as parameter and outputs it to a slot"
		)
{
	ParameteredObject::_setTags("charon-utils") ;

	ParameteredObject::_addOutputSlot(
		statement, "statement", "statement");

	ParameteredObject::_addParameter< bool >(
		statementpar, "statementpar", "statement", false);
}

void StatementGenerator::execute() {
	statement=statementpar;
}

// the following functions are needed
// for class StatementGenerator to work as a charon plugin.
extern "C" statementgenerator_DECLDIR ParameteredObject* create(
		const std::string& name, ParameteredObject::template_type) {
	return new StatementGenerator(name);
}

extern "C" statementgenerator_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" statementgenerator_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}
