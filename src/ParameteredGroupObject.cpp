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
    _inputs=new InputSlotBundle("InputSlotBundle",getName()+"-inputs");
    _outputs=new OutputSlotBundle("OutSlotBundle",getName()+"-outputs");


    //_inputs->setNumberOfVirtualSlots(4);
    //_outputs->setNumberOfVirtualSlots(4);
    _pluginMan->insertInstance(_inputs);
    _pluginMan->insertInstance(_outputs);



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



int SlotBundle::size()
{
    return _virtualOutputSlots.size();
}

void InputSlotBundle::_removeAllSlots()
{
    for(int i=0;i<_virtualOutputSlots.size();i++)
    {
	_removeOutputSlot(_virtualOutputSlots[i]->getName());

    }
}

void OutputSlotBundle::_removeAllSlots()
{
    for(int i=0;i<_virtualInputSlots.size();i++)
    {
	_removeInputSlot(_virtualInputSlots[i]->getName());
    }
}

void InputSlotBundle::_addAllSlots()
{
    for(int i=0;i<_virtualOutputSlots.size();i++)
    {
	_addOutputSlot(*((Slot*)_virtualOutputSlots[i]),_virtualOutputSlots[i]->getName(),"Virtual Slot",_virtualOutputSlots[i]->getType());
    }
}

void OutputSlotBundle::_addAllSlots()
{
    for(int i=0;i<_virtualInputSlots.size();i++)
    {
	_addInputSlot(*((Slot*)_virtualInputSlots[i]),_virtualInputSlots[i]->getName(),"Virtual Slot",_virtualInputSlots[i]->getType());
    }
}

void SlotBundle::_deleteAllSlots()
{
    _removeAllSlots();
    for(int i=0;i<_virtualOutputSlots.size();i++)
    {
	delete _virtualOutputSlots[i];
	delete _virtualInputSlots[i];
    }
    _virtualOutputSlots.clear();
    _virtualInputSlots.clear();
}

void SlotBundle::setNumberOfVirtualSlots(int num)
{
    _deleteAllSlots();
    for(int i=0;i<num;i++)
    {
	VirtualOutputSlot* out=new VirtualOutputSlot(i);
	VirtualInputSlot* in= new VirtualInputSlot(i);
	out->setVirtualPartnerSlot(in);
	_virtualOutputSlots.push_back(out);
	_virtualInputSlots.push_back(in);
    }
    _addAllSlots();
}

std::vector<VirtualInputSlot*>& InputSlotBundle::getSlotVector()
{
    return _virtualInputSlots;
}

std::vector<VirtualOutputSlot*>& OutputSlotBundle::getSlotVector()
{
    return _virtualOutputSlots;
}

const int ParameteredGroupObject::getNumberOfInputSlots() const
{
    return _inputs->size();
}

const int ParameteredGroupObject::getNumberOfOutputSlots() const
{
    return _outputs->size();
}

const std::pair<InputSlotIntf*,OutputSlotIntf*> ParameteredGroupObject::getInputSlot(int slotnr) const
{
    VirtualOutputSlot* int_out=_inputs->getInternalSlotVector()[slotnr];
    VirtualInputSlot* in=_inputs->getSlotVector()[slotnr];
    if(slotnr<0||slotnr>_inputs->size())
	ParameteredObject::raise("(EE) Index out of range!!");
    return std::pair<InputSlotIntf*,OutputSlotIntf*>(dynamic_cast<InputSlotIntf*>(in),dynamic_cast<OutputSlotIntf*>(int_out));
}

const std::pair<OutputSlotIntf*,InputSlotIntf*> ParameteredGroupObject::getOutputSlot(int slotnr) const
{
    VirtualOutputSlot* out=_outputs->getSlotVector()[slotnr];
    VirtualInputSlot* int_in=_outputs->getInternalSlotVector()[slotnr];
    if(slotnr<0||slotnr>_outputs->size())
	ParameteredObject::raise("(EE) Index out of range!!");
    return std::pair<OutputSlotIntf*,InputSlotIntf*>(dynamic_cast<OutputSlotIntf*>(out),dynamic_cast<InputSlotIntf*>(int_in));
}

void ParameteredGroupObject::setNumberOfInputSlots(int i)
{
    std::vector<VirtualInputSlot*>& ins=_inputs->getSlotVector();

    for(int i=0;i<ins.size();i++)
    {
	_removeInputSlot(ins[i]->getName());


    }

    _inputs->setNumberOfVirtualSlots(i);


    for(int i=0;i<ins.size();i++)
    {


	_addInputSlot(*(dynamic_cast<Slot*>(ins[i])),ins[i]->getName(),"VirtualInput",ins[i]->getType());
    }

}

void ParameteredGroupObject::setNumberOfOuputSlots(int i)
{
    std::vector<VirtualOutputSlot*>& outs=_outputs->getSlotVector();
    for(int i=0;i<outs.size();i++)
    {

	_removeOutputSlot(outs[i]->getName());
    }

    _outputs->setNumberOfVirtualSlots(i);


    for(int i=0;i<outs.size();i++)
    {

	_addOutputSlot(*(dynamic_cast<Slot*>(outs[i])),outs[i]->getName(),"VirtualOutput",outs[i]->getType());
    }
}

std::vector<VirtualOutputSlot *> & InputSlotBundle::getInternalSlotVector()
{
    return _virtualOutputSlots;
}

std::vector<VirtualInputSlot *> & OutputSlotBundle::getInternalSlotVector()
{
    return _virtualInputSlots;
}




