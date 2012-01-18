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
/** \file ParameteredObject.cpp
 *  Implementation of class ParameteredObject.
 *  \author <a href="mailto:daniel.kondermann@iwr.uni-heidelberg.de">
 *      Daniel Kondermann</a>
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \author <a href="mailto:stephan-meister@arcor.de">Stephan Meister</a>
 *  \author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
 *
 *  Based on a file written by Daniel Kondermann, modified by Jens-Malte
 *  Gottfried and Stephan Meister.
 *
 *  \date 11.12.2008
 */

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <charon-core/ParameteredObject.hxx>
#include <charon-core/PluginManagerInterface.h>

// Instantiate static variables.
std::map<std::string, unsigned int> ParameteredObject::_genericClassNameCount;
bool ParameteredObject::_createMetadata = false;

std::string ParameteredObject::_genericName() {
	ParameteredObject::_genericClassNameCount[_className]++;
	std::stringstream str;
	str << _className << ParameteredObject::_genericClassNameCount[_className];
	return str.str();
}

ParameteredObject::ParameteredObject(const std::string& className,
		const std::string& name, const std::string& description) :
	_className(className), _instanceName(name), _executed(false) {

	// use autogenerated (unique) name if given name is empty
	if (!name.length())
		_instanceName = _genericName();

	if (_createMetadata) {
		_metadata.set<std::string> (_className + ".doc", description);
		_metadata.set<std::string> (_className + ".parameters");
		_metadata.set<std::string> (_className + ".inputs");
		_metadata.set<std::string> (_className + ".outputs");
	}
        _initialized=false;
}

ParameteredObject::~ParameteredObject() {
	if (_createMetadata) {
		_metadata.save(
#ifdef UNIX
				"lib" +
#endif
						_className + ".wrp");
	}
        if(_initialized)
            finalize();
}

bool ParameteredObject::_addSomething(const std::string& extension,
		const std::string& name, const std::string& doc,
		const std::string& type, const std::string& defaultValue) {

	// Check that param is not yet registered.
	// Parameters can only be assigned once!
	if( _parameters.find(name) != _parameters.end() ||
		_inputs.find(name) != _inputs.end() ||
		_outputs.find(name) != _outputs.end()) {
		sout << "(EE) ******************************************************\n"
			 << "(EE) The parameter or slot \"" << name
			 << "\" has already been defined!\n"
			 << "(EE) Slots and Parameter names must be unique "
			 << "for each Plugin!\n"
			 << "(EE) ******************************************************\n"
			 << std::endl;
		return false;
	}

	if (_createMetadata) {
		// Here, type has to be set explicitly.
		// Use guessType, if no type has been given.
		assert(type.length());

		std::vector<std::string> someList;
		// get all elements of the given section
		if (_metadata.isSet(_className + "." + extension)) {
			someList = _metadata.getList<std::string> (_className + "."
					+ extension);
		}

		std::vector<std::string>::const_iterator found = std::find(
				someList.begin(), someList.end(), name);

		if (found == someList.end()) {
			someList.push_back(name);
			_metadata.set<std::string>(_className+"."+extension, someList);
			_metadata.set<std::string>(_className+"."+name + ".type", type);
			_metadata.set<std::string>(_className+"."+name + ".doc", doc);
			if (defaultValue.length())
				_metadata.set<std::string> (_className + "." + name,
						defaultValue);
		}
	}

	return true;
}

void ParameteredObject::_addParameter(AbstractParameter& param,
		const std::string& name, const std::string& doc,
		const std::string& type) {
	// assign parameter to this object
	param.init(this, name);

	// if type is given, we do not need guessing
	std::string guessedType = type;
	if (!guessedType.size())
		guessedType = param.guessType();

	// add metadata
	if (_addSomething("parameters", name, doc, guessedType,
			param.getDefaultString())) {
		// and add it to the parameters list
		_parameters.insert(std::make_pair(name, &param));
	}
}

