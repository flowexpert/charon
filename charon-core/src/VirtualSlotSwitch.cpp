/*  Copyright (C) 2013 Gerald Mwangi

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
/** \file VirtualSlotSwitch.cpp
 *  Implementation of parameter class VirtualSlotSwitch.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 19.09.2013
 */

#include <charon-core/ParameteredObject.hxx>
#include <charon-core/VirtualSlotSwitch.h>

VirtualSlotSwitch::VirtualSlotSwitch(const std::string& name) :
		ParameteredObject(
			"VirtualSlotSwitch", name,
			"<h2>Connects its output to one of its inputs according to a "
			"statement or inputnumber</h2><br>"
			"Connects its output to one of its inputs according to a "
			"statement or inputnumber."
		),
		statement(true,false),
    inputnumber(true,false),output(0)
{

	ParameteredObject::_addInputSlot(
		statement, "statement",
		"statement to switch between input 0 and input 1",
		"bool");
	ParameteredObject::_addInputSlot(
		inputnumber, "inputnumber",
		"connects output to according input",
		"int");

	ParameteredObject::_addOutputSlot(
        output, output.getName(),
		"output connected to one of the inputs",
        output.getType());

	ParameteredObject::_addParameter< int >(
		number_of_inputs, "number_of_inputs",
		"number_of_inputs",
		2, "int");
    ParameteredObject::_addParameter< bool >(
        boolean_inputnumber, "boolean_inputnumber",
        "cast inputnumber to bool",
        0, "bool");
    _setDynamic(true);
    oldselection=0;



}

VirtualSlotSwitch::~VirtualSlotSwitch()
{
    setNumberOfVirtualSlots(0);
}

void VirtualSlotSwitch::execute() {

    if(!statement.connected()&&!inputnumber.connected())
        raise("Either statement or inputnumber must be connected!");
    int selected_in=-1;
    if(statement.connected())
        selected_in=statement();
    if(inputnumber.connected())
    {

        selected_in=inputnumber();
        if(boolean_inputnumber())
            if(selected_in)
                selected_in=1;
    }
    if(selected_in>=_virtualInputSlots.size()||selected_in<0)
    {
        std::stringstream msg;
        msg<<"Out of range: Selected input: "<<selected_in<<". Possible range [0,"<<_virtualInputSlots.size()-1<<"]"<<std::endl;
        raise(msg.str());

    }
    if(oldselection!=selected_in)
    {
        sout<<"Switching to slot "<<selected_in<<std::endl;
        output.setVirtualPartnerSlot(_virtualInputSlots[selected_in]);

        _virtualInputSlots[oldselection]->setVirtualPartnerSlot(_virtualOutputSlots[oldselection]);
        oldselection=selected_in;
    }


}

void VirtualSlotSwitch::prepareDynamicInterface(const ParameterFile &file)
{
    number_of_inputs.load(file);

    if(number_of_inputs<2)
        raise("Number of inputs must be at least 2");
    setNumberOfVirtualSlots(number_of_inputs);
    output.setVirtualPartnerSlot(_virtualInputSlots[0]);

}

void VirtualSlotSwitch::setNumberOfVirtualSlots(int num)
{

    //Temporary list of virtual slots
    std::vector<VirtualOutputSlot*> tempOutputSlots;
    std::vector<VirtualInputSlot*> tempInputSlots;

    //Keep smallest common number of slots
    int nslots=std::min(num,(int)_virtualOutputSlots.size());
    for(int i=0;i<nslots;i++)
    {
        tempInputSlots.push_back(_virtualInputSlots[i]);
        tempOutputSlots.push_back(_virtualOutputSlots[i]);
    }

    //Add more slots if needed
    for(int i=nslots;i<num;i++)
    {
        VirtualOutputSlot* out=new VirtualOutputSlot(i);
        VirtualInputSlot* in= new VirtualInputSlot(i);
        out->setVirtualPartnerSlot(in);
        tempInputSlots.push_back(in);
        tempOutputSlots.push_back(out);
    }

    //Remove slots if _virtualOutputSlots.size()> num
    for(size_t i=num;i<_virtualOutputSlots.size();i++) {
        _removeInputSlot(_virtualInputSlots[i]->getName());

        ((VirtualOutputSlot*)_virtualOutputSlots[i])->disconnect();
        ((VirtualInputSlot*)_virtualInputSlots[i])->disconnect();
        delete _virtualOutputSlots[i];
        delete _virtualInputSlots[i];
    }

    //Remember new slotlist
    _virtualInputSlots=tempInputSlots;
    _virtualOutputSlots=tempOutputSlots;

    //Eventually add new slots to internal interface
    for(int i=nslots;i<num;i++)
    {
        _addInputSlot(*((Slot*)_virtualInputSlots[i]),
            _virtualInputSlots[i]->getName(),
            "Virtual Slot",_virtualInputSlots[i]->getType());
    }




}

// the following functions are needed
// for class VirtualSlotSwitch to work as a charon plugin.
extern "C" virtualslotswitch_DECLDIR ParameteredObject*
		create(const std::string& name, ParameteredObject::template_type) {
	return new VirtualSlotSwitch(name);
}

extern "C" virtualslotswitch_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" virtualslotswitch_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}
