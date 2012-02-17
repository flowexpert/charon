

#include "../include/charon-core/WhileGroup.h"
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
		std::cout << "(II) \tRead slot data: "
			 << in1() << ", " << in2() <<" from "<<getName()<< std::endl;
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
	    _iter++;
	}

private:
	int _iter;
	int _maxit;
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
	setNumberOfInputSlots(2);
	setNumberOfOuputSlots(2);
	loopOutputToInput(0,0);
	StatementGenerator* st=new StatementGenerator("Statementgen");

	WhileGroupStatement* wst=dynamic_cast<WhileGroupStatement*>(_pluginMan->getInstance("WhileGroupStatement"));

	Reader* simpleout=new Reader("Reader");
	Reader* simpleout2=new Reader("Reader2");
	Outputgen* innerOutgen=new Outputgen("InnerOutgen",20,0.33f);
	_pluginMan->insertInstance(innerOutgen);
	_pluginMan->insertInstance(simpleout);
	_pluginMan->insertInstance(simpleout2);
	_pluginMan->insertInstance(st);
	_pluginMan->connect(st->statement,wst->statement);
	OutputSlotIntf* in1=getInputSlot(0).second;
	OutputSlotIntf* in2=getInputSlot(1).second;
	_pluginMan->connect(&simpleout->in1,dynamic_cast<Slot*>(in1));
	_pluginMan->connect(&simpleout->in2,dynamic_cast<Slot*>(in2));
	_pluginMan->connect(&simpleout2->in1,dynamic_cast<Slot*>(in1));
	_pluginMan->connect(&simpleout2->in2,dynamic_cast<Slot*>(in2));

	_pluginMan->connect(&innerOutgen->out1,dynamic_cast<Slot*>(getOutputSlot(0).second));
	_pluginMan->connect(&innerOutgen->out2,dynamic_cast<Slot*>(getOutputSlot(1).second));

    }
};

void testWhileIterationSimplOutput()
{
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
}

void testWhileIterationLoop()
{
    ParameterFile file;
    file.save("TestWhileGroupLoop.wrp");

    TestWhileGroupLoop* testWhilegroup=new TestWhileGroupLoop;
    std::vector<std::string> paths;
    paths.push_back(CHARON_PLUGINS);
    testWhilegroup->pluginPaths=paths;
    PluginManager man(paths);

    man.insertInstance(testWhilegroup);
    testWhilegroup->initialize();

    Outputgen* extoutgen=new Outputgen("ExternOutpugen");
    Reader* reader=new Reader("ExternReader");
    man.insertInstance(extoutgen);
    man.insertInstance(reader);
    man.connect(&extoutgen->out1,dynamic_cast<Slot*>(testWhilegroup->getInputSlot(0).first));
    man.connect(&extoutgen->out2,dynamic_cast<Slot*>(testWhilegroup->getInputSlot(1).first));
    man.connect(&reader->in1,dynamic_cast<Slot*>(testWhilegroup->getOutputSlot(0).first));
    man.connect(&reader->in2,dynamic_cast<Slot*>(testWhilegroup->getOutputSlot(1).first));
    man.runWorkflow();
}

int main()
{
    //sout.assign(std::cout);
    int ret = EXIT_SUCCESS;

   //ret |= ExceptionHandler::run(testWhileIterationSimplOutput);
    ret |= ExceptionHandler::run(testWhileIterationLoop);

    return ret;

}