void ParameteredObject::_addInputSlot(Slot& slot, const std::string& name,
		const std::string& doc, const std::string& type) {
	// if type is given, we do not need guessing
	std::string guessedType = type;
	if (!guessedType.size())
		guessedType = slot.guessType();

	// assign parameter to this object
	slot.init(this, name, guessedType);

	// add metadata
	if (_addSomething("inputs", name, doc, guessedType)) {
		// and add it to the inputs slot list
		_inputs.insert(std::make_pair(name, &slot));

		if (_createMetadata) {
			if (slot.getMulti())
				_metadata.set<std::string> (_className + "." + name
						+ ".multi", "true");
			if (slot.getOptional())
				_metadata.set<std::string> (_className + "." + name
						+ ".optional", "true");
		}
	}
}

void ParameteredObject::_addOutputSlot(Slot& slot, const std::string& name,
		const std::string& doc, const std::string& type) {
	// if type is given, we do not need guessing
	std::string guessedType = type;
	if (!guessedType.size())
		guessedType = slot.guessType();

	// assign parameter to this object
	slot.init(this, name, guessedType);

	// add metadata
	if (_addSomething("outputs", name, doc, guessedType)) {
		// and add it to the output slot list
		_outputs.insert(std::make_pair(name, &slot));
		if (_createMetadata) {
			if (!slot.getMulti())
				_metadata.set<std::string> (_className + "." + name
						+ ".multi", "false");
			if (!slot.getOptional())
				_metadata.set<std::string> (_className + "." + name
						+ ".optional", "false");
		}
	}
}

void ParameteredObject::run() {
	// avoid duplicate execution
	if (_executed) {
		sout << "(II) Skipping reexecution of " << this->getClassName()
			<< " \"" << this->getName() << "\"" << std::endl;
		return;
	}

	// check requirements
	if (!connected()) {
		ParameteredObject::raise("not (completely) connected!");
	}

	runPreceeding();

	// now all inputs are ready, execute the current object
	// by calling the template function
	sout << "(II) Executing " << getClassName() << " \"";
	sout << getName() << "\"" << std::endl;
	_prepareSlots();
	execute();
	_commitSlots();

	// post execution code
	_executed = true;
}

void ParameteredObject::runPreceeding() {
	// collect preceeding objects
	std::set<ParameteredObject*> targetObjects;
	for (std::map<std::string, Slot*>::iterator it = _inputs.begin(); it
			!= _inputs.end(); it++) {
		const std::set<Slot*>& ts = it->second->getTargets();
		std::set<Slot*>::const_iterator ti = ts.begin();
		for (; ti!=ts.end();ti++) {
			targetObjects.insert(&(*ti)->getParent());
		}
	}

	// run all preceeding objects
	std::set<ParameteredObject*>::iterator curObj = targetObjects.begin();
	for (; curObj != targetObjects.end(); curObj++) {
		(*curObj)->run();
	}
}

void ParameteredObject::execute() {
    if(!_initialized)
        raise("This plugin must be initialized before execution!");
	// empty default implementation
	sout << "(WW) this plugin has not overridden execute() or does call "
		 << "ParameteredObject::execute() directly which is deprecated."
		 << std::endl;
}

void ParameteredObject::_prepareSlots() {
	std::map<std::string, Slot*>::const_iterator cur;
	for (cur = _inputs.begin(); cur != _inputs.end(); cur++) {
		cur->second->prepare();
	}
	for (cur = _outputs.begin(); cur != _outputs.end(); cur++) {
		cur->second->prepare();
	}
}

void ParameteredObject::_commitSlots() {
	std::map<std::string, Slot*>::const_iterator cur;
	for (cur = _inputs.begin(); cur != _inputs.end(); cur++) {
		cur->second->finalize();
	}
	for (cur = _outputs.begin(); cur != _outputs.end(); cur++) {
		cur->second->finalize();
	}
}

