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
/** \file OutputSlotBundle.cpp
 *  Implementation of parameter class OutputSlotBundle.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 16.10.2012
 */

#include <charon-core/ParameteredObject.hxx>
#include "charon-core/OutputSlotBundle.h"

OutputSlotBundle::OutputSlotBundle(const std::string& name) :
		SlotBundle(
			"OutputSlotBundle", name,
			"<h2>Output slots to be used with groups</h2><br>"
			"Output slots to be used with groups. This plugin can be loaded "
			"into a group workflow file. It creates an output interface for "
			"the group object in the parent workflow. The number of slots "
			"(termed virtual slots) is defined by a parameter and each "
			"virtual slot can be connected to any interior plugin (or slot "
			"thereof) of any type. Once connected, the virtual slot takes on "
			"the type of the connected interior slot. In the parent worfklow "
			"the virtual slot then behaves exactly like the interior slot "
			"(same type, same name)."
		)
{
	ParameteredObject::_setTags("charon-core") ;
}

/// Creates an instance of the plugin
extern "C" outputslotbundle_DECLDIR ParameteredObject*
		create(const std::string& name, ParameteredObject::template_type) {
	return new OutputSlotBundle(name);
}

/// Deletes an instance of the plugin
extern "C" outputslotbundle_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" outputslotbundle_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}

std::vector<VirtualOutputSlot *> & OutputSlotBundle::getSlotVector()
{
	return _virtualOutputSlots;
}

std::vector<VirtualInputSlot *> & OutputSlotBundle::getInternalSlotVector()
{
	return _virtualInputSlots;
}

void OutputSlotBundle::_addSlot(int i)
{
	_addInputSlot(*((Slot*)_virtualInputSlots[i]),
		_virtualInputSlots[i]->getName(),
		"Virtual Slot",_virtualInputSlots[i]->getType());
}

void OutputSlotBundle::_removeSlot(int i)
{
	_removeInputSlot(_virtualInputSlots[i]->getName());
}
