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
/** \file Slots.cpp
 *  Implementation of non-templated Slot methods.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 11.01.2011
 */

#include "../include/charon-core/Slots.hxx"

// ==========================   class Slot   ================================

Slot::Slot() :
	_parent(0) {
}

void Slot::init(ParameteredObject* parent, std::string name,
		std::string type) {

	assert(parent); // parent valid
	_parent = parent;
	_name = name;
	_type = StringTool::toLowerCase(type);
}

Slot::~Slot() {
}

ParameteredObject& Slot::getParent() {
	return *_parent;
}

const ParameteredObject& Slot::getParent() const {
	return *_parent;
}

std::string Slot::getName() const {
	return _name;
}

bool Slot::getMulti() const {
	return _multiSlot;
}

bool Slot::getOptional() const {
	return _optional;
}

bool Slot::connected() {
	return (getTargets().size() > 0);
}

bool Slot::connected(Slot& target) {
	std::set<Slot *> targets = getTargets();
	return (targets.find(&target) != targets.end());
}

bool Slot::connect(Slot& target) {
	if (!&target) {
		raise("Slot::connect(): invalid target slot given (null)");
	}
	bool ret = true;
	ret &= _addTarget(&target);
	ret &= target._addTarget(this);
	return ret;
}

bool Slot::disconnect() {
	bool res = true;
	std::set<Slot*>::iterator tIter;
	std::set<Slot*> targets = getTargets();
	for (tIter = targets.begin(); tIter != targets.end(); tIter++) {
		res &= (*tIter)->_removeTarget(this);
		res &= _removeTarget(*tIter);
	}
	return res;
}

bool Slot::disconnect(Slot& target) {
	std::set<Slot*> targets = getTargets();
	if (targets.find(&target) == targets.end())
		return false;
	bool res = true;
	res &= target._removeTarget(this);
	res &= _removeTarget(&target);
	return res;
}

void Slot::save(ParameterFile& /*pf*/) const {
	//Does nothing, erasing the parameter file is not required
}

void Slot::printInfo(const std::string& msg) const {
	sout << "(II) Slot \"" << _parent->getName() << "." << _name
		<< "\": " << msg << std::endl;
}

void Slot::printWarning(const std::string& msg) const {
	sout << "(WW) Slot \"" << _parent->getName() << "." << _name
		<< "\": " << msg << std::endl;
}

void Slot::printError(const std::string& msg) const {
	sout << "(EE) Slot \"" << _parent->getName() << "." << _name
		<< "\" (type " << getType() << "): " << msg << std::endl;
}

void Slot::raise(const std::string& msg) const {
	throw std::runtime_error(
				"Error in Slot \"" + _parent->getName() + "." + _name
				+ "\" (type " + getType() + "): "
		+ msg);
}

VirtualSlot::VirtualSlot(std::string virtType,int num)

{
    this->_type="Virtual";

    std::stringstream vname;
    vname<<"Virtual"<<virtType<<num;
    _virtualNum=vname.str();
    this->_name=_virtualNum;
    _partner=0;


    _optional=true;


}

bool VirtualSlot::_addTarget(Slot *target)
{
    assert(target);

    if(sl)
    {
	if(_target.size()>0)
	    return false;

	_name=sl->getName();
	_type=sl->getType();


	_partner->setNameAndType(sl->getName(),sl->getType());
	_target.insert(target);
	onAddTarget(target);
    }
    else
	return false;

    return true;


}

std::string VirtualSlot::getType() const
{
    return _type;
}

std::string VirtualSlot::getName() const
{
    return _name;
}

const std::string& VirtualOutputSlot::getConfig() const
{
    return _managerconfig;
}

bool VirtualSlot::_removeTarget(Slot *target)
{

    if(target==*(_target.begin()))
    {

	this->_type="Virtual";
	this->_name=_virtualNum;

	_target.clear();
	onRemoveTarget(target);

    }

    return true;

}

