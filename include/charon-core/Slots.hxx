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
/** @file Slots.hxx
 *  Implementation of the abstract class Slot and template classes InputSlot
 *  and OutputSlot.
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
 *
 *  @date 03.04.2009
 *
 *  This file does contain the implementations of the classes
 *  - Slot
 *  - InputSlot
 *  - OutputSlot
 *
 *  In most cases, you want not to include this file but ParameteredObject.hxx
 */

#ifndef _SLOTS_HXX_
#define _SLOTS_HXX_

#include "ParameteredObject.h"
#include "PluginManagerInterface.h"
#include <stdexcept>

// ========================   class AbstractSlot   ==========================

template<class T>
AbstractSlot<T>::AbstractSlot() {
}

template<class T>
AbstractSlot<T>::~AbstractSlot() {
	if (connected())
		disconnect();
}

template<class T>
bool AbstractSlot<T>::_addTarget(Slot* target) {
	if(!target) {
		Slot::raise("AbstractSlot::_addTarget: null pointer given");
	}

	// Check slot types by comparing their _type strings.
	if ((target->getType() != this->getType())&&!((target->getType()=="Virtual")||(this->getType()=="Virtual"))) {
		this->printError("Source slot of failed connection");
		target->printError("Target slot of failed connection");
		Slot::raise("Cannot connect slots of different types.");
	}

	_targets.insert((AbstractSlot<T>*) target);
	if (!_multiSlot && _targets.size() > 1) {
		this->printError("Source slot of failed connection");
		target->printError("Target slot of failed connection");
		Slot::raise("Tried to add multiple targets to non-multi slot.");
	}
	return true;
}

template<class T>
bool AbstractSlot<T>::_removeTarget(Slot* target) {
	if(!target) {
		Slot::raise("AbstractSlot::_removeTarget: null pointer given");
	}
	// dynamic_cast isn't possible on dynamically loaded plugins.
	// AbstractSlot<T>* t = dynamic_cast<AbstractSlot<T>*>(target);
	return (_targets.erase((AbstractSlot<T>*) target) > 0);
}

template<class T>
std::string AbstractSlot<T>::guessType() const {
	return TypeDetector::instance().type(typeid(T).name());
}

template<class T>
std::set<Slot*> AbstractSlot<T>::getTargets() const {
	std::set<Slot*> res;
	res.insert(_targets.begin(), _targets.end());
	return res;
}

template<class T>
void AbstractSlot<T>::save(ParameterFile& pf) const {
	Slot::save(pf);
	std::vector<std::string> targetList;
	typename std::set<AbstractSlot<T>*>::const_iterator slot;
	for (slot = _targets.begin(); slot != _targets.end(); slot++)
		targetList.push_back((*slot)->getParent().getName() + "."
				+ (*slot)->getName());
	if (targetList.size())
		pf.set<std::string> (_parent->getName() + "." + _name, targetList);
	else if (pf.isSet(_parent->getName() + "." + _name))
		pf.erase(_parent->getName() + "." + _name);
}

template<typename T>
inline std::string AbstractSlot<T>::getType() const {
	std::string::size_type pos = _type.find("<t>");
	if (pos != std::string::npos) {
		std::string temp(this->_type);
		temp.replace(pos + 1, 1, getParent().getTemplateType());
		return temp;
	} else if (_type == "t") {
		return (getParent().getTemplateType());
	}
	return _type;
}

template<class T>
typename std::set<AbstractSlot<T>*>::const_iterator AbstractSlot<T>::begin() const {
	return _targets.begin();
}

template<class T>
typename std::set<AbstractSlot<T>*>::const_iterator AbstractSlot<T>::end() const {
	return _targets.end();
}

// =========================   class InputSlot   ============================

template<class T>
InputSlot<T>::InputSlot(bool optional, bool multi) {
	AbstractSlot<T>::_optional = optional;
	AbstractSlot<T>::_multiSlot = multi;
}

template<class T>
InputSlot<T>::~InputSlot() {
}

template<class T>
void InputSlot<T>::load(const ParameterFile& pf,
		const PluginManagerInterface * man) {
	// disconnect from other slots
	Slot::disconnect();

	if (!pf.isSet(Slot::_parent->getName() + "." + Slot::_name))
		// nothing to do
		return;

	// get all targets from parameter file
	std::vector<std::string> targetList = pf.getList<std::string> (
			Slot::_parent->getName() + "." + Slot::_name);

	// only multislots can be connected to more than one source!
	if (!(Slot::_multiSlot || (targetList.size() < 2))) {
		Slot::raise("multiple targets but not a multi-slot");
	}

	if (!targetList.size())
		// nothing to do
		return;

	typename std::vector<std::string>::const_iterator tStrIter;

	// and add corresponding slots to _targets
	for (tStrIter = targetList.begin(); tStrIter != targetList.end(); tStrIter++) {
		ParameteredObject* targObj = man->getInstance(tStrIter->substr(0,
				tStrIter->find(".")));

		Slot * targetSlot = targObj->getSlot(
					tStrIter->substr(tStrIter->find(".") + 1));

		// typecheck is performed in _addTarget that is called in connect
		AbstractSlot<T>* tmp = (AbstractSlot<T>*) (targetSlot);
		connect(*tmp);
	}
}

