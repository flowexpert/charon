/*  This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
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
#include <charon-core/SplitStream.h>
#include <charon-core/StringTool.h>
#include <charon-core/PluginManagerInterface.h>
#include <charon-core/DataManagerParameterFile.hxx>

ParameteredObject::ParameteredObject(const std::string& className,
		const std::string& name, const std::string& description) :
	_createMetadata(true),
	_className(className), _instanceName(name),
	_initialized(false), _executed(false) {

	// avoid empty instance name, use dummy value if needed
	if (_instanceName == "") {
		_instanceName = _className + "0";
	}

	// forbid invalid descriptions
	if (description.find("\n") != std::string::npos) {
		raise("do not use newlines in descriptions! (replace e.g. by <br>)");
	}

	if (_createMetadata) {
		_metadata.set<std::string> (_className + ".doc", description);
		_metadata.set<std::string> (_className + ".parameters");
		_metadata.set<std::string> (_className + ".inputs");
		_metadata.set<std::string> (_className + ".outputs");
	}

	_setDynamic(false);

	// additional Active
	_addParameter(_active, "active", 
		"If this is set to false,"
		"the plugin and all following ones are not executed",
		true, "bool");
}

ParameteredObject::~ParameteredObject() {
	if(_initialized)
		finalize();
}

const ParameterFile& ParameteredObject::getMetadata() {
	if (!_createMetadata) {
		sout << "(WW) requesting metadata, "
			"but metadata cache has been cleared." << std::endl;
	}
	return _metadata;
}

bool ParameteredObject::_addSomething(const std::string& extension,
		const std::string& name, const std::string& doc,
		const std::string& type, const std::string& defaultValue) {
	std::string nameL = name;
	std::transform(
		nameL.begin(), nameL.end(), nameL.begin(), (int(*)(int)) tolower);

	// forbid invalid docstrings
	if (doc.find("\n") != std::string::npos) {
		raise("do not use newlines in docstrings! (replace e.g. by <br>)");
	}

	// Check that param is not yet registered.
	// Parameters can only be assigned once!
	if( _parameters.find(name) != _parameters.end() ||
		_inputs.find(name) != _inputs.end() ||
		_outputs.find(name) != _outputs.end() ||
		_parameters.find(nameL) != _parameters.end() ||
		_inputs.find(nameL) != _inputs.end() ||
		_outputs.find(nameL) != _outputs.end()) {
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
			someList = _metadata.getList<std::string> (
					_className + "." + extension);
		}

		someList.push_back(name);
		_metadata.set<std::string>(_className+"."+extension, someList);
		_metadata.set<std::string>(_className+"."+name + ".type", type);
		_metadata.set<std::string>(_className+"."+name + ".doc", doc);
		if (defaultValue.length()) {
			_metadata.set<std::string> (_className + "." + name, defaultValue);
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
	_addInputSlot(slot,name,name,doc,type);
}

void ParameteredObject::_addInputSlot(Slot &slot, const std::string &name,const std::string &displayname, const std::string &doc, const std::string &type)
{
	// if type is given, we do not need guessing
	std::string guessedType = type;
	if (!guessedType.size())
		guessedType = slot.guessType();

	// assign parameter to this object
	slot.init(this, name,displayname, guessedType);

	// add metadata
	if (_addSomething("inputs", name, doc, guessedType)) {
		// and add it to the inputs slot list
		_inputs.insert(std::make_pair(name, &slot));

		if (_createMetadata) {
			_metadata.set<std::string> (_className + "." + name
					+ ".displayname", slot.getDisplayName());
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
	_addOutputSlot(slot,name,name,doc,type);
}

void ParameteredObject::_addOutputSlot(Slot& slot, const std::string& name,  const std::string& displayname,
		const std::string& doc, const std::string& type) {
	// if type is given, we do not need guessing
	std::string guessedType = type;
	if (!guessedType.size())
		guessedType = slot.guessType();

	// assign parameter to this object
	slot.init(this, name,displayname, guessedType);

	// add metadata
	if (_addSomething("outputs", name, doc, guessedType)) {
		// and add it to the output slot list
		_outputs.insert(std::make_pair(name, &slot));
		if (_createMetadata) {
			_metadata.set<std::string> (_className + "." + name
					+ ".displayname", slot.getDisplayName());
			if (!slot.getMulti())
				_metadata.set<std::string> (_className + "." + name
						+ ".multi", "false");
			if (!slot.getOptional())
				_metadata.set<std::string> (_className + "." + name
						+ ".optional", "false");
		}
	}
}

void ParameteredObject::_setTags(const std::string& tags) {
	if(_createMetadata) {
		_metadata.set<std::string> (_className + ".tags", tags);
	}
}


void ParameteredObject::run() {
	// avoid duplicate execution
	if (_executed) {
		sout << "(DD) Skipping reexecution of " << this->getClassName()
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

void ParameteredObject::runPreceeding() const {
	// collect preceeding objects
	std::set<ParameteredObject*> targetObjects;
	std::map<std::string, Slot*>::const_iterator it;
	for (it = _inputs.begin(); it != _inputs.end(); it++) {
		const std::set<Slot*>& ts = it->second->getTargets();
		std::set<Slot*>::const_iterator ti = ts.begin();
		for (; ti!=ts.end();ti++) {
			targetObjects.insert(&(*ti)->getParent());
		}
	}

	// run all preceeding objects
	std::set<ParameteredObject*>::iterator curObj = targetObjects.begin();
	for (; curObj != targetObjects.end(); curObj++) {
		if (*curObj) {
			(*curObj)->run();
		}
	}
}

void ParameteredObject::runPreceeding(const Slot& slot) const {
	// collect preceeding objects
	std::set<ParameteredObject*> targetObjects;
	const std::set<Slot*>& ts = slot.getTargets();
	std::set<Slot*>::const_iterator ti = ts.begin();
	for (; ti!=ts.end();ti++) {
		targetObjects.insert(&(*ti)->getParent());
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
	if (_executed) {
		sout << "(II) resetting execution state of " << getClassName()
			 << " \"" << getName() << "\"" << std::endl;
	}
    setExecuted(false);
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
	{
		Slot* sl=slotIter->second;
		if(sl->connected())
			sl->save(pf);
	}

	for (slotIter = _outputs.begin(); slotIter != _outputs.end(); slotIter++)
	{
		Slot* sl=slotIter->second;
		if(sl->connected())
			sl->save(pf);
	}
	onSave(pf);
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

void ParameteredObject::loadSlots(const ParameterFile& pf,
		const PluginManagerInterface * man) {
	// load slot connections
	std::map<std::string, Slot*>::const_iterator slotIter;
	for (slotIter = _inputs.begin(); slotIter != _inputs.end(); slotIter++)
		slotIter->second->load(pf, man);
	for (slotIter = _outputs.begin(); slotIter != _outputs.end(); slotIter++)
		slotIter->second->load(pf, man);


	    onLoad(pf,man);
}

bool ParameteredObject::connected() const {
	std::map<std::string, Slot*>::const_iterator slotIter;
	bool res = true;
	for (slotIter = _inputs.begin(); slotIter != _inputs.end(); slotIter++) {
		const Slot* cur = slotIter->second;
		if (!cur->getOptional() && !cur->connected()) {
			sout << "(WW) unconnected: " << cur->getParent().getName()
					<< "." << cur->getName() << std::endl;
			res = false;
		}
	}
	for (slotIter = _outputs.begin(); slotIter != _outputs.end(); slotIter++) {
		const Slot* cur = slotIter->second;
		if (!cur->getOptional() && !cur->connected()) {
			sout << "(WW) unconnected: " << cur->getParent().getName()
					<< "." << cur->getName() << std::endl;
			res = false;
		}
	}
	return res;
}

Slot* ParameteredObject::getSlot(const std::string& slotName) const {
	std::map<std::string, Slot*>::const_iterator slot;
	// try input slot
	slot = _inputs.find(slotName);
	if (slot != _inputs.end()) {
		return slot->second;
	}
	// try output slot
	slot = _outputs.find(slotName);
	if (slot != _outputs.end()) {
		return slot->second;
	}

	// try to fix slot casing
	// this is more expensive than simple map lookup,
	// so this is done only if needed
	std::string slotNameFixed = fixCase(slotName);
	sout << "(WW) getSlot: wrong slot casing: " << _instanceName
		 << "." << slotName  << " (fixed: " << slotNameFixed << ")"
		 << std::endl;

	// try again with fixed name
	slot = _inputs.find(slotNameFixed);
	if (slot != _inputs.end()) {
		return slot->second;
	}
	slot = _outputs.find(slotNameFixed);
	if (slot != _outputs.end()) {
		return slot->second;
	}

	// if everything else fails
	throw std::invalid_argument(
		std::string("Slot \"") + slotName + "\" in instance \""
			+ getName() + "\" of plugin \"" + getClassName()
				+ "\" not found!");
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

void ParameteredObject::clearMetadata() {
	_createMetadata = false;
	_metadata.clear();
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

void ParameteredObject::prepareDynamicInterface(const ParameterFile&) {
	initialize();
}

void ParameteredObject::_setDynamic(bool v) {
	if (_createMetadata) {
		_metadata.set(_className + ".isDynamicModule", v);
    }
}

bool ParameteredObject::isDynamic() {
	if (_createMetadata) {
		return _metadata.get<bool>(_className + ".isDynamicModule", false);
	}
	else {
		sout << "(WW) called isDynamic() without enabled metadata generation"
			 << std::endl;
		return false;
	}
}

std::string ParameteredObject::fixCase(const std::string& name) const {
	std::string nameL=name, curL;
	std::transform(
		nameL.begin(), nameL.end(), nameL.begin(),(int(*)(int)) tolower);
	std::set<std::string> names;
	std::map<std::string, Slot*>::const_iterator sIter;
	std::map<std::string, AbstractParameter*>::const_iterator pIter;
	for(sIter=_inputs.begin();sIter!=_inputs.end();sIter++) {
		names.insert(sIter->first);
	}
	for(sIter=_outputs.begin();sIter!=_outputs.end();sIter++) {
		names.insert(sIter->first);
	}
	for(pIter=_parameters.begin();pIter!=_parameters.end();pIter++) {
		names.insert(pIter->first);
	}
	std::set<std::string>::const_iterator nIter;
	for(nIter=names.begin(); nIter!=names.end(); nIter++) {
		curL=*nIter;
		std::transform(
			curL.begin(),curL.end(), curL.begin(),(int(*)(int)) tolower);
		if (nameL==curL) {
			return *nIter;
		}
	}
	raise("Parameter/Slot " + name + " not found!");
	return std::string();
}

void ParameteredObject::_removeInputSlot(std::string name)
{
	// remove metadata
	if (_removeSomething("inputs", name)) {
		// and remove it from the input slot list
		if(_inputs.find(name)!=_inputs.end()) {
			_inputs.erase(name);
		}
		if(_metadata.isSet(_className + "." + name+ ".multi")) {
			_metadata.erase(_className + "." + name+ ".multi");
		}
		if(_metadata.isSet(_className + "." + name+ ".optional")) {
			_metadata.erase(_className + "." + name+ ".optional");
		}
	}
}

bool ParameteredObject::_removeSomething(
		const std::string &extension, const std::string &name)
{
	// Check that param is  registered.
	// Parameters can only be assigned once!
	if( _parameters.find(name) == _parameters.end() ||
		_inputs.find(name) == _inputs.end() ||
		_outputs.find(name) == _outputs.end()) {
		sout << "(EE) ******************************************************\n"
			 << "(EE) The parameter or slot \"" << name
			 << "\" has not been defined!\n"
			 << "(EE) Slots and Parameter names must be unique "
			 << "for each Plugin!\n"
			 << "(EE) ******************************************************\n"
			 << std::endl;
		return false;
	}

	if (_createMetadata) {
		std::vector<std::string> someList;
		// get all elements of the given section
		if (_metadata.isSet(_className + "." + extension)) {
			someList = _metadata.getList<std::string> (_className + "."
					+ extension);
		}
		std::vector<std::string>::iterator found = std::find(
				someList.begin(), someList.end(), name);
		if (found != someList.end()) {
			if(_metadata.isSet(_className+"."+name + ".type")) {
				_metadata.erase(_className+"."+name + ".type");
			}
			if(_metadata.isSet(_className+"."+name + ".doc")) {
				_metadata.erase(_className+"."+name + ".doc");
			}
			if(_metadata.isSet(_className + "." + name)) {
				_metadata.erase(_className + "." + name);
			}
			if(_metadata.isSet(_className+"."+extension)) {
				_metadata.erase(_className+"."+extension);
			}
			someList.erase(found);
			_metadata.set<std::string>(_className+"."+extension, someList);
		}
	}

	return true;
}

void ParameteredObject::_removeOutputSlot(std::string name) {
	// remove metadata
	if (_removeSomething("outputs", name)) {
		// and remove it from the output slot list
		if(_outputs.find(name)!=_outputs.end()) {
			_outputs.erase(name);
		}
		if(_metadata.isSet(_className + "." + name+ ".multi")) {
			_metadata.erase(_className + "." + name+ ".multi");
		}
		if(_metadata.isSet(_className + "." + name+ ".optional")) {
			_metadata.erase(_className + "." + name+ ".optional");
		}
	}
}

void ParameteredObject::initialize() {
	if(this->_initialized)
		raise("Plugin is already initialized!");
	_initialized=true;
}

void ParameteredObject::finalize() {
	if(!_initialized)
		raise("Plugin was not initialied, or has been already finalized");
	_initialized=false;
}

void ParameteredObject::onLoad(
		const ParameterFile&, const PluginManagerInterface* man) {
	if(man->initializePluginOnLoad()) {
		initialize();
	}
}

void ParameteredObject::onSave(ParameterFile&) const {
}
