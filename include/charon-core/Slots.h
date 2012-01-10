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
/** @file Slots.h
 *  Declaration of the abstract class
 *  Slot and template classes InputSlot and OutputSlot.
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
 *
 *  This file does only contain the (prototyped) interface of the classes
 *  - Slot
 *  - InputSlot
 *  - OutputSlot
 *
 *  The implementation of these prototypes can be found in Slots.hxx.
 *
 *  In most cases, you want not to include this file but ParameteredObject.hxx
 *
 *  @date 03.04.2009
 */

#ifndef _SLOTS_H
#define _SLOTS_H

#include "TypeDetector.h"
#include "AbstractData.hxx"
#include "ParameterFile.h"
#include <set>
#include "DllEx.h"

class ParameteredObject;
class PluginManagerInterface;

/// Commom properties of slot objects.
class charon_core_DLL_PUBLIC Slot {
private:
	/// forbid copying
	Slot(const Slot&);

protected:
	/// Add slot target.
	/// This does not touch the target slot itself.
	/// @param target       Target slot to add.
	/// @retval true        Target sucessfully connected.
	/// @retval false       Target was not connected.
	virtual bool _addTarget(Slot* target) = 0;

	/// Remove slot target.
	/// This does not touch the target slot itself.
	/// @param target       Target slot to remove.
	/// @retval true        Target sucessfully disconnected.
	/// @retval false       Target was not connected.
	virtual bool _removeTarget(Slot* target) = 0;

	/// Pointer to parent object
	ParameteredObject* _parent;

	/// Slot name.
	std::string _name;

	/// Slot type
	std::string _type;

	/// flag to mark this slot as optional
	bool _optional;

	/// flag to mark this slot as a multislot,
	/// that can have multiple sources/targets.
	bool _multiSlot;

public:
	/// default constructor
	Slot();

	/// initialize parent and name
	/// @param parent           parent object
	/// @param name             slot name
	/// @param type             slot type
	/// @throws std::string     Error message if invalid parent set.
	void init(ParameteredObject* parent, std::string name, std::string type);

	virtual ~Slot();

	/// get parent object
	ParameteredObject& getParent();

	/// get parent object (const version)
	const ParameteredObject& getParent() const;

	/// Get slot name.
	std::string getName() const;

	/// throw runtime error with slot name and type info
	void printError(const std::string& msg) const;

	/// throw runtime error with slot name and type info
	void raise(const std::string& msg) const;

	/**
	 * Get slot type.
	 * If "<T>" is found inside the type string, this will be replaced through
	 * the template type of the parent ParameteredObject.
	 *
	 * @return std::string representation of the slot type
	 */
	virtual std::string getType() const = 0;

	/// return value of _multiSlot;
	bool getMulti() const;

	/// return value of _optional;
	bool getOptional() const;

	/// Try to guess slot type.
	virtual std::string guessType() const = 0;

	/// Get pointers to the connected targets.
	virtual std::set<Slot*> getTargets() const = 0;

	/// Check if slot is connected.
	bool connected();

	/// Check if slot is connected to given slot.
	/// \param target           Target slot to check.
	/// \retval true            Given slots are connected.
	bool connected(Slot& target);

	/// Connect with given slot.
	/** \param target           Target slot to connect to.
	 *  \retval true            operation successful
	 */
	bool connect(Slot& target);

	/// Remove all slot targets.
	bool disconnect();

	/// Remove all slot targets.
	bool disconnect(Slot& target);

	/// Save slot connections
	/// This function disconnects already established connections in the
	/// parameterFile and stores the current one.
	/// @warning            You have to make sure, that this function
	///                     is only called once in each save run,
	///                     otherwise the connections will be erased
	///                     from the target slot.
	/// @param pf           ParameterFile to save to
	virtual void save(ParameterFile& pf) const;

	/// Load slot connections
	/// @param pf           ParameterFile to load from
	/// @param man          PluginManager to get the instances from
	virtual void load(
			const ParameterFile& pf,
			const PluginManagerInterface* man) = 0;
};

/// Encapsulation of slot connection handling (type specific)
/// and common code for input and output slots
template <typename T>
class charon_core_PUBLIC AbstractSlot : public Slot {
protected:
	/// Pointer to data of connected output slot.
	std::set<AbstractSlot<T>*> _targets;

	/// Add slot target.
	/// This does not touch the target slot itself.
	/// @param target       Target slot to add.
	/// @retval true        Target sucessfully connected.
	/// @retval false       Target was not connected.
	virtual bool _addTarget(Slot* target);

	/// Remove slot target.
	/// This does not touch the target slot itself.
	/// @param target       Target slot to remove.
	/// @retval true        Target sucessfully disconnected.
	/// @retval false       Target was not connected.
	virtual bool _removeTarget(Slot* target);

public:
	AbstractSlot();
	virtual ~AbstractSlot();

	// overload Slot functions
	virtual std::string guessType() const;
	virtual std::set<Slot*> getTargets() const;
	virtual void save(ParameterFile& pf) const;
	virtual void load(
			const ParameterFile& pf, const PluginManagerInterface* man);
	virtual std::string getType() const;

	/// @name Iterators for stl-like usage
	//  @{
	/// iterator to the beginning of the source list
	typename std::set<AbstractSlot<T>*>::const_iterator begin() const;
	/// iterator to the end of the source list
	typename std::set<AbstractSlot<T>*>::const_iterator end() const;
	//  @}
};

/// Input slot.
/// This slot does not stores the data itself, but contains a pointer
/// to the connected output slot. Data are read from this source, if needed.
template <typename T>
class charon_core_PUBLIC InputSlot :
		public AbstractSlot<T>, public AbstractROData<T>,
		public AbstractMultiROData<T> {
public:
	/// Create new input slot.
	/// @param optional     make this slot optional
	/// @param multi        make this slot connectable to multiple slots
	InputSlot(bool optional=false, bool multi=false);

	virtual ~InputSlot();

	// overload Slot functions
	virtual operator T() const;
	virtual const T& operator()() const;
	virtual const T& operator[](std::size_t pos) const;
	virtual std::size_t size() const;
};

/// Output slot.
/// This slot stores the output data and a list of connected slots.
template <typename T>
class charon_core_PUBLIC OutputSlot :
		public AbstractSlot<T>, public AbstractData<T> {

private:
	T* data; ///< Slot data.

public:
	/// Create new output slot.
	OutputSlot();
	/// Create new output slot.
	/// @param initval      initialize data with this value
	OutputSlot(const T& initval);
	virtual ~OutputSlot();

	// overload Slot functions
	virtual operator T() const;
	virtual const T& operator()() const;
	virtual T& operator()();
	virtual T& operator= (const T& B);
};

#endif /* _SLOTS_H */
