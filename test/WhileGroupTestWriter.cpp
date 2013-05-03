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
/** \file WhileGroupTestWriter.cpp
 *  Implementation of parameter class WhileGroupTestWriter.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 27.11.2012
 */

#include <charon-core/ParameteredObject.hxx>
#include "WhileGroupTestWriter.h"

WhileGroupTestWriter::WhileGroupTestWriter(const std::string& name) :
		ParameteredObject(
			"WhileGroupTestWriter", name,
			"<h2>Test module output module for whilegroup</h2><br>"
			"Test module output module for whilegroup"
		)
{

	ParameteredObject::_addOutputSlot(
		out1, "out1",
		"out1",
		"int");
	ParameteredObject::_addOutputSlot(
		out2, "out2",
		"out2",
		"float");

	ParameteredObject::_addParameter(
		inside_group, "inside_group",
		"inside_group",
		"bool");


}

void WhileGroupTestWriter::execute() {
	if(inside_group)
	{
		out1=20;
		out2=0.33f;
	}
	else
	{
		out1=10;
		out2=5.5f;
	}

}

// the following functions are needed
// for class WhileGroupTestWriter to work as a charon plugin.
extern "C" whilegrouptestwriter_DECLDIR ParameteredObject*
		create(const std::string& name, ParameteredObject::template_type) {
	return new WhileGroupTestWriter(name);
}

extern "C" whilegrouptestwriter_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" whilegrouptestwriter_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}
