#include "charon-core/SlotBundle.h"

SlotBundle::SlotBundle(
		const std::string &className, const std::string &name,
		const std::string &doc)
	:ParameteredObject(className,name,doc) {

	ParameteredObject::_addParameter< int >(
		num_slots, "num_slots",
		"The number of virtual slots",
		0, "int");
	_setDynamic(true);
}

SlotBundle::~SlotBundle() {
	//_deleteAllSlots();
}

void SlotBundle::execute() {

}

void SlotBundle::initialize() {
}

void SlotBundle::finalize() {
}

int SlotBundle::size() {
	return _virtualOutputSlots.size();
}

void SlotBundle::_deleteAllSlots()
{
	setNumberOfVirtualSlots(0);
}

void SlotBundle::setNumberOfVirtualSlots(int num)
{

	//Temporary list of virtual slots
	std::vector<VirtualOutputSlot*> tempOutputSlots;
	std::vector<VirtualInputSlot*> tempInputSlots;

	//Keep smallest common number of slots
	int nslots=std::min(num,(int)_virtualOutputSlots.size());
	for(int i=0;i<nslots;i++)
	{
		tempInputSlots.push_back(_virtualInputSlots[i]);
		tempOutputSlots.push_back(_virtualOutputSlots[i]);
	}

	//Add more slots if needed
	for(int i=nslots;i<num;i++)
	{
		VirtualOutputSlot* out=new VirtualOutputSlot(i);
		VirtualInputSlot* in= new VirtualInputSlot(i);
		out->setVirtualPartnerSlot(in);
		tempInputSlots.push_back(in);
		tempOutputSlots.push_back(out);
	}

	//Remove slots if _virtualOutputSlots.size()> num
	for(int i=num;i<_virtualOutputSlots.size();i++)
	{
		_removeSlot(i);
		((VirtualOutputSlot*)_virtualOutputSlots[i])->disconnect();
		((VirtualInputSlot*)_virtualInputSlots[i])->disconnect();
		delete _virtualOutputSlots[i];
		delete _virtualInputSlots[i];
	}

	//Remember new slotlist
	_virtualInputSlots=tempInputSlots;
	_virtualOutputSlots=tempOutputSlots;

	//Eventually add new slots to internal interface
	for(int i=nslots;i<num;i++)
		_addSlot(i);




}

void SlotBundle::loadConnection(ParameterFile pf, PluginManagerInterface* man)
{
//	_load(pf,man);
	ParameteredObject::loadParameters(pf);
	ParameteredObject::loadSlots(pf,man);
}

void SlotBundle::prepareDynamicInterface(const ParameterFile &file)
{
	num_slots.load(file);

	std::cout<<"num slots "<<num_slots()<<std::endl;
	setNumberOfVirtualSlots(num_slots());
}