template<class T>
InputSlot<T>::operator T() const {
	if (!AbstractSlot<T>::_targets.size()) {
		Slot::raise("access to unconnected input slot (cast operator)");
	}
	return _getDataFromOutputSlot(*(AbstractSlot<T>::_targets.begin()));
}

template<class T>
const T& InputSlot<T>::operator()() const {
	if (!AbstractSlot<T>::_targets.size()) {
		Slot::raise("access to unconnected input slot (operator())");
	}
	return _getDataFromOutputSlot(*(AbstractSlot<T>::_targets.begin()));
}

template<class T>
const T& InputSlot<T>::operator[](std::size_t pos) const {
	if (pos >= AbstractSlot<T>::_targets.size()) {
		std::ostringstream err;
		err << "pos out of range: pos = " << pos;
		err << "; size = " << AbstractSlot<T>::_targets.size();
		throw std::out_of_range(err.str());
	}

	typename std::set<AbstractSlot<T>*>::const_iterator item;
	item = AbstractSlot<T>::_targets.begin();
	for (unsigned int i = 0; i < pos; i++)
		item++;
	return _getDataFromOutputSlot(*item);
}

template<typename T>
std::size_t InputSlot<T>::size() const {
	return AbstractSlot<T>::_targets.size();
}

template <typename T>
const T& InputSlot<T>::_getDataFromOutputSlot(
			const AbstractSlot<T>* slot) const
{
    // c-style cast can produce weird errors, so use dynamic cast to make sure
    // const OutputSlot<T>* source = (const OutputSlot<T>*) slot;
    const OutputSlot<T>* source = dynamic_cast<const OutputSlot<T>*>(slot);
    if (!source) {
	    Slot::raise("input connection data type mismatch (dynamic cast)");
    }
    if (source->getType() != this->getType()) {
	    Slot::raise("input connection type mismatch");
    }

    const Slot::CacheType& scType = source->getCacheType();
    switch (scType) {
    case Slot::CACHE_MEM:
	    return source->operator()();
    case Slot::CACHE_MANAGED: {
		    const std::string& config = source->getConfig();
		    typename std::map<std::string,T>::const_iterator found;
		    found = _dataCache.find(config);
		    if (found == _dataCache.end()) {
			    Slot::raise("data could not be retrieved (not cached)");
		    }
		    return found->second;
	    }
    case Slot::CACHE_INVALID:
	    Slot::raise("cannot read from output with cache type \"invalid\"");
    default:
	    Slot::raise("no implementation for given output slot cache type");
    }
    // this should never be reached
    throw std::runtime_error(
		    "unexpected error in InputSlot::_getDataFromOutputSlot");
}

template <typename T>
const T& InputSlot<T>::_getDataFromOutputSlot(
			const OutputSlotIntf* slot) const {

	// c-style cast can produce weird errors, so use dynamic cast to make sure
	// const OutputSlot<T>* source = (const OutputSlot<T>*) slot;
	const OutputSlot<T>* source = dynamic_cast<const OutputSlot<T>*>(slot->getThisPointer());
	if (!source) {
		Slot::raise("input connection data type mismatch (dynamic cast)");
	}
	if (source->getType() != this->getType()) {
		Slot::raise("input connection type mismatch");
	}

	const Slot::CacheType& scType = source->getCacheType();
	switch (scType) {
	case Slot::CACHE_MEM:
		return source->operator()();
	case Slot::CACHE_MANAGED: {
			const std::string& config = source->getConfig();
			typename std::map<std::string,T>::const_iterator found;
			found = _dataCache.find(config);
			if (found == _dataCache.end()) {
				Slot::raise("data could not be retrieved (not cached)");
			}
			return found->second;
		}
	case Slot::CACHE_INVALID:
		Slot::raise("cannot read from output with cache type \"invalid\"");
	default:
		Slot::raise("no implementation for given output slot cache type");
	}
	// this should never be reached
	throw std::runtime_error(
			"unexpected error in InputSlot::_getDataFromOutputSlot");
}

