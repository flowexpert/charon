/*	Copyright (C) 2012 Gerald Mwangi

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

#include "../include/charon-core/WhileGroup.h"
#include "../include/charon-core/ExceptionHandler.h"
/** @file   testWhileGroup.cpp
 *  @brief  Unit tests WhileGroup and WhileGroupStatement class.
 *  @date   06.02.2012
 *  @author <a href="mailto:gerald.mwangi@gmx.de">Gerald Mwangi</a>
 *  This test contains a whilegroup. OUTSIDE of the group there is a number generator (extoutgen) with 2 slots, and a reader object (extreader).
 *  INSIDE the group there are 2 nested readers (intreader,intreader2) connected to the Virtual input slots and a generator connected to the virtual output slots (intoutgen).
 *  In loop mode, the virtual output slot 0 is looped to the virtual input slot 0.
 *  The while group should run 3 iterations.
 *  0th iteration: (intreader,intreader2).in1=extoutgen.out1 and (intreader,intreader2).in2=extoutgen.out2
 *  1st - 3rd iteration: (intreader,intreader2).in1=intoutgen.out1 and (intreader,intreader2).in2=extoutgen.out2
 *  after 3rd iteration: extreader.in1=intoutgen.in1, extreader.in2=intoutgen.in2
 */

int _iter;
int _maxit;
bool loopMode=false;
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
	    if(getName()=="ExternReader")
	    {
		if(in1!=20||in2!=0.33f) //Values of intoutgen
			raise("Incorrect Values in Loopmode!");
	    }
	    else if(loopMode)
	    {

		//0th iter values of extoutgen, else   (intreader,intreader2).in1=intoutgen.out1 and (intreader,intreader2).in2=extoutgen.out2
		if((_iter>0)&&(in1!=20||in2!=5.5f)||(_iter==0&&(in1!=10||in2!=5.5f)))
			raise("Incorrect Values in Loopmode!");
	    }
	    else
	    {
		if(in1!=10||in2!=5.5f)
			raise("Incorrect Values in Loopmode!");
	    }
	    //Did we shoot over the maximum?
	    if(_iter>_maxit)
		raise("Too many iterations!");

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
	Outputgen(const std::string& name = "",int val1=10,float val2=5.5f) :
			ParameteredObject("outputgen", name,
				"class to generate int and float output"),
			out1(val1),   // set initial values
			out2(val2)  // ------- " --------
	{
		// slots
		_addOutputSlot(out1, "out1", "integer output slot");
		_addOutputSlot(out2, "out2", "float output slot");
	}

protected:
	virtual void execute() {
	}
};

class SimpleOutput : public ParameteredObject {
public:


	/// create a new sample object
	/// @param name             Object name
	SimpleOutput(const std::string& name = "") :
			ParameteredObject("SimpleOutput", name, "send name to standard output")
	{

	}

protected:
	virtual void execute() {
	    std::cout<<getName()<<std::endl;
	}
};

class StatementGenerator : public ParameteredObject {
public:
	/// default constructor
	/// \param name             instance name
	StatementGenerator(const std::string& name = "",int max=3)
	    :ParameteredObject("StatementGenerator",name,"StatementGenerator")
	{
	    _maxit=max;
	    _iter=0;
	    _addOutputSlot(statement,"statement","","bool");

	   // statement=true;
	}

	/// statement
	OutputSlot< bool > statement;



	/// Update object.
	virtual void execute()
	{
	    statement=_iter<_maxit;
	    sout<<"Iteration "<<_iter<<std::endl;
	    if(statement)
		_iter++;
	}

private:

};

class TestWhileGroup:public WhileGroup
{
public:
    TestWhileGroup()
	:WhileGroup("TestWhileGroup")
    {
	workFlowFile="TestWhileGroup.wrp";
    }
    void initializeWhileGroup()
    {
	StatementGenerator* st=new StatementGenerator("Statementgen");

	WhileGroupStatement* wst=dynamic_cast<WhileGroupStatement*>(_pluginMan->getInstance("WhileGroupStatement"));


	SimpleOutput* simpleout=new SimpleOutput("Simpleout");
	_pluginMan->insertInstance(simpleout);
	_pluginMan->insertInstance(st);
	_pluginMan->connect(st->statement,wst->statement);
    }
};