void ParameteredObject::resetExecuted(bool propagate) {
	_executed = false;
	if (propagate) {
		for (std::map<std::string, Slot*>::iterator it = _outputs.begin();
				it != _outputs.end(); it++) {
			std::set<Slot*> targets = it->second->getTargets();
			std::set<Slot*>::iterator tIt;
			for(tIt = targets.begin(); tIt != targets.end(); tIt ++)
				(*tIt)->getParent().resetExecuted(propagate);
		}
	}
}

std::set<ParameteredObject*> ParameteredObject::_getTargetNodes() {
	std::set<ParameteredObject*> res;
	std::set<ParameteredObject*> children;
	std::map<std::string, Slot*>::const_iterator outIter;
	std::set<Slot*>::const_iterator tIter;
	for (outIter = _outputs.begin(); outIter != _outputs.end(); outIter++) {
		const std::set<Slot*>& targets = outIter->second->getTargets();
		for(tIter = targets.begin(); tIter != targets.end(); tIter++)
			children.insert(&((*tIter)->getParent()));
	}
	if(children.size() > 0) {
		std::set<ParameteredObject*>::const_iterator cIter;
		for (cIter = children.begin(); cIter != children.end(); cIter++) {
			const std::set<ParameteredObject*>& targets =
					(*cIter)->_getTargetNodes();
			res.insert(targets.begin(), targets.end());
		}
	}
	else
		res.insert(this);
	return res;
}

std::set<std::string> ParameteredObject::getNeighbours() const {
	std::set<std::string> res;
	std::map<std::string, Slot*>::const_iterator slotIter;
	std::set<Slot*> targets;
	std::set<Slot*>::const_iterator targetIter;

	for (slotIter = _inputs.begin(); slotIter != _inputs.end(); slotIter++) {
		if (slotIter->second->connected()) {
			targets = slotIter->second->getTargets();
			for (targetIter = targets.begin();
					targetIter != targets.end(); targetIter++)
				res.insert((*targetIter)->getParent()._instanceName);
		}
	}
	for (slotIter = _outputs.begin(); slotIter != _outputs.end(); slotIter++) {
		if (slotIter->second->connected()) {
			targets = slotIter->second->getTargets();
			for (targetIter = targets.begin();
					targetIter != targets.end(); targetIter++)
				res.insert((*targetIter)->getParent()._instanceName);
		}
	}
	return res;
}

std::set<std::string> ParameteredObject::getNeighbours(
		const ParameterFile& pf) const {
	std::set<std::string> res;
	std::map<std::string, Slot*>::const_iterator slotIter;
	std::set<std::string> slotNames;
	std::set<std::string>::const_iterator slot, resItem;
	std::vector<std::string> targetList;
	std::vector<std::string>::const_iterator target;

	// find names of the connected objects
	for (slotIter = _inputs.begin(); slotIter != _inputs.end(); slotIter++)
		slotNames.insert(slotIter->second->getName());
	for (slotIter = _outputs.begin(); slotIter != _outputs.end(); slotIter++)
		slotNames.insert(slotIter->second->getName());
	for (slot = slotNames.begin(); slot != slotNames.end(); slot++) {
		// skip unset parameters (not connected)
		if (!pf.isSet(_instanceName + "." + *slot))
			continue;
		// add targets of the set ones
		targetList = pf.getList<std::string> (_instanceName + "." + *slot);
		for (target = targetList.begin(); target != targetList.end(); target++)
			res.insert(target->substr(0, target->find(".")));
	}

	// make sure that these objects exist and have the correct type
	for (resItem = res.begin(); resItem != res.end(); resItem++) {
		std::string type = pf.get<std::string> (*resItem + ".type");
		//getInstance(type, *resItem);
	}

	return res;
}

