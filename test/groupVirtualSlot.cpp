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
#include "../include/charon-core/ExceptionHandler.h"


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
class TestGroupReader: public ParameteredGroupObject
{
public:
    TestGroupReader()
	: ParameteredGroupObject("TestGroupReader","TestGroupReaderName","TestGroupReaderDoc")
    {
	workFlowFile="TestGroupReader.wrp";
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
class TestGroupWriter: public ParameteredGroupObject
{
public:
    TestGroupWriter()
	: ParameteredGroupObject("TestGroupWriter","TestGroupWriterName","TestGroupWriterDoc")
    {
	workFlowFile="TestGroupWriter.wrp";
    }
    void initializeGroup()
    {


	setNumberOfOuputSlots(2);
	Outputgen* outgen=new Outputgen("OutputgenWriter");
	outgen->out2.setCacheType(Slot::CACHE_MANAGED);
	Slot* in1=dynamic_cast<Slot*>(getOutputSlot(0).second);
	Slot* in2=dynamic_cast<Slot*>(getOutputSlot(1).second);
	_pluginMan->insertInstance(outgen);
	_pluginMan->connect(&(outgen->out1),in1);
	_pluginMan->connect(&(outgen->out2),in2);



    }



};

void testDataToGroup()
{
    ParameterFile file;
    file.save("TestGroupReader.wrp");

    Outputgen* generator=new Outputgen("OutgenReader");
    generator->out1.setCacheType(Slot::CACHE_MANAGED);
    std::vector<std::string> paths;
    paths.push_back(CHARON_PLUGINS);
    TestGroupReader* group=new TestGroupReader;
    group->pluginPaths=paths;
    //generator.initialize();
    group->initialize();


    PluginManager* man=new PluginManager(paths);
    man->insertInstance(group);
    man->insertInstance(generator);
    Slot* in=dynamic_cast<Slot*>(group->getInputSlot(0).first);
    Slot* in2=dynamic_cast<Slot*>(group->getInputSlot(1).first);
    man->connect(&(generator->out1),in);
    man->connect(&(generator->out2),in2);
    sout<<"Number of input in Testgroupreader "<<group->getNumberOfInputSlots()<<std::endl;
    sout<<"Number of output in Testgroupreader "<<group->getNumberOfOutputSlots()<<std::endl;

    man->runWorkflow();
    man->saveParameterFile("testDataToGroup.wrp");
    delete man;

}

void testDataFromGroup()
{
    ParameterFile file;
    file.save("TestGroupWriter.wrp");

    Reader* readerob=new Reader("Reader");

    std::vector<std::string> paths;
    paths.push_back(CHARON_PLUGINS);
    TestGroupWriter* group=new TestGroupWriter;
    group->pluginPaths=paths;
    //generator.initialize();
    group->initialize();


    PluginManager* man=new PluginManager(paths);
    man->insertInstance(group);
    man->insertInstance(readerob);
    Slot* out1=dynamic_cast<Slot*>(group->getOutputSlot(0).first);
    Slot* out2=dynamic_cast<Slot*>(group->getOutputSlot(1).first);
    man->connect(out1,&(readerob->in1));
    man->connect(out2,&(readerob->in2));
    sout<<"Number of input in Testgroupwriter "<<group->getNumberOfInputSlots()<<std::endl;
    sout<<"Number of output in Testgroupwriter "<<group->getNumberOfOutputSlots()<<std::endl;
    man->runWorkflow();
    man->saveParameterFile("testDataFromGroup.wrp");

    delete man;
}

int main()
{
    sout.assign(std::cout);
    int ret = EXIT_SUCCESS;
    ret |= ExceptionHandler::run(testDataToGroup);
    ret |= ExceptionHandler::run(testDataFromGroup);

    return ret;


}
