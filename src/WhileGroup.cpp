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
/** \file WhileGroup.cpp
 *  Implementation of parameter class WhileGroup.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 16.02.2012
 */

#include <charon-core/ParameteredObject.hxx>
#include "../include/charon-core/WhileGroup.h"

WhileGroup::WhileGroup(const std::string& name) :
                ParameteredGroupObject(
			"WhileGroup", name,
			"<h2>Executes the group as long as a given statement is "
			"true</h2><br>"
			"Executes the group as long as a given statement is true"
		)
{
    _addInputSlot(statement,"Statement","Statement for this whilegroup","bool");
}

void WhileGroup::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

        while(statement)
        {
            _pluginMan->executeWorkflow();
            _pluginMan->resetExecuted();
        }
}

// the following functions are needed
// for class WhileGroup to work as a charon plugin.
extern "C" whilegroup_DECLDIR ParameteredObject*
		create(const std::string& name, template_type) {
	return new WhileGroup(name);
}

extern "C" whilegroup_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" whilegroup_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}

WhileGroupStatement::WhileGroupStatement(InputSlot<bool> * pstatement)
    :ParameteredObject("WhileGroupStatement","WhileGroupStatement","Statement used in a Whilegroup")
{
    _whilestatement=pstatement;
    _addInputSlot(statement,"Statement","Statement for this whilegroup","bool");
}

void WhileGroupStatement::execute()
{
    PARAMETEREDOBJECT_AVOID_REEXECUTION;
    ParameteredObject::execute();
    (*_whilestatement)=statement();
}

void WhileGroup::initializeGroup()
{
    _pluginMan->insertInstance(new WhileGroupStatement(&statement));
}
