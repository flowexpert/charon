#ifndef PARAMETEREDGROUPOBJECT_H
#define PARAMETEREDGROUPOBJECT_H

#include "ParameteredObject.h"


class PluginManager;
/**
  * @interface SlotBundleIntf
  * @brief Interface for SlotBundle
  */
class SlotBundleIntf
{
public:
    /// GetSlotVector
    virtual Slot* operator[](int i)=0;

    /// Size of bundle
    virtual int size()=0;
};


/**
  * @class SlotBundle
  * @brief Represents slot interface of group. Loadable into a nested group workflow to expose slots to a global group.
  */

class SlotBundle: public ParameteredObject,public SlotBundleIntf
{




public:
    /// Default constructor.
    /** Init class name with given string.
     *  Also generates a unique instance name if necessary.
     *  \param className    Initialization value for className property.
     *  \param name         Instance name (auto generation if empty)
     *  \param doc  Class docstring (for metadata)
     */
    SlotBundle(const std::string& className,
	       const std::string& name = "", const std::string& doc = "");
    virtual ~SlotBundle();


    /// Set the number of slots
    void setNumberOfSlots(int i);

    /// Initialize
    void initialize();

    /// Execute
    /** Depending on the cache settings of the slots and the bundle cache setting
      * this execute() will handle the transport of the data in the slots
      */
    void execute();



    /// Finalize
    void finalize();

    /// CacheOption
    /** Bundle cache setting. Determines whether to override the individual cache settings of the slots.
      */
    enum CacheOption
    {
	OVERRIDE,
	NO_OVERRIDE

    };

    /// BundleContext
    enum BundleContext
    {
	SAME_CONTEXT,	    /// inputSlots and outputSlots in same process/thread
	DIFFERENT_CONTEXTS  /// inputSlots and outputSlots in different processes/threads
    };

    /// SetCacheOption
    void setCacheOption(CacheOption opt);

    /// GetCacheOption
    CacheOption getCacheOption() const;

    /// GetSlotVector
    virtual Slot* operator[](int i);

    /// Size of bundle
    virtual int size();
protected:

    virtual void _addSlot(Slot* sl)=0;
    virtual void _removeAllSlots()=0;
    std::vector<VirtualSlot*> _virtualOutputSlots;
    std::vector<VirtualInputSlot*> _virtualInputSlots;
    BundleContext _context;

private:
    CacheOption _overrideCaches;

    /// Configuration.
    /**
      * stores name, doc, types of the slots.
      */
    ParameterList<std::string> _config;

    ParameterFile _bundleConfigFile;

    /// Execute Bundle
    //void executeBundle();





};

/**
  * @class InputSlotBundle
  * @brief The function getSlotVector exposes _inputSlots to the parent group.
  */

class InputSlotBundle:public SlotBundle
{
public:
    InputSlotBundle(const std::string& className,
		    const std::string& name = "", const std::string& doc = "");

    void addSlot(VirtualSlot *sl);



};

/**
  * @class InputSlotBundle
  * @brief The function getSlotVector exposes _outputSlots to the parent group.
  */

class OutputSlotBundle: public SlotBundle
{
public:
    OutputSlotBundle(const std::string& className,
		    const std::string& name = "", const std::string& doc = "");

    void addSlot(VirtualSlot *sl);


};



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

    InputSlotBundle* _inputs;
    OutputSlotBundle* _outputs;



};



#endif // PARAMETEREDGROUPOBJECT_H
