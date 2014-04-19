#ifndef SLOTBUNDLE_H
#define SLOTBUNDLE_H
#include "SlotBundleInterfaces.h"
#include "ParameteredObject.hxx"
/// Represents slot interface of group
/** Loadable into a nested group workflow to expose slots to a global group.
 */
class VirtualOutputSlot ;
class VirtualInputSlot ;

class charon_groups_DLL_PUBLIC SlotBundle: public ParameteredObject,public SlotBundleIntf
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

	/// The number of virtual slots
	Parameter< int > num_slots;

	/// Set the number of slots
	void setNumberOfVirtualSlots(int num);

	///prepare the interface of the bundle
	void prepareDynamicInterface(const ParameterFile &file);

	/// Initialize
	void initialize();

	/// Execute
	/** Depending on the cache settings of the slots and the bundle cache setting
	  * this execute() will handle the transport of the data in the slots
	  */
	void execute();

	/// Finalize
	void finalize();


	/// Load connections
	void loadConnection(ParameterFile pf,PluginManagerInterface* man);



	/// Size of bundle
	virtual int size();


	protected:
	virtual void _addSlot(int i)=0;
	virtual void _removeSlot(int i)=0;

	std::vector<VirtualOutputSlot*> _virtualOutputSlots;
	std::vector<VirtualInputSlot*> _virtualInputSlots;


private:


	void _deleteAllSlots();


};

#endif // SLOTBUNDLE_H