void VirtualSlot::save(ParameterFile &pf) const
{
    Slot::save(pf);




    if(_target.size()>0)
    {

	std::string target=*(_target.begin())->getParent().getName() + "."+ *(_target.begin())->getName();
	pf.set<std::string> (_parent->getName() + "." + _virtualNum+".outslot", target);
	pf.set<std::string> (_parent->getName() + "." + _virtualNum+".config", _managerconfig);
    }
    else
    {
	if (pf.isSet(_parent->getName() + "." + _virtualNum+".outslot"))
	    pf.erase(_parent->getName() + "." + _virtualNum+".outslot");

	if (pf.isSet(_parent->getName() + "." + _virtualNum+".name"))
	    pf.erase(_parent->getName() + "." + _virtualNum+".name");
	if (pf.isSet(_parent->getName() + "." + _virtualNum+".type"))
	    pf.erase(_parent->getName() + "." + _virtualNum+".type");

    }


    pf.set<std::string> (_parent->getName() + "." + _virtualNum+".name", _name);
    pf.set<std::string> (_parent->getName() + "." + _virtualNum+".type", _type);


}

OutputSlotIntf * VirtualOutputSlot::getThisPointer()
{
    if(_partner)
    {
	if(_partner->getTargets().size())
	{
	    OutputSlotIntf* sl=dynamic_cast<OutputSlotIntf*>(*(_partner->getTargets().begin()));
	    return sl;
	}
	Slot::raise(_partner->getName()+" not connected!!");
    }
    else
    {
	return this;
    }
}

void VirtualSlot::load(const ParameterFile &pf, const PluginManagerInterface *man)
{
    std::string outsl=pf.get<std::string>(_parent->getName() + "." + _virtualNum+".outslot");
    //std::string insl=pf.get<std::string>(_parent->getName() + "." + _virtualNum+".inslot");

    OutputSlotIntf* sl=dynamic_cast<OutputSlotIntf*>(man->getInstance(outsl));
    assert(sl);
    _target.insert(dynamic_cast<Slot*>(sl));

    _name=pf.get<std::string>(_parent->getName() + "." + _virtualNum+".name");
    _type=pf.get<std::string>(_parent->getName() + "." + _virtualNum+".type");
    _managerconfig=pf.get<std::string>(_parent->getName() + "." + _virtualNum+".config");
}

void VirtualSlot::setVirtualPartnerSlot(VirtualSlot *insl)
{
    if(_partner!=insl)
    {
	if(isValidPartner(insl))
	{
	    _partner=insl;
	    insl->setVirtualPartnerSlot(this);
	}
	else
	    Slot::raise("Invalid VirtualSlot pairing!!");
    }
}


void VirtualOutputSlot::setCacheType(Slot::CacheType type)
{
    assert(_target.size());
    *(_target.begin())->setCacheType(type);
}

Slot::CacheType VirtualOutputSlot::getCacheType() const
{
    assert(_target.size());
    return *(_target.begin())->getCacheType();
}

void VirtualSlot::prepare()
{
}

void VirtualSlot::finalize()
{
}

std::string VirtualSlot::guessType() const
{
    return getType();
}

std::set<Slot *> VirtualSlot::getTargets() const
{
    return _target;
}




VirtualInputSlot::VirtualInputSlot(int num)
    :VirtualSlot("InputSlot",0)
{

}

bool VirtualInputSlot::_addTarget(Slot *target)
{
    assert(target);
    InputSlotIntf* sl=dynamic_cast<InputSlotIntf*>(target);
    if(sl)
    {
	if(_slot.size()>0)
	    return false;

	_name=sl->getName();
	_type=sl->getType();


	_slot.insert(target);
    }
    else
	return false;

    return true;


}

std::string VirtualInputSlot::getType() const
{
    return _type;
}

std::string VirtualInputSlot::getName() const
{
    return _name;
}




VirtualOutputSlot::VirtualOutputSlot(int num)
    :VirtualSlot("OutputSlot",0)
{
}

bool VirtualOutputSlot::isValidPartner(VirtualSlot *insl)
{
    return dynamic_cast<VirtualInputSlot*>(insl);
}

bool VirtualInputSlot::isValidPartner(VirtualSlot *insl)
{
    return dynamic_cast<VirtualOutputSlot*>(insl);
}

void VirtualSlot::setNameAndType(std::string name, std::string type)
{
    _name=name;
    _type=type;
}

void VirtualSlot::onAddTarget(Slot *target)
{
}

void VirtualSlot::onRemoveTarget(Slot *target)
{
}

void VirtualInputSlot::onAddTarget(Slot *target)
{
    OutputSlotIntf* sl=dynamic_cast<OutputSlotIntf*>(target);
    if(sl)
    {
	_managerconfig=sl->getConfig();
    }
}

void VirtualInputSlot::onRemoveTarget(Slot *target)
{
    OutputSlotIntf* sl=dynamic_cast<OutputSlotIntf*>(target);
    if(sl)
    {
	_managerconfig="";
    }
}








