/*  This file is part of Charon.

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
#include "../include/charon-core/ParameteredGroupObject.h"
#include "../include/charon-core/PluginManager.h"


ParameteredGroupObject::ParameteredGroupObject(const std::string &className, const std::string &name, const std::string &doc)
	:ParameteredObject(className,name,doc)
{
	_addParameter(pluginPaths,"pluginPaths","The paths where the plugins are stored.","string");
	_addParameter<bool>(debugSuffix,"debugSuffix","Load debug plugins suffixed with '_d'",1);
	_addParameter(workFlowFile,"workflowfile","The workflow contained in this group","FileOpen");
	_pluginMan=0;
	_inputs=0;
	_outputs=0;
	_setDynamic(true);
}

ParameteredGroupObject::~ParameteredGroupObject()
{
}

void ParameteredGroupObject::initialize()
{
	ParameteredObject::initialize();

	if(_inputs)
	{
		std::vector<VirtualInputSlot*> vinput=_inputs->getSlotVector();
		for(size_t i=0;i<vinput.size();i++)
		{
			VirtualInputSlot* in=vinput[i];
			_removeInputSlot(in->getName());
			Parameter<int> *par=_loopOutputNumber[i];
			_removeSomething("parameters",par->getName());
			delete par;
			breakLoop(i);
		}
	}
	_loopOutputNumber.clear();

	if(_outputs)
	{
		std::vector<VirtualOutputSlot*> voutput=_outputs->getSlotVector();
		for(size_t i=0;i<voutput.size();i++)
		{
			VirtualOutputSlot* out=voutput[i];
			_removeOutputSlot(out->getName());
		}
	}

	_inputs=0;
	_outputs=0;
	if (_pluginMan!=0) {
		_pluginMan->reset();
		delete _pluginMan;
		_pluginMan = 0;
	}

	_pluginMan = new PluginManager(pluginPaths(),debugSuffix());
	_pluginMan->loadParameterFile(workFlowFile());
	const std::map<std::string, ParameteredObject*>& objs=_pluginMan->getObjectList();

	std::map<std::string, ParameteredObject *>::const_iterator it=objs.begin();
	for(;it!=objs.end();it++)
	{
		ParameteredObject* obj=it->second;
		InputSlotBundleIntf* tinputs=dynamic_cast<InputSlotBundleIntf*>(obj);
		OutputSlotBundleIntf* toutputs=dynamic_cast<OutputSlotBundleIntf*>(obj);
		if(tinputs)
			_inputs=tinputs;
		if(toutputs)
			_outputs=toutputs;
	}

	if(_inputs)
	{
		std::vector<VirtualInputSlot*> vinput=_inputs->getSlotVector();
		for(size_t i=0;i<vinput.size();i++)
		{
			VirtualInputSlot* in=vinput[i];
			_addInputSlot(*in,in->getName(),in->getDisplayName(),"",in->getType());
			Parameter<int> *par=new Parameter<int>(-1);
			std::stringstream pname;
			pname<<"loop_input_"<<i<<"_to_output";
			std::stringstream pdoc;
			pdoc<<"The input "<<i<<" gets the data of the given output after one iteration of a loop. To disable the connection, set to -1";
			_addParameter(*par,pname.str(),pdoc.str(),"int");
			_loopOutputNumber.push_back(par);
		}
	}
	if(_outputs)
	{
		std::vector<VirtualOutputSlot*> voutput=_outputs->getSlotVector();
		for(size_t i=0;i<voutput.size();i++)
		{
			VirtualOutputSlot* out=voutput[i];
			_addOutputSlot(*out,out->getName(),out->getDisplayName(),"",out->getType());
		}
	}

	initializeGroup();

	#pragma message ("need to load slotbundle connections")
	//_inputs->loadConnection(ParameterFile(workFlowFile),_pluginMan);
	//_outputs->loadConnection(ParameterFile(workFlowFile),_pluginMan);
}

void ParameteredGroupObject::initializeGroup()
{
	// Does nothing!
}

void ParameteredGroupObject::execute()
{
	executeGroup();
}

void ParameteredGroupObject::finalize()
{

	finalizeGroup();
	if (_pluginMan) {
		_pluginMan->reset();
		delete _pluginMan;
		_pluginMan=0;
	}
	ParameteredObject::finalize();
}

void ParameteredGroupObject::finalizeGroup()
{
	// Does nothing!
}

void ParameteredGroupObject::executeGroup() {
	if(_pluginMan!=0) {
		_pluginMan->runWorkflow();
	}
}
















void ParameteredGroupObject::onSave(ParameterFile&) const
{
	_pluginMan->saveParameterFile(workFlowFile());
}



void ParameteredGroupObject::loopInputToOutput(int input, int output)
{
	VirtualOutputSlot* out=_inputs->getInternalSlotVector()[input];
	VirtualInputSlot* in=_outputs->getInternalSlotVector()[output];
	out->setLoopPartner(in);
	out->setLoop(false);
	_loopedSlots.insert(std::pair<int,VirtualOutputSlot*>(input,out));
}

void ParameteredGroupObject::breakLoop(int input)
{
	std::map<int,VirtualOutputSlot*>::iterator it;
	it=_loopedSlots.find(input);
	if(it==_loopedSlots.end())
		sout<<this->getName()<<": Input "<<input<<" is not looped!";

	VirtualOutputSlot* out=(*it).second;
	out->setLoopPartner(0);
	out->setLoop(false);
	_loopedSlots.erase(it);
}

void ParameteredGroupObject::enableLoopConnections()
{
	std::map<int,VirtualOutputSlot*>::iterator it=_loopedSlots.begin();
	for(;it!=_loopedSlots.end();it++)
	{
		VirtualOutputSlot* out=(*it).second;
		out->setLoop(true);
	}
}

void ParameteredGroupObject::disableLoopConnections()
{
	std::map<int,VirtualOutputSlot*>::iterator it=_loopedSlots.begin();
	for(;it!=_loopedSlots.end();it++)
	{
		VirtualOutputSlot* out=(*it).second;
		out->setLoop(false);
	}
}

void ParameteredGroupObject::prepareDynamicInterface(const ParameterFile &file)
{
	workFlowFile.load(file);
	pluginPaths.load(file);
	initialize();
}

void ParameteredGroupObject::onLoad(
		const ParameterFile& pf, const PluginManagerInterface* /*man*/) {
	for(size_t i=0; i<_loopOutputNumber.size(); i++) {
		Parameter<int> *par=_loopOutputNumber[i];
		par->load(pf);
		if((*par)()>=0)
		{
			loopInputToOutput(i,(*par)());
		}
	}
}





