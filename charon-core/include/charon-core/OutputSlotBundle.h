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
/** \file OutputSlotBundle.h
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 16.10.2012
 *  Declaraion of parameter class OutputSlotBundle.
 */
#ifndef _OUTPUTSLOTBUNDLE_H_
#define _OUTPUTSLOTBUNDLE_H_

#ifdef _MSC_VER
#ifdef outputslotbundle_EXPORTS
/// Visual C++ specific code
#define outputslotbundle_DECLDIR __declspec(dllexport)
#else
#define outputslotbundle_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define outputslotbundle_DECLDIR
#endif

#include "SlotBundle.h"

/// Output slots to be used with groups
/** Output slots to be used with groups. This plugin can be loaded into a
 *  group workflow file. It creates an output interface for the group object
 *  in the parent workflow. The number of slots (termed virtual slots) is
 *  defined by a parameter and each virtual slot can be connected to any
 *  interior plugin (or slot thereof) of any type. Once connected, the virtual
 *  slot takes on the type of the connected interior slot. In the parent
 *  worfklow the virtual slot then behaves exactly like the interior slot
 *  (same type, same name).
 */
class outputslotbundle_DECLDIR OutputSlotBundle : public SlotBundle, public OutputSlotBundleIntf {
public:
	/// default constructor
	/// \param name             instance name
	OutputSlotBundle(const std::string& name = "");


	std::vector<VirtualOutputSlot*>& getSlotVector();
	std::vector<VirtualInputSlot*>& getInternalSlotVector();



protected:
	/// Update object.
	 void _addSlot(int i);
	 void _removeSlot(int i);
};

#endif /* _OUTPUTSLOTBUNDLE_H_ */