void ParameteredObject::saveParameters(ParameterFile& pf) const {
	// set _instanceName.type
	if (pf.isSet(_instanceName + ".type")) {
		assert(pf.get<std::string>(_instanceName + ".type")
				== _className);
	} else
		pf.set<std::string> (_instanceName + ".type", _className);

	// save parameters
	std::map<std::string, AbstractParameter*>::const_iterator par;
	for (par = _parameters.begin(); par != _parameters.end(); par++)
		par->second->save(pf);
}

void ParameteredObject::save(ParameterFile& pf) const {
	// save parameters
	saveParameters(pf);

	// save slots
	std::map<std::string, Slot*>::const_iterator slotIter;

	for (slotIter = _inputs.begin(); slotIter != _inputs.end(); slotIter++)
		slotIter->second->save(pf);

	for (slotIter = _outputs.begin(); slotIter != _outputs.end(); slotIter++)
		slotIter->second->save(pf);
}

void ParameteredObject::loadParameters(const ParameterFile& pf) {
	// check type
	assert(pf.isSet(_instanceName + ".type"));
#ifndef NDEBUG
	if(StringTool::toLowerCase(pf.get<std::string>(_instanceName + ".type"))
		!= StringTool::toLowerCase(_className)) {
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__;
		msg << "\n\tclassname and type do not match!";
		msg << "\n\tclassname: " << _className;
		msg << "\n\tinstance name: " << _instanceName;
		msg << "\n\ttype: " << pf.get<std::string>(_instanceName + ".type");
		throw std::runtime_error(msg.str().c_str());
	}
#endif

	// load parameters
	std::map<std::string, AbstractParameter*>::const_iterator par;
	for (par = _parameters.begin(); par != _parameters.end(); par++)
		par->second->load(pf);
}

void ParameteredObject::_load(const ParameterFile& pf,
		const PluginManagerInterface * man) {
	// load parameters
	loadParameters(pf);

	// load slot connections
	std::map<std::string, Slot*>::const_iterator slotIter;
	for (slotIter = _inputs.begin(); slotIter != _inputs.end(); slotIter++)
		slotIter->second->load(pf, man);
	for (slotIter = _outputs.begin(); slotIter != _outputs.end(); slotIter++)
		slotIter->second->load(pf, man);

	// initialize object
	initialize();
}

bool ParameteredObject::connected() const {
	std::map<std::string, Slot*>::const_iterator slotIter;
	bool res = true;
	for (slotIter = _inputs.begin(); slotIter != _inputs.end(); slotIter++)
		res = res && (slotIter->second->getOptional()
				|| slotIter->second->connected());
	for (slotIter = _outputs.begin(); slotIter != _outputs.end(); slotIter++)
		res = res && (slotIter->second->getOptional()
				|| slotIter->second->connected());
	return res;
}

bool ParameteredObject::_connect(ParameteredObject* target,
		const std::string& ownSlotStr, const std::string& targetSlotStr) {
	std::map<std::string, Slot*>::const_iterator slotIter;
	bool ownIsIn; // true if ownSlot is an input slot
	slotIter = _inputs.find(ownSlotStr);
	ownIsIn = (slotIter != _inputs.end());
	if (!ownIsIn) {
		slotIter = _outputs.find(ownSlotStr);
		assert(slotIter != _outputs.end());
	}
	Slot* ownSlot = slotIter->second;

	if (ownIsIn) {
		slotIter = target->_outputs.find(targetSlotStr);
		assert(slotIter != target->_outputs.end());
	} else {
		slotIter = target->_inputs.find(targetSlotStr);
		assert(slotIter != target->_inputs.end());
	}
	Slot* targetSlot = slotIter->second;

	// add target slot to target ist of own slot
	return ownSlot->connect(*targetSlot);
}

