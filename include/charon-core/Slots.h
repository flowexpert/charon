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

	/// prepare slot
	/** this may be used on input/output slots to allocate memory */
	virtual void prepare() = 0;

	/// finalize slot
	/** this may be used on input/output slots to save/free data memory */
	virtual void finalize() = 0;

	virtual ~Slot();

	/// get parent object
	ParameteredObject& getParent();

	/// get parent object (const version)
	const ParameteredObject& getParent() const;

	/// Get slot name.
	std::string getName() const;

	/// print info message with slot name to sout
	void printInfo(const std::string& msg) const;

	/// print warning message with slot name to sout
	void printWarning(const std::string& msg) const;

	/// print error with slot name and type info to sout
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
	/** Input slots set up the connection between slots calling
	 *  _addTargets() on the partners to be connected via connect().
	 *  Output slots load their slot configuration, i.e. set up e.g. their
	 *  cache type if specified in the parameter file.
	 *  @param pf           ParameterFile to load from
	 *  @param man          PluginManager to get the instances from
	 */
	virtual void load(
			const ParameterFile& pf,
			const PluginManagerInterface* man) = 0;

	/// slot data cache type
	/** this enum selects how output data is stored after execution */
	enum CacheType {
		CACHE_INVALID, ///< dumped after execution, not recommended
		CACHE_MEM,     ///< stay in memory after execution
		CACHE_MANAGED  ///< cached on disk after execution, save memory
	};

	/// interface of data management classes
	/** This part of the interface provides the data I/O functions
	 */
	template<typename T>
	class DataManager {
	public:
		virtual ~DataManager() {}
		/// write data
		virtual void setData(const T& data /** [in] data to write */) = 0;
		/// read data
		virtual T getData() = 0;
		/// get configuration string
		virtual std::string getConfig() const = 0;
	};

	/// interface of data management factories
	/** These data managers are able e.g. to save some data
	 *  to disk and read them in.
	 *  This is used in the slot data exchange mechanism.
	 */
	template <typename T>
	class DataManagerFactory {
	public:
		/// get data manager from configuration string
		/** Given the parameter string, the factory should set up
		 *  a data manager that is able to re-read the data.
		 *  \param slot    slot to generate manager for
		 *  \param config  configuration string
		 *                 if this string is empty, a default manager
		 *                 e.g. for writing is generated.
		 *  \returns       data manager suitable for given data type
		 *                 or a null pointer if none is available
		 */
		static DataManager<T>* getManager(
			const Slot& slot, const std::string& config = "");
	};
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
	virtual void load(
			const ParameterFile& pf, const PluginManagerInterface* man);
	virtual operator T() const;
	virtual const T& operator()() const;
	virtual const T& operator[](std::size_t pos) const;
	virtual std::size_t size() const;
	/// prepare slot
	/** Here, all connected output slots are scanned for managed data cache.
	 *  For managed output slots, the data is queried using the
	 *  needed data manager and stored in the _dataCache map.
	 *  This is neccessary because the access operators are defined
	 *  as const operators and so are not able to allocate persistent
	 *  memory causing the references to become invalid.
	 *
	 *  The _dataCache map is cleared in finalize(), this way,
	 *  the memory is freed.
	 */
	virtual void prepare();
	virtual void finalize();
private:
	/// handle data extraction from output slot
	/** \warning make sure that the given slot is really an output slot,
	 *           this will be c-casted to OutputSlot<T>
	 *  \param slot         output slot to extract data from
	 *  \returns            extracted data
	 */
	const T& _getDataFromOutputSlot(const AbstractSlot<T>* slot) const;

	/// data cache for managed output slots
	std::map<std::string, T> _dataCache;
};

/// Output slot.
/// This slot stores the output data and a list of connected slots.
template <typename T>
class charon_core_PUBLIC OutputSlot :
		public AbstractSlot<T>, public AbstractData<T> {

private:
	/// initial value cache
	const T _initVal;
	T* data; ///< Slot data.
	/// check for valid data pointer
	void _check() const;
	/// create data element, if needed
	void _prepare();
	/// output slot data cache type
	Slot::CacheType _cacheType;
	/// config string
	std::string _managerConfig;

public:
	/// Create new output slot.
	/** \param initval      initialize data with this value */
	OutputSlot(const T& initval = T());
	/// initialize data element
	virtual ~OutputSlot();

	virtual void load(
			const ParameterFile& pf, const PluginManagerInterface* man);

	/// change data cache type
	void setCacheType(Slot::CacheType type);
	/// query data cache type
	Slot::CacheType getCacheType() const {
		return _cacheType;
	}
	/// get manager configuration string
	const std::string& getConfig() const {
		return _managerConfig;
	}

	// overload Slot functions
	virtual operator T() const;
	virtual const T& operator()() const;
	virtual T& operator()();
	virtual T& operator= (const T& B);
	virtual void prepare();
	virtual void finalize();
};

#endif /* _SLOTS_H */
