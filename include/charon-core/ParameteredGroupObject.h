#ifndef PARAMETEREDGROUPOBJECT_H
#define PARAMETEREDGROUPOBJECT_H

#include "ParameteredObject.h"
#include "PluginManager.h"



/**
  * @interface SlotBundleIntf
  * @brief Interface for SlotBundle
  */
class SlotBundleIntf
{
public:


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
    void setNumberOfVirtualSlots(int num);

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





    /// Size of bundle
    virtual int size();

protected:

    virtual void _addAllSlots()=0;
    virtual void _removeAllSlots()=0;
    std::vector<VirtualOutputSlot*> _virtualOutputSlots;
    std::vector<VirtualInputSlot*> _virtualInputSlots;
    BundleContext _context;

private:
    CacheOption _overrideCaches;

    void _deleteAllSlots();


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
    std::vector<VirtualInputSlot*>& getSlotVector();

    std::vector<VirtualOutputSlot*>& getInternalSlotVector();

protected:

    void _addAllSlots();
    void _removeAllSlots();



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
    std::vector<VirtualOutputSlot*>&  getSlotVector();

    std::vector<VirtualInputSlot*>& getInternalSlotVector();
protected:

    void _addAllSlots();
    void _removeAllSlots();


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

    void setNumberOfInputSlots(int i);
    void setNumberOfOuputSlots(int i);

    const int getNumberOfInputSlots() const;
    const int getNumberOfOutputSlots() const;

    const std::pair<InputSlotIntf*,OutputSlotIntf*> getInputSlot(int slotnr) const;
    const std::pair<OutputSlotIntf*,InputSlotIntf*> getOutputSlot(int slotnr) const;

    /// Custom save
    /**
      * saves the internal workflow
      */
    virtual void onSave(ParameterFile &pf) const;


protected:
    PluginManager* _pluginMan;

    InputSlotBundle* _inputs;
    OutputSlotBundle* _outputs;



};



#endif // PARAMETEREDGROUPOBJECT_H