template<typename T>
void InputSlot<T>::prepare() {
	typename std::set<AbstractSlot<T>*>::const_iterator item;
	item = AbstractSlot<T>::_targets.begin();
	for (;item != AbstractSlot<T>::_targets.end(); item++) {
		const AbstractSlot<T>* slot = *item;
		if (slot->getType() != this->getType()) {
			Slot::raise("input connection type mismatch");
		}
		const OutputSlot<T>* source = dynamic_cast<const OutputSlot<T>*>(slot);
		if (!source) {
			Slot::raise("input connection data type mismatch (dynamic cast)");
		}
		const Slot::CacheType& scType = source->getCacheType();
		if (scType == Slot::CACHE_MANAGED) {
			const std::string& config = source->getConfig();
			Slot::DataManager<T>* manager =
				Slot::DataManagerFactory<T>::getManager(*source, config);
			if (!manager) {
				Slot::raise("no data manager for this slot type available");
			}
			_dataCache.insert(std::pair<std::string,T>(
					config,manager->getData()));
			delete manager;
		}
	}
}

template<typename T>
void InputSlot<T>::finalize() {
	_dataCache.clear();
}

// ============================   class OutputSlot   ========================

template<typename T>
OutputSlot<T>::OutputSlot(const T& initval) :
		_initVal(initval), data(0), _cacheType(Slot::CACHE_MEM) {
	AbstractSlot<T>::_optional = true;
	AbstractSlot<T>::_multiSlot = true;
}

template<typename T>
void OutputSlot<T>::prepare() {
	if (!data) {
		data = new T(_initVal);
	}
}

template<typename T>
OutputSlot<T>::~OutputSlot() {
	if (data) {
		delete data;
	}
}

template<class T>
void OutputSlot<T>::load(
	const ParameterFile& pf, const PluginManagerInterface*) {
	const std::string parName =
			Slot::_parent->getName()+"."+Slot::_name+".caching";
	if (pf.isSet(parName)) {
		const std::string cType = pf.get<std::string>(parName);
		if (cType == "managed") {
			setCacheType(Slot::CACHE_MANAGED);
		}
		else if (cType == "memory") {
			setCacheType(Slot::CACHE_MEM);
		}
		else if (cType == "invalid") {
			setCacheType(Slot::CACHE_INVALID);
		}
		else {
			Slot::raise("Invalid cache type specified in parameter file.");
		}
	}
}

template<typename T>
void OutputSlot<T>::setCacheType(Slot::CacheType type) {
	const std::string prefix = "selected cache type: ";
	switch (type) {
	case Slot::CACHE_MEM:
		Slot::printInfo(prefix + "memory");
		break;
	case Slot::CACHE_MANAGED: {
			Slot::printInfo(prefix + "managed");
			Slot::DataManager<T>* manager =
					Slot::DataManagerFactory<T>::getManager(*this);
			if (!manager) {
				Slot::raise("no data manager for this slot type available");
			}
			delete manager;
		}
		break;
	case Slot::CACHE_INVALID:
		Slot::printWarning(prefix + "invalid");
		break;
	default:
		Slot::raise("setCacheType: invalid cache type given");
		break;
	}
	_cacheType = type;
}

template<typename T>
void OutputSlot<T>::_check() const {
	if (!data) {
		Slot::raise("reading from uninitialized output slot");
	}
}

template<typename T>
void OutputSlot<T>::_prepare() {
	if (!data) {
		Slot::printWarning(
					"accessing uninitialized output slot, "
					"creating default data element");
		prepare();
	}
}

template<class T>
OutputSlot<T>::operator T() const {
	_check();
	return *data;
}

template<class T>
const T& OutputSlot<T>::operator()() const {
	_check();
	return *data;
}

template<class T>
T& OutputSlot<T>::operator()() {
	_prepare();
	return *data;
}

template<class T>
T& OutputSlot<T>::operator=(const T& B) {
	_prepare();
	*data = B;
	return *data;
}

template<typename T>
void OutputSlot<T>::finalize() {
	switch (_cacheType) {
	case Slot::CACHE_MEM:
		// do not delete data after execution if memory caching is selected
		break;
	case Slot::CACHE_MANAGED: {
			// write data to manager
			Slot::DataManager<T>* manager =
					Slot::DataManagerFactory<T>::getManager(*this);
			if (!manager) {
				Slot::raise("no data manager for this slot type available");
			}
			manager->setData(*data);
			_managerConfig = manager->getConfig();
			delete manager;
		}
		// now data can be deleted
		// so continue as invalid
	case Slot::CACHE_INVALID:
		delete data;
		data = 0;
		break;
	default:
		break;
	}
}

template<typename T>
std::string OutputSlot<T>::getName() const
{
    return this->_name;
}

template<typename T>
std::string OutputSlot<T>::getType() const
{
    return this->_type;
}

// ==============================   data managers   ==========================

template <typename T>
Slot::DataManager<T>* Slot::DataManagerFactory<T>::getManager(
		const Slot&, const std::string&) {
	return 0;
}

#endif /* _SLOTS_HXX_ */
