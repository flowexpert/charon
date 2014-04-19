#ifndef SLOTBUNDLEINTERFACES_H
#define SLOTBUNDLEINTERFACES_H
#include <vector>

#include <charon-core/DllEx.h>

class VirtualInputSlot;
class VirtualOutputSlot;

/// Interface for SlotBundle
class charon_core_PUBLIC SlotBundleIntf {
public:
	virtual ~SlotBundleIntf(){
	}

	/// Size of bundle
	virtual int size()=0;
};

/// Interface for InputSlotBundle
class InputSlotBundleIntf
{
public:
	virtual ~InputSlotBundleIntf() {
	}

	/// get virtual inputs to be exposed in parent workflow.
	virtual std::vector<VirtualInputSlot*>& getSlotVector()=0;

	/// get virtual inputs to be exposed in group workflow.
	virtual std::vector<VirtualOutputSlot*>& getInternalSlotVector()=0;
};

/// Interface for OutputSlotBundle
class OutputSlotBundleIntf
{
public:
	virtual ~OutputSlotBundleIntf(){
	}

	/// get virtual inputs to be exposed in parent workflow.
	virtual std::vector<VirtualOutputSlot*>& getSlotVector()=0;
	/// get virtual inputs to be exposed in group workflow.
	virtual std::vector<VirtualInputSlot*>& getInternalSlotVector()=0;
};

#endif // SLOTBUNDLEINTERFACES_H
