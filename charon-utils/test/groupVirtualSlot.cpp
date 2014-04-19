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
/** @file   groupVirtualSlot.cpp
 *  @brief  Unit tests VirtualSlot and ParameteredGroupObject class. See groupVirtualSlot.cpp in charon-core/test/ for details
 *  @date   06.02.2012
 *  @author <a href="mailto:gerald.mwangi@gmx.de">Gerald Mwangi</a>
 */
#ifndef PENGUINJPGFILE
/// test image file
#define PENGUINJPGFILE
#error PENGUINJPGFILE not defined!
#endif
#include <IfGroup.h>
#include <ParameteredGroupObject.h>
#include "../include/charon-utils/DataManagerCImgList.hxx"
#include <ExceptionHandler.h>

using namespace cimg_library;
/// sample ParameteredObject class.
class Reader : public ParameteredObject {
public:
	/// sample integer input slot
	InputSlot<CImgList<double> >   in1;


	/// create a new sample object
	/// @param name             Object name
	Reader(const std::string& name = "") :
			ParameteredObject("Reader", name, "read slot data")
	{
		_addInputSlot (in1,  "in1",  "image","CImgList");

	}

protected:
	virtual void execute() {
		sout<<"Reader width: "<<in1()[0].width()<<" height: "<<in1()[0].height()<<std::endl;
		if(in1()[0].width()!=60||in1()[0].height()!=60)
		    raise("Invalid Image size");
	}
};

/// sample ParameteredObject class.
class Outputgen : public ParameteredObject {

public:
	/// sample integer output slot
	OutputSlot<CImgList<double> >    out1;


	/// create a new sample object
	/// @param name             Object name
	Outputgen(const std::string& name = "") :
			ParameteredObject("outputgen", name,
				"class to generate int and float output")
	{
		// slots
		_addOutputSlot(out1, "out1", "integer output slot","CImgList");

	}

protected:
	virtual void execute() {
	    out1().assign(PENGUINFILE);
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


	setNumberOfInputSlots(1);
	Reader* rd=new Reader("Reader");
	Slot* out1=dynamic_cast<Slot*>(getInputSlot(0).second);

	_pluginMan->insertInstance(rd);
	//_pluginMan->connect(out1->getParent().getName()+"."+out1->getName(),rd->getName()+"."+rd->in1.getName());
	_pluginMan->connect(out1,&(rd->in1));



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


	setNumberOfOuputSlots(1);
	Outputgen* outgen=new Outputgen("OutputgenWriter");

	Slot* in1=dynamic_cast<Slot*>(getOutputSlot(0).second);

	_pluginMan->insertInstance(outgen);
	_pluginMan->connect(&(outgen->out1),in1);




    }



};

void testDataToGroup()
{


    Outputgen* generator=new Outputgen("OutgenReader");
    generator->out1.setCacheType(Slot::CACHE_MANAGED);
    std::vector<std::string> paths;
    paths.push_back(LOCAL_PLUGIN_DIR);
    TestGroupReader* group=new TestGroupReader;
    group->pluginPaths=paths;
    //generator.initialize();
    ParameterFile file;
    //file.set<std::string>(group->getName()+"-inputs.type","InputSlotBundle");
   // file.set<std::string>(group->getName()+"-outputs.type","OutputSlotBundle");
    file.save("TestGroupReader.wrp");

    group->initialize();


    PluginManager* man=new PluginManager(paths);
    man->insertInstance(group);
    man->insertInstance(generator);
    Slot* in=dynamic_cast<Slot*>(group->getInputSlot(0).first);

    man->connect(&(generator->out1),in);

    sout<<"Number of input in Testgroupreader "<<group->getNumberOfInputSlots()<<std::endl;
    sout<<"Number of output in Testgroupreader "<<group->getNumberOfOutputSlots()<<std::endl;

    man->runWorkflow();
    man->saveParameterFile("testDataToGroup.wrp");
    delete man;

}

void testDataFromGroup()
{


    Reader* readerob=new Reader("Reader");

    std::vector<std::string> paths;
    paths.push_back(LOCAL_PLUGIN_DIR);
    TestGroupWriter* group=new TestGroupWriter;
    group->pluginPaths=paths;
    //generator.initialize();
    ParameterFile file;
   // file.set<std::string>(group->getName()+"-inputs.type","InputSlotBundle");
   // file.set<std::string>(group->getName()+"-outputs.type","OutputSlotBundle");
    file.save("TestGroupWriter.wrp");
    group->initialize();


    PluginManager* man=new PluginManager(paths);
    man->insertInstance(group);
    man->insertInstance(readerob);
    Slot* out1=dynamic_cast<Slot*>(group->getOutputSlot(0).first);

    man->connect(out1,&(readerob->in1));

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
