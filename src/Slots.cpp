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

#include <charon-core/Slots.hxx>

// ==========================   class Slot   ================================

Slot::Slot() :
	_parent(0) {
}

void Slot::init(ParameteredObject* parent, std::string name,
		std::string type) {
	assert(!_parent); // not yet initialized
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
	sout << "(II) Slot \"" << getParent().getName() << "." << getName()
		<< "\": " << msg << std::endl;
}

void Slot::printWarning(const std::string& msg) const {
	sout << "(WW) Slot \"" << getParent().getName() << "." << getName()
		<< "\": " << msg << std::endl;
}

void Slot::printError(const std::string& msg) const {
	sout << "(EE) Slot \"" << getParent().getName() << "." << getName()
		<< "\" (type " << getType() << "): " << msg << std::endl;
}

void Slot::raise(const std::string& msg) const {
	throw std::runtime_error(
				"Error in Slot \"" + getParent().getName() + "." + getName()
				+ "\" (type " + getType() + "): "
				+ msg);
}
