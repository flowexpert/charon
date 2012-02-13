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
/** @file   groupVirtualSlot.cpp
 *  @brief  Unit tests VirtualSlot and ParameteredGroupObject class.
 *  @date   06.02.2012
 *  @author <a href="mailto:gerald.mwangi@gmx.de">Gerald Mwangi</a>
 */

#include "../include/charon-core/IfGroup.h"
#include "../include/charon-core/ParameteredGroupObject.h"
#include "../include/charon-core/DataManagerParameterFile.hxx"


/// sample ParameteredObject class.
class Reader : public ParameteredObject {
public:
	/// sample integer input slot
	InputSlot<int>   in1;
	/// sample float input slot
	InputSlot<float> in2;

	/// create a new sample object
	/// @param name             Object name
	Reader(const std::string& name = "") :
			ParameteredObject("Reader", name, "read slot data")
	{
		_addInputSlot (in1,  "in1",  "integer input slot");
		_addInputSlot (in2,  "in2",  "float input slot");
	}

protected:
	virtual void execute() {
		sout << "(II) \tRead slot data: "
			 << in1() << ", " << in2() << std::endl;
	}
};

/// sample ParameteredObject class.
class Outputgen : public ParameteredObject {

public:
	/// sample integer output slot
	OutputSlot<int>    out1;
	/// sample float output slot
	OutputSlot<float>  out2;

	/// create a new sample object
	/// @param name             Object name
	Outputgen(const std::string& name = "") :
			ParameteredObject("outputgen", name,
				"class to generate int and float output"),
			out1(10),   // set initial values
			out2(5.5f)  // ------- " --------
	{
		// slots
		_addOutputSlot(out1, "out1", "integer output slot");
		_addOutputSlot(out2, "out2", "float output slot");
	}

protected:
	virtual void execute() {
	}
};


/// sample ParameteredGroupObject class
class TestGroup: public ParameteredGroupObject
{
public:
    TestGroup()
	: ParameteredGroupObject("TestGroup","TestGroupName","TestGroupDoc")
    {
	workFlowFile="TestGroup.wrp";
    }
    void initializeGroup()
    {


	setNumberOfInputSlots(2);
	Reader* rd=new Reader("Reader");
	Slot* out1=dynamic_cast<Slot*>(getInputSlot(0).second);
	Slot* out2=dynamic_cast<Slot*>(getInputSlot(1).second);
	_pluginMan->insertInstance(rd);
	//_pluginMan->connect(out1->getParent().getName()+"."+out1->getName(),rd->getName()+"."+rd->in1.getName());
	_pluginMan->connect(out1,&(rd->in1));
	_pluginMan->connect(out2,&(rd->in2));


    }



};

int main()
{
    ParameterFile file;
    file.save("TestGroup.wrp");

    Outputgen generator("Outgen");
    generator.out1.setCacheType(Slot::CACHE_MANAGED);
    std::vector<std::string> paths;
    paths.push_back("/home/gmwangi/Programming/workspace-qtcreator/Charon/sources/supernodes/install/lib/charon-plugins");
    TestGroup group;
    group.pluginPaths=paths;
    //generator.initialize();
    group.initialize();

    PluginManager* man=new PluginManager(paths);
    man->insertInstance(&group);
    man->insertInstance(&generator);
    Slot* in=dynamic_cast<Slot*>(group.getInputSlot(0).first);
    Slot* in2=dynamic_cast<Slot*>(group.getInputSlot(1).first);
    man->connect(&(generator.out1),in);
    man->connect(&(generator.out2),in2);
    sout<<"Number of input in Testgroup "<<group.getNumberOfInputSlots()<<std::endl;

    man->runWorkflow();

}