class TestWhileGroupLoop:public WhileGroup
{
public:
    TestWhileGroupLoop()
	:WhileGroup("TestWhileGroupLoop")
    {
	workFlowFile="TestWhileGroupLoop.wrp";
    }
    void initializeWhileGroup()
    {
//	setNumberOfInputSlots(2);
	//setNumberOfOuputSlots(2);
	loopInputToOutput(0,0);
	StatementGenerator* st=new StatementGenerator("Statementgen");

	WhileGroupStatement* wst=dynamic_cast<WhileGroupStatement*>(_pluginMan->getInstance("WhileGroupStatement"));

	Reader* intreader=new Reader("Reader");
	Reader* intreader2=new Reader("Reader2");
	Outputgen* intoutgen=new Outputgen("InnerOutgen",20,0.33f);
	_pluginMan->insertInstance(intoutgen);
	_pluginMan->insertInstance(intreader);
	_pluginMan->insertInstance(intreader2);
	_pluginMan->insertInstance(st);
	_pluginMan->connect(st->statement,wst->statement);
	OutputSlotIntf* in1=0;//getInputSlot(0).second;
	OutputSlotIntf* in2=0;//getInputSlot(1).second;
	_pluginMan->connect(&intreader->in1,dynamic_cast<Slot*>(in1));
	_pluginMan->connect(&intreader->in2,dynamic_cast<Slot*>(in2));
	_pluginMan->connect(&intreader2->in1,dynamic_cast<Slot*>(in1));
	_pluginMan->connect(&intreader2->in2,dynamic_cast<Slot*>(in2));

//	_pluginMan->connect(&intoutgen->out1,dynamic_cast<Slot*>(getOutputSlot(0).second));
	//_pluginMan->connect(&intoutgen->out2,dynamic_cast<Slot*>(getOutputSlot(1).second));

    }
};

void testWhileIterationSimplOutput()
{
    loopMode=false;
    ParameterFile file;
    file.save("TestWhileGroup.wrp");

    TestWhileGroup* testWhilegroup=new TestWhileGroup;
    std::vector<std::string> paths;
    paths.push_back(CHARON_PLUGINS);
    testWhilegroup->pluginPaths=paths;
    PluginManager man(paths);
    man.insertInstance(testWhilegroup);
    testWhilegroup->initialize();
    man.runWorkflow();
    //Did we shoot over the maximum?
    if(_iter!=_maxit)
	throw;
}

void testWhileIterationLoop()
{
    loopMode=true;
    ParameterFile file;
    file.save("TestWhileGroupLoop.wrp");

    TestWhileGroupLoop* testWhilegroup=new TestWhileGroupLoop;
    std::vector<std::string> paths;
    paths.push_back(CHARON_PLUGINS);
    testWhilegroup->pluginPaths=paths;
    PluginManager man(paths);

    man.insertInstance(testWhilegroup);
    testWhilegroup->initialize();

    Outputgen* extoutgen=new Outputgen("ExternOutputgen");
    Reader* extreader=new Reader("ExternReader");
    man.insertInstance(extoutgen);
    man.insertInstance(extreader);
	//man.connect(&extoutgen->out1,dynamic_cast<Slot*>(testWhilegroup->getInputSlot(0).first));
	//man.connect(&extoutgen->out2,dynamic_cast<Slot*>(testWhilegroup->getInputSlot(1).first));
	//man.connect(&extreader->in1,dynamic_cast<Slot*>(testWhilegroup->getOutputSlot(0).first));
	//man.connect(&extreader->in2,dynamic_cast<Slot*>(testWhilegroup->getOutputSlot(1).first));
    man.runWorkflow();
}

int main()
{
    sout.assign(std::cout);
    int ret = EXIT_SUCCESS;

    ret |= ExceptionHandler::run(testWhileIterationSimplOutput);
    ret |= ExceptionHandler::run(testWhileIterationLoop);

    return ret;

}


