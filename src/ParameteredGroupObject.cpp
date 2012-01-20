#include "../include/charon-core/ParameteredGroupObject.h"

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
    _pluginMan=new PluginManager(pluginPaths(),DEFAULT_DEBUG_SUFFIX);
    _pluginMan->loadParameterFile(workFlowFile());



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
