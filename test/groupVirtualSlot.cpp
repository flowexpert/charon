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
	if(in1!=10  || in2!=5.5f)
	    raise("Incorrect Values!");
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


//	setNumberOfInputSlots(2);
	Reader* rd=new Reader("Reader");


	//Virtual slots receiving Data from outside


		_pluginMan->insertInstance(rd);
		_pluginMan->connect("inputs.VirtualSlot-out0","Reader.in1");
		_pluginMan->connect("inputs.VirtualSlot-out1","Reader.in2");


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



		Outputgen* outgen=new Outputgen("OutputgenWriter");
		_pluginMan->insertInstance(outgen);
		_pluginMan->connect("outputs.VirtualSlot-in0","OutputgenWriter.out1");
		_pluginMan->connect("outputs.VirtualSlot-in1","OutputgenWriter.out2");



    }



};

void createChildWorkflows()
{
	PluginManager man(MODULE_DIR,CHARON_PLUGINS);




	ParameteredObject* out=man.createInstance("OutputSlotBundle","outputs");



	ParameterFile pfout;


	out->setParameter("num_slots",2);

	out->saveParameters(pfout);
	out->prepareDynamicInterface(pfout);



	man.saveParameterFile("TestGroupWriter.wrp");

	man.reset();

	ParameteredObject* in=man.createInstance("InputSlotBundle","inputs");
	in->setParameter("num_slots",2);
	ParameterFile pf;

	in->saveParameters(pf);
	in->prepareDynamicInterface(pf);

	man.saveParameterFile("TestGroupReader.wrp");

	man.reset();

	in=man.createInstance("InputSlotBundle","inputs");
	out=man.createInstance("OutputSlotBundle","outputs");

	man.connect("inputs.VirtualSlot-out0","outputs.VirtualSlot-in0");
	man.connect("inputs.VirtualSlot-out1","outputs.VirtualSlot-in1");
	man.saveParameterFile("TestGroupPassThrough.wrp");


}

void testDataToGroup()
{
    //Needed to initialize the groups
    ParameterFile file;
    file.save("TestGroupReader.wrp");

    //Create output generator
    Outputgen* generator=new Outputgen("OutgenReader");
    generator->out1.setCacheType(Slot::CACHE_MANAGED);
    std::vector<std::string> paths;
    paths.push_back(CHARON_PLUGINS);

    //Create group
    TestGroupReader* group=new TestGroupReader;
    group->pluginPaths=paths;
    //generator.initialize();
    group->initialize();


    PluginManager* man=new PluginManager(paths);
    man->insertInstance(group);
    man->insertInstance(generator);

    //Connect output generator to virtual slots of group. Data should flow from outputgen into group into nested reader
	Slot* in=0;//dynamic_cast<Slot*>(group->getInputSlot(0).first);
	Slot* in2=0;//dynamic_cast<Slot*>(group->getInputSlot(1).first);
    man->connect(&(generator->out1),in);
    man->connect(&(generator->out2),in2);
	sout<<"Number of input in Testgroupreader "<<std::endl;//group->getNumberOfInputSlots()<<std::endl;
	sout<<"Number of output in Testgroupreader "<<std::endl;//group->getNumberOfOutputSlots()<<std::endl;

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
	Slot* out1=0;//dynamic_cast<Slot*>(group->getOutputSlot(0).first);
	Slot* out2=0;//dynamic_cast<Slot*>(group->getOutputSlot(1).first);
    man->connect(out1,&(readerob->in1));
    man->connect(out2,&(readerob->in2));
	//sout<<"Number of input in Testgroupwriter "<<group->getNumberOfInputSlots()<<std::endl;
	//sout<<"Number of output in Testgroupwriter "<<group->getNumberOfOutputSlots()<<std::endl;
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
