/*  Copyright (C) 2012 Gerald Mwangi
    Copyright (C) 2012 Michael Baron

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
/** \file ForStatement.cpp
 *  Implementation of parameter class ForStatement.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 08.11.2012
 */

#include <charon-core/ParameteredObject.hxx>
#include "../include/charon-core/ForStatement.h"

#define INVALID_VALUE -1000

ForStatement::ForStatement(const std::string& name) :
		ParameteredObject(
			"ForStatement", name,
			"<h2>Statement module to be used in counting loops</h2><br>"
			"Statement module to be used in loops. This plugin is intended "
			"for loops (e.g. WhileGroup). It interfaces the loop depending "
			"on its counter value."
		)
{
	ParameteredObject::_setTags("charon-core") ;
	
	ParameteredObject::_addInputSlot(
		initialValue, "initialValue", "initial value", "double" );
	ParameteredObject::_addInputSlot(
		incrementValue, "incrementValue", "increment value", "double" );
	ParameteredObject::_addInputSlot(
		finalValue, "finalValue", "final value", "double" );
	ParameteredObject::_addOutputSlot(
		currentValue, "currentValue",
		"ForStatement to interface to the external loop",
		"int");

	_currentValue = int(INVALID_VALUE);
}

void ForStatement::execute() {
	_initialValue = initialValue();
	_incrementValue = incrementValue();
	_finalValue = finalValue();

	if (_currentValue == INVALID_VALUE) _currentValue = _initialValue;
	else _currentValue += _incrementValue;
	currentValue() = _currentValue;
}

// the following functions are needed
// for class ForStatement to work as a charon plugin.
extern "C" forstatement_DECLDIR ParameteredObject*
		create(const std::string& name, ParameteredObject::template_type) {
	return new ForStatement(name);
}

extern "C" forstatement_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" forstatement_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}

bool ForStatement::operator ()() const
{
	return (_currentValue < _finalValue);
}

