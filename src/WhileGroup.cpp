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
#include "../include/charon-core/StatementIntf.h"

WhileGroup::WhileGroup(const std::string& name) :
		ParameteredGroupObject(
			"WhileGroup", name,
			"<h2>Executes the group as long as a given statement is "
			"true</h2><br>"
			"Executes the group as long as a given statement is true"
		),statement(true,false)
{
	ParameteredObject::_setTags("charon-core") ;

	_addInputSlot(statement,"Statement","Statement for this whilegroup","bool");
	// statement=true;
	_innerWhilestatement=0;
}

void WhileGroup::executeGroup() {
	ParameteredObject::execute();

	bool extstatement=true;
	if(!_innerWhilestatement)
		return;


	if(statement.connected()) {
		extstatement=extstatement&&statement();
	}
	while((*_innerWhilestatement)&&extstatement) {
		_pluginMan->runWorkflow();
		enableLoopConnections();
		_pluginMan->resetExecuted();
	}
    _innerWhilestatement->reset();
	disableLoopConnections();
}

/// Creates an instance of the plugin
extern "C" whilegroup_DECLDIR ParameteredObject*
		create(const std::string& name, ParameteredObject::template_type) {
	return new WhileGroup(name);
}

/// Deletes an instance of the plugin
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



void WhileGroup::initializeGroup() {

	initializeWhileGroup();

	std::map<std::string, ParameteredObject *> objs=_pluginMan->getObjectList();

	std::map<std::string, ParameteredObject *>::iterator it=objs.begin();
	for(;it!=objs.end();it++)
	{
		ParameteredObject* obj=it->second;
		StatementIntf* curStatement=dynamic_cast<StatementIntf*>(obj);

		if (curStatement) {
			_innerWhilestatement=curStatement;
			break;
		}

	}
	if(!_innerWhilestatement)
	{
		std::stringstream msg;
		msg<<"Could not find a statement module in the workflow: "<<this->workFlowFile();
		raise(msg.str());
	}
}

void WhileGroup::initializeWhileGroup() {

}
