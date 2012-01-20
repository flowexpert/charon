#ifndef PARAMETEREDGROUPOBJECT_H
#define PARAMETEREDGROUPOBJECT_H

#include "ParameteredObject.h"

#include "PluginManager.h"
class ParameteredGroupObject : public ParameteredObject
{
public:


    /// Default constructor.
    /** Init class name with given string.
     *  Also generates a unique instance name if necessary.
     *  \param className    Initialization value for className property.
     *  \param name         Instance name (auto generation if empty)
     *  \param doc  Class docstring (for metadata)
     */
    ParameteredGroupObject(const std::string& className,
	    const std::string& name = "", const std::string& doc = "");

    ParameterList<std::string> pluginPaths;
    Parameter<std::string> workFlowFile;

    /// Default deconstructor
    virtual ~ParameteredGroupObject();

    /// Initialization
    /** Calls initialize Group
     *
     */
    void initialize();

    /// Group Initialization
    /** Does nothing. Can be used in derived classes.
      */
    virtual void initializeGroup();

    /// Execution
    /** Calls executeGroup()
      */
    void execute();

    /// Execution
    /** Executes the group by calling executeWorkflow() in PluginManager
      */
    virtual void executeGroup();

    /// Finalization
    /** Calls finalizeGroup
      */
    void finalize();

    /// Finalization
    /** Does nothing. Can be used in derived classes.
      */
    virtual void finalizeGroup();

protected:
    PluginManager* _pluginMan;



};

#endif // PARAMETEREDGROUPOBJECT_H
