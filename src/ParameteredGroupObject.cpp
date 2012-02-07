#include "../include/charon-core/ParameteredGroupObject.h"
#include "../include/charon-core/PluginManager.h"


ParameteredGroupObject::ParameteredGroupObject(const std::string &className, const std::string &name, const std::string &doc)
    :ParameteredObject(className,name,doc)
{
    _addParameter(pluginPaths,"pluginPaths","The paths where the plugins are stored.","string");
    _addParameter(workFlowFile,"workflowfile","The workflow contained in this group","FileOpen");
    _pluginMan=0;


}

ParameteredGroupObject::~ParameteredGroupObject()
{

}

void ParameteredGroupObject::initialize()
{
    ParameteredObject::initialize();

    if(_pluginMan!=0)
    {
	_pluginMan->reset();
	delete _pluginMan;

    }
    _pluginMan=new PluginManager(pluginPaths(),true);
    _pluginMan->loadParameterFile(workFlowFile());
    _inputs=new InputSlotBundle("InputSlotBundle",getName()+".inputs");
    _outputs=new OutputSlotBundle("OutSlotBundle",getName()+".outputs");
    _inputs->addSlot(new VirtualSlot());
    _inputs->addSlot(new VirtualSlot(1));
    _inputs->addSlot(new VirtualSlot(2));
    _inputs->addSlot(new VirtualSlot(3));

    _outputs->addSlot(new VirtualSlot());
    _outputs->addSlot(new VirtualSlot(1));
    _outputs->addSlot(new VirtualSlot(2));
    _outputs->addSlot(new VirtualSlot(3));
    _pluginMan->insertInstance(_inputs);
    _pluginMan->insertInstance(_outputs);

    for(int i=0;i<_inputs->size();i++)
    {

	_addInputSlot(*(dynamic_cast<Slot*>((*(_inputs))[i])),(*(_inputs))[i]->getName(),"Virtual",(*(_inputs))[i]->getType());
    }




    initializeGroup();
}

void ParameteredGroupObject::initializeGroup()
{
    // Does nothing!
}

void ParameteredGroupObject::execute()
{
    PARAMETEREDOBJECT_AVOID_REEXECUTION;
    ParameteredObject::execute();

    executeGroup();
}

void ParameteredGroupObject::finalize()
{

    finalizeGroup();
    if(_pluginMan!=0)
    {
	_pluginMan->reset();
	delete _pluginMan;

    }
    ParameteredObject::finalize();

}

void ParameteredGroupObject::finalizeGroup()
{
    // Does nothing!
}

void ParameteredGroupObject::executeGroup()
{
    if(_pluginMan!=0)
	_pluginMan->executeWorkflow();;
}





SlotBundle::SlotBundle(const std::string &className, const std::string &name, const std::string &doc)
    :ParameteredObject(className,name,doc)
{
}

SlotBundle::~SlotBundle()
{

}

void SlotBundle::execute()
{
    //executeBundle();
}



void SlotBundle::addSlot(VirtualSlot *sl)
{
    _virtualOutputSlots.push_back(sl);

}

void InputSlotBundle::addSlot(VirtualSlot *sl)
{
    _addOutputSlot(*sl,sl->getName(),"VirtualSlot",sl->getType());
    SlotBundle::addSlot(sl);
}

void OutputSlotBundle::addSlot(VirtualSlot *sl)
{

    _addInputSlot(*sl,sl->getName(),"VirtualSlot",sl->getType());
    SlotBundle::addSlot(sl);
}

InputSlotBundle::InputSlotBundle(const std::string &className, const std::string &name, const std::string &doc)
    :SlotBundle(className,name,doc)
{
}

OutputSlotBundle::OutputSlotBundle(const std::string &className, const std::string &name, const std::string &doc)
    :SlotBundle(className,name,doc)
{
}

void SlotBundle::initialize()
{
}

void SlotBundle::finalize()
{
}

Slot * SlotBundle::operator [](int i)
{
    return _virtualOutputSlots[i];
}

int SlotBundle::size()
{
    return _virtualOutputSlots.size();
}