bool ParameteredObject::_disconnect(ParameteredObject* target,
		const std::string& ownSlotStr, const std::string& targetSlotStr) {
	std::map<std::string, Slot*>::const_iterator slotIter;
	bool ownIsIn; // true if ownSlot is an input slot
	slotIter = _inputs.find(ownSlotStr);
	ownIsIn = (slotIter != _inputs.end());
	if (!ownIsIn) {
		slotIter = _outputs.find(ownSlotStr);
		assert(slotIter != _outputs.end());
	}
	Slot* ownSlot = slotIter->second;

	if (ownIsIn) {
		slotIter = target->_outputs.find(targetSlotStr);
		assert(slotIter != target->_outputs.end());
	} else {
		slotIter = target->_inputs.find(targetSlotStr);
		assert(slotIter != target->_inputs.end());
	}
	Slot* targetSlot = slotIter->second;

	// remove target slot from target list of own slot
	return ownSlot->disconnect(*targetSlot);
}

Slot* ParameteredObject::getSlot(const std::string& slotName) {
	std::map<std::string, Slot*>::iterator slot;
	slot = _inputs.find(slotName);
	if (slot == _inputs.end()) {
		slot = _outputs.find(slotName);
		if (slot == _outputs.end())
			throw std::invalid_argument(
				std::string("Slot \"") + slotName + "\" in instance \""
					+ getName() + "\" of plugin \"" + getClassName()
					+ "\" not found!");
	}
	return slot->second;
}

const Slot* ParameteredObject::getSlot(const std::string& slotName) const {
	std::map<std::string, Slot*>::const_iterator slot;
	slot = _inputs.find(slotName);
	if (slot == _inputs.end()) {
		slot = _outputs.find(slotName);
		if (slot == _outputs.end())
			throw std::invalid_argument(
				std::string("Slot \"") + slotName + "\" in instance \""
					+ getName() + "\" of plugin \"" + getClassName()
					+ "\" not found!");
	}
	return slot->second;
}

const std::map<std::string, Slot*>&
		ParameteredObject::getInputSlots() const {
	return _inputs;
}

const std::map<std::string, Slot*>&
		ParameteredObject::getOutputSlots() const {
	return _outputs;
}

const std::map<std::string, AbstractParameter*>&
		ParameteredObject::getParameters() const {
	return _parameters;
}

void ParameteredObject::setCreateMetadata(bool c) {
	_createMetadata = c;
}

bool ParameteredObject::getCreateMetadata() {

	return _createMetadata;
}

void ParameteredObject::raise(const std::string& message) const
{
	std::string msg = this->getClassName() ;
	const std::string& templateType = this->getTemplateType() ;
	if(templateType.length() > 0)
		msg += "<" + templateType + ">" ;
	msg += " \"" + this->getName() + "\" : " + message ;

	throw std::runtime_error(msg) ;
}


AbstractParameter & ParameteredObject::getParameter(
		const std::string & name) const {
	if (_parameters.find(name) != _parameters.end()) {
		return *(_parameters.find(name)->second);
	} else {
		throw std::invalid_argument(
			std::string("Parameter ") + name + " in instance \""
					+ getName() + "\" of plugin \"" + getClassName()
					+ "\" does not exist!");
	}
}

const std::string ParameteredObject::getTemplateType() const {
	return "";
}

std::string ParameteredObject::templateTypeToString(template_type t) {
	switch (t) {
	case ParameteredObject::TYPE_DOUBLE:
		return "double";
	case ParameteredObject::TYPE_FLOAT:
		return "float";
	case ParameteredObject::TYPE_INT:
		return "int";
	default:
		return ParameteredObject::TYPE_DOUBLE;
	}
}



void ParameteredObject::initialize()
{
    if(this->_initialized)
        raise("Plugin is already initialized!");
    _initialized=true;
}

void ParameteredObject::finalize()
{
    if(!_initialized)
        raise("Plugin was not initialied!");
    _initialized=false;
}



/*
ParameteredObject::build_type ParameteredObject::getBuildType() const
{
	return ParameteredObject::UNDEFINED_BUILD ;
}
*/
