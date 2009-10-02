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

// ==========================   class Slot   ================================

inline Slot::Slot() :
	_parent(0) {
}

inline void Slot::init(ParameteredObject* parent, std::string name,
		std::string type) {
	assert(!_parent); // not yet initialized
	assert(parent); // parent valid
	_parent = parent;
	_name = name;
	_type = StringTool::toLowerCase(type);
}

inline Slot::~Slot() {
}

inline ParameteredObject& Slot::getParent() {
	return *_parent;
}

inline const ParameteredObject& Slot::getParent() const {
	return *_parent;
}

inline std::string Slot::getName() const {
	return _name;
}

inline bool Slot::getMulti() const {
	return _multiSlot;
}

inline bool Slot::getOptional() const {
	return _optional;
}

inline bool Slot::connected() {
	return (getTargets().size() > 0);
}

inline bool Slot::connected(Slot* target) {
	std::set<Slot *> targets = getTargets();
	return (targets.find(target) != targets.end());
}

inline bool Slot::connect(Slot* target) {
	assert(target);
	bool ret = true;
	ret &= _addTarget(target);
	ret &= target->_addTarget(this);
	return ret;
}

inline bool Slot::disconnect() {
	bool res = true;
	std::set<Slot*>::iterator tIter;
	std::set<Slot*> targets = getTargets();
	for (tIter = targets.begin(); tIter != targets.end(); tIter++) {
		res &= (*tIter)->_removeTarget(this);
		res &= _removeTarget(*tIter);
	}
	return res;
}

inline bool Slot::disconnect(Slot* target) {
	std::set<Slot*> targets = getTargets();
	if (targets.find(target) == targets.end())
		return false;
	bool res = true;
	res &= target->_removeTarget(this);
	res &= _removeTarget(target);
	return res;
}

inline void Slot::save(ParameterFile& /*pf*/) const {
	//Does nothing, erasing the parameter file is not required
}

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

	assert(target);

	/*
	 * Check slot types
	 * Here, the _type strings of the two slots are compared. Comparing
	 * type-IDs or using dynamic_cast is not possible because plugins could
	 * be compiled using different compilers which create different IDs.
	 */
	if (target->getType() != this->getType()) {
		throw(std::runtime_error("Error: Cannot connect slots of different "
			"types. Tried to connect \n"
			"Slot \"" + getParent().getName() + "." + getName() + "\" (type "
				+ getType() + ") to\n"
			"Slot \"" + target->getParent().getName() + "." + target->getName()
				+ "\" (type " + target->getType() + ")."));
	}

	_targets.insert((AbstractSlot<T>*) target);
	return true;
}

template<class T>
bool AbstractSlot<T>::_removeTarget(Slot* target) {
	assert(target);
	/*
	 * dynamic_cast isn't possible on dynamically loaded plugins.
	 * AbstractSlot<T>* t = dynamic_cast<AbstractSlot<T>*>(target);
	 */

	// if this assertion fails, please check the slot types!
	// Auto-typecheck will be performed, when built with
	// metadata creation enabled.
	assert(target->getType() == getType());
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

template<class T>
void AbstractSlot<T>::load(const ParameterFile& pf,
		const PluginManagerInterface * man) {
	// disconnect from other slots
	disconnect();

	if (!pf.isSet(_parent->getName() + "." + _name))
		// nothing to do
		return;

	// get all targets from parameter file
	std::vector<std::string> targetList = pf.getList<std::string> (
			_parent->getName() + "." + _name);

	// only multislots can be connected to more than one source!
	assert(_multiSlot || (targetList.size() < 2));

	if (!targetList.size())
		// nothing to do
		return;

	typename std::vector<std::string>::const_iterator tStrIter;

	// and add corresponding slots to _targets
	for (tStrIter = targetList.begin(); tStrIter != targetList.end(); tStrIter++) {
		ParameteredObject* targObj = man->getInstance(tStrIter->substr(0,
				tStrIter->find(".")));

		Slot * targetSlot = targObj->getSlot(tStrIter->substr(tStrIter->find(
				".") + 1));

		/*
		 * Check slot types
		 * Here, the _type strings of the two slots are compared. Comparing
		 * type-IDs or using dynamic_cast is not possible because plugins could
		 * be compiled using different compilers which create different IDs.
		 */
		if (targetSlot->getType() != this->getType()) {
			throw(std::runtime_error(
					"Error: Cannot connect slots of different "
						"types. Tried to connect \n"
						"Slot \"" + getParent().getName() + "." + getName()
							+ "\" (type " + getType() + ") to\n"
						"Slot \"" + targetSlot->getParent().getName() + "."
							+ targetSlot->getName() + "\" (type "
							+ targetSlot->getType() + ")."));
		}
		AbstractSlot<T>* tmp = (AbstractSlot<T>*) (targetSlot);
		connect(tmp);
	}
}

template<typename T>
void AbstractSlot<T>::execute() {
	for (typename std::set<AbstractSlot<T>*>::iterator it = _targets.begin(); it
			!= _targets.end(); it++) {
		(*it)->getParent().execute();
	}
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
InputSlot<T>::operator T() const {
	assert(AbstractSlot<T>::_targets.size());
	assert(((Slot*)(*(AbstractSlot<T>::_targets.begin())))->getType() == this->getType());
	OutputSlot<T>* source =
			(OutputSlot<T>*) (*(AbstractSlot<T>::_targets.begin()));
	return source->operator()();
}

template<class T>
const T& InputSlot<T>::operator()() const {
	assert(AbstractSlot<T>::_targets.size());
	assert(((Slot*)(*(AbstractSlot<T>::_targets.begin())))->getType() == this->getType());
	OutputSlot<T>* source =
			(OutputSlot<T>*) (*(AbstractSlot<T>::_targets.begin()));
	return source->operator()();
}

template<class T>
const T& InputSlot<T>::operator[](unsigned int pos) const {
	if (pos >= AbstractSlot<T>::_targets.size()) {
		std::ostringstream err;
		err << "pos out of range: pos = " << pos;
		err << "; size = " << AbstractSlot<T>::_targets.size();
		throw err.str();
	}

	typename std::set<AbstractSlot<T>*>::const_iterator item;
	item = AbstractSlot<T>::_targets.begin();
	for (unsigned int i = 0; i < pos; i++)
		item++;

	assert(((Slot*)(*item))->getType() == this->getType());
	OutputSlot<T>* source = (OutputSlot<T>*) (*item);
	return source->operator()();
}

// ============================   class OutputSlot   ========================

template<class T>
OutputSlot<T>::OutputSlot(const T& initval) :
	data(initval) {
	AbstractSlot<T>::_optional = true;
	AbstractSlot<T>::_multiSlot = true;
}

template<class T>
OutputSlot<T>::~OutputSlot() {
}

/*template<class T>
 void OutputSlot<T>::updateChildren() const {
 typename std::set<AbstractSlot<T>*>::const_iterator cur;
 for (cur = AbstractSlot<T>::_targets.begin(); cur
 != AbstractSlot<T>::_targets.end(); cur++)
 (*cur)->getParent().update((*cur)->getName());
 }*/

template<class T>
OutputSlot<T>::operator T() const {
	return data;
}

template<class T>
const T& OutputSlot<T>::operator()() const {
	return data;
}

template<class T>
T& OutputSlot<T>::operator()() {
	return data;
}

template<class T>
T& OutputSlot<T>::operator=(const T& B) {
	data = B;
	return data;
}

#endif /* _SLOTS_HXX_ */
