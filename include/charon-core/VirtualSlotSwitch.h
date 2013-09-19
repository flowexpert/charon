/*  Copyright (C) 2013 Gerald Mwangi

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
/** \file VirtualSlotSwitch.h
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 19.09.2013
 *  Declaraion of parameter class VirtualSlotSwitch.
 */
#ifndef _VIRTUALSLOTSWITCH_H_
#define _VIRTUALSLOTSWITCH_H_

#ifdef _MSC_VER
#ifdef virtualslotswitch_EXPORTS
/// Visual C++ specific code
#define virtualslotswitch_DECLDIR __declspec(dllexport)
#else
#define virtualslotswitch_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define virtualslotswitch_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>

/// Connects its output to one of its inputs according to a statement or
/// inputnumber
/** Connects its output to one of its inputs according to a statement or
 *  inputnumber.
 */
class virtualslotswitch_DECLDIR VirtualSlotSwitch : public ParameteredObject {
public:
	/// default constructor
	/// \param name             instance name
	VirtualSlotSwitch(const std::string& name = "");
    ~VirtualSlotSwitch();
	/// statement to switch between input 0 and input 1
	InputSlot< bool > statement;
	/// connects output to according input
	InputSlot< int > inputnumber;

	/// output connected to one of the inputs
    VirtualOutputSlot  output;

	/// number_of_inputs
	Parameter< int > number_of_inputs;


protected:
	/// Update object.
	virtual void execute();
    virtual void prepareDynamicInterface(const ParameterFile &file);
    std::vector< VirtualInputSlot* > _virtualInputSlots;
    std::vector<VirtualOutputSlot*> _virtualOutputSlots;
    int oldselection;

    void setNumberOfVirtualSlots(int num);
};

#endif /* _VIRTUALSLOTSWITCH_H_ */
