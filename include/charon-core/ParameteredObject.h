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
/** \file ParameteredObject.h
 *  Declaration of the base class ParameteredObject.
 *  \author <a href="mailto:daniel.kondermann@iwr.uni-heidelberg.de">
 *      Daniel Kondermann</a>
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \author <a href="mailto:stephan-meister@arcor.de">Stephan Meister</a>
 *  \author <a href="mailto:bc002@ix.urz.uni-heidelberg-de">Cornelius Ratsch</a>
 *
 *  Original file written by Daniel Kondermann, modified by Jens-Malte
 *  Gottfried and Stephan Meister.
 *
 *  In most cases, you do not want do include this file directly, use
 *  ParameteredObject.hxx instead.
 *
 *  \date 11.12.2008
 *
 *  Changes:
 *  -   added documentation
 *  -   renamed private members (left protected unchaged to maintain interface)
 *  -   split up into precompilable cpp part and header
 *  -   modified parametered object interface
 */

#ifndef _ParameteredObject_H
#define _ParameteredObject_H

#include <set>
#include "ParameterFile.hxx"
#include "Parameter.h"
#include "Slots.h"
#include "DllEx.h"

/// Integer which represents a template type.
typedef unsigned short int template_type;

class PluginManagerInterface;
template<typename T>
class TemplatedParameteredObject;


/// Base class for serializable objects
/** This is the base class for objects that should be able to save and
 *  reload their settings in/from a ParameterFile.
 *  Derive all savable/restorable classes from this one.
 *  Reimplement the execute() method to perform calculations.
 *
 *  \remarks
 *  -   Object names cannot be changed after instantiation because this
 *      could mess up the parameter file store.
 *  -   Objects know of each other, pointers to all existing objects
 *      can be retrieved out of the ParameteredObject::_objectList.
 */

class charon_core_DLL_PUBLIC ParameteredObject {
	friend class PluginManager;
	template<typename T>
	friend class TemplatedParameteredObject;

public:
	/// defined build type, see static members for options
	typedef unsigned short int build_type ;

private:

	/// status of initialization
	bool _initialized;

	/// Count number of parametered objects with different class names.
	static std::map<std::string, unsigned int> _genericClassNameCount;

	/// Specifies if the ParameteredObject should create metadata information
	static bool _createMetadata;

	/// Class tracking.
	/** This parameterfile is used for metadata generation
	 *  and stores the class names with their parameters
	 *  and slots.
	 */
	ParameterFile _metadata;

	/// Create unique name.
	/** The name consists of the class name and a number.
	 *  The number is increased on every function call.
	 *  \returns unique name
	 */
	std::string charon_core_LOCAL _genericName();

	/// Class name.
	std::string _className;

	/// Name of the instance.
	std::string _instanceName;

	/// This vector stores all parameters that have to be saved/restored.
	std::map<std::string, AbstractParameter*> _parameters;

	/// Input slots.
	std::map<std::string, Slot*> _inputs;

	/// Output slots.
	std::map<std::string, Slot*> _outputs;

	/// status of execution (set to true during ParameteredObject::execute())
	bool _executed;

	/// forbid copying
	/** Implementation needed to compile with MSVC.
	 */
	ParameteredObject(const ParameteredObject&) {}

	/// forbid instantiation without className etc.
	ParameteredObject();

	/// Load own content from the given parameter file.
	/** All registered parameters are loaded from the ParameterFile.
	 *  This function does NOT touch any slot.
	 *  InstanceName is used as prefix.
	 *  \param pf           ParameterFile to save to.
	 *  \param man          Pointer to the currently active PluginManager
	 */
	void _load(
			const ParameterFile& pf, const PluginManagerInterface * man);



	/// Common code for _addParameter, _addInputSlot, _addOutputSlot.
	/** This function does nothing, if _createMetadata is set to false.
	 *  \param extension    section of metadata file where to add
	 *                      (e.g. parameters, inputs, outputs)
	 *  \param name         parameter name to pass to AbstractParameter::init()
	 *  \param doc          parameter docstring (for metadata generation)
	 *  \param type         Parameter type (string representation)
	 *  \param defaultValue Default value, if any
	 *  \retval true        Parameter/Slot name is unique
	 */
	bool _addSomething(
			const std::string& extension, const std::string& name,
			const std::string& doc, const std::string& type,
			const std::string& defaultValue = "");

	/// Connect slots.
	/** String version for convenience.
	 *  See _connect(ParameteredObject*, Slot&, Slot&).
	 *  \param target       Target object.
	 *  \param ownSlot      Slot of the *this object (in or out).
	 *  \param targetSlot   Slot of the target (out or in).
	 *  \retval true operation successful
	 */
	bool _connect(
			ParameteredObject* target,
			const std::string& ownSlot,
			const std::string& targetSlot);

	/// Disconnect slots.
	/** This function has the same arguments as ParameteredObject::connect()
	 *  but removes an established connection.
	 *  \param target       Target object.
	 *  \param ownSlot      Slot of the *this object (in or out).
	 *  \param targetSlot   Slot of the target (out or in).
	 *  \retval true operation successful
	 */
	bool _disconnect(
			ParameteredObject* target,
			const std::string& ownSlot,
			const std::string& targetSlot);

	/// prepare slot data
	void _prepareSlots();

	/// commit slot data
	void _commitSlots();

protected:
	/// Add parameters.
	/** If _createMetadata is true, the parameter is also recorded to the
	 *  class information in the _metadata parameter file.
	 *  To be recorded correctly, you have also to specify the parameter
	 *  docstring and type (e.g. int, float, or arbitrary).
	 *  If type is an empty string, AbstractParameter will guess the type,
	 *  which works on basic types like int, float (see class TypeDetector)
	 *  This assigns the parameter to this object.
	 *  Each parameter can only be assigned once, of assigned twice, an
	 *  error occurs (raises exception in AbstractParameter::init())
	 *  \param param        New parameter to add
	 *  \param name         parameter name to pass to AbstractParameter::init()
	 *  \param doc          parameter docstring (for metadata generation)
	 *  \param type         Parameter type (string representation)
	 */
	void _addParameter(AbstractParameter& param, const std::string& name,
		const std::string& doc, const std::string& type = "");

	/// Same function but adding a default value.
	/** Useful if you want to initialize the parameters not in their
	 *  constructor.
	 *  \param param        New parameter to add
	 *  \param name         parameter name to pass to AbstractParameter::init()
	 *  \param doc          parameter docstring (for metadata generation)
	 *  \param defVal       new default Value
	 *  \param type         Parameter type (string representation)
	 */
	template<class T>
	void _addParameter(
			Parameter<T>& param, const std::string& name,
			const std::string& doc, const T& defVal,
			const std::string& type = "");

	/// \name Register input/output slots.
	/** These functions needs to be called by the derived class in order
	 *  to register all objects which can be used as inputs or outputs.
	 *  The object is only valid configured, if all slots with property
	 *  <i>optional</i> set to false are connected to slots of some other
	 *  object. Per default, only one connection can be established per input
	 *  source, if you want multiple connections to input slots, set the
	 *  property <i>multislot</i> to true. Per default, output slots can have
	 *  an arbitrary number of targets.
	 *  The call syntax is similar to _addParameter.
	 *
	 *  _addInput/OutputSlot() has to be called multiple times to add more then
	 *  one slot.
	 */

	//  \{
	/// Register input slot.
	/** \param slot         New slot to add
	 *  \param name         parameter name to pass to AbstractParameter::init()
	 *  \param doc          parameter docstring (for metadata generation)
	 *  \param type         Parameter type (string representation)
	 */
	void _addInputSlot(Slot& slot, const std::string& name,
		const std::string& doc, const std::string& type = "");

	/// Register output slot.
	/** \param slot         New slot to add
	 *  \param name         parameter name to pass to AbstractParameter::init()
	 *  \param doc          parameter docstring (for metadata generation)
	 *  \param type         Parameter type (string representation)
	 */
	void _addOutputSlot(Slot& slot, const std::string& name,
		const std::string& doc, const std::string& type = "");

	/// register member function
	/**
	 *  This hack is useful to get some member functions compiled
	 *  (and exported into) the (dynamic) library.
	 *  No code is executed, because this is always not zero, but the
	 *  compiler is forced to compile the given (templated) function
	 *  and export the symbol.
	 *  \param x	function to export
	 */
	#define _addFunction(x) if (!this && &x) throw 42;

	/// register some constructor
	/**
	 *  use this hack, if you have multiple templated constructors and want
	 *  them to be compiled (and exported) into the (dynamic) library.
	 *
	 *  Example: <code>_addConstructor(myObject(par1,par2,par3));</code>
	 *
	 *  \warning
	 *      at least on gcc, there are multiple kinds of constructors, i.e.
	 *      some base constructor that is used on derived classes and some
	 *      full constructor that is used to create instances.
	 *      This hack will only produce the full constructor, not the base one.
	 */
	#define _addConstructor(x) if (!this && new x) throw 42;
	//  \}

	/// Default constructor.
	/** Init class name with given string.
	 *  Also generates a unique instance name if necessary.
	 *  \param className    Initialization value for className property.
	 *  \param name         Instance name (auto generation if empty)
	 *  \param doc  Class docstring (for metadata)
	 */
	ParameteredObject(const std::string& className,
		const std::string& name = "", const std::string& doc = "");

	/// set property _executed
	/** \param value        New value of _executed
	 */
	void _setExecuted(bool value) {
		_executed = value;
	}

	/// get target nodes
	/** Determine target leaves of the execution workflow (i.e. Parametered
	 *  Objects with no targets connected to output slots).
	 *  \returns set of target nodes
	 */
	std::set<ParameteredObject*> _getTargetNodes();

	/// initialize plugin
	/** The default implementation does nothing at all.
	 *  Override this function implementing new modules.
	 *  This function may be pure virtual in future releases.
	 */
	virtual void initialize();

	/// execute plugin code
	/** The default implementation does nothing at all.
	 *  Override this function implementing new modules.
	 *  This function may be pure virtual in future releases.
	 */
	virtual void execute();

	/// finalize plugin
	/** The default implementation does nothing at all.
	 *  Override this function implementing new modules.
	 *  This function may be pure virtual in future releases.
	 */
	virtual void finalize();

	/// run all preceeding objects
	void runPreceeding() const;

	/// run all preceeding objects connected to
	/// a particular slot
	/** \param slot   select slot where preceeding
	 *                objects should be run */
	void runPreceeding(const Slot& slot) const;

public:
	/// The template type of the instance is double
	static const template_type TYPE_DOUBLE = 0;

	/// The template type of the instance is float
	static const template_type TYPE_FLOAT = 1;

	/// The template type of the instance is integer
	static const template_type TYPE_INT = 2;

	/// The build type was not defined explicitly,
	/// make no assumptions about the release/debug configuration
	static const build_type UNDEFINED_BUILD = 0 ;

	/// the object was compiled in debug mode
	static const build_type DEBUG_BUILD = 1 ;

	/// the object was compiled in release mode
	static const build_type RELEASE_BUILD = 2 ;

	// get the build type of this object
	/* \returns            build_type defaults to UNDEFINED_BUILD
	 */
	//virtual build_type getBuildType() const ;

	/// Converts template_type to std::string
	/** \param t            template type to convert
	 *  \returns            string representation of t
	 */
	static std::string templateTypeToString(template_type t);

	/// Delete parametered object.
	/** Updates the parametered object list (_objectList)
	 */
	virtual ~ParameteredObject();

	/// Save own content to the given parameter file.
	/** All registered parameters are saved to the ParameterFile.
	 *  This function does NOT touch any slot.
	 *  InstanceName is used as prefix.
	 *  \param pf           ParameterFile to save to.
	 */
	void save(ParameterFile& pf) const;

	/// Update object.
	/**
	 *  This function calls run on all InputSlots,
	 *  then the execute function is run.
	 *  After execution, the executed flag is set.
	 *  This function returns immediately, if the executed
	 *  flag is set already to avoid duplicate execution.
	 */
	virtual void run();

	/// get execution status
	bool executed() const {
		return _executed;
	}

	/// deprecated macro
#define PARAMETEREDOBJECT_AVOID_REEXECUTION \
		sout << "(WW) using deprecated AVOID_REEXECUTION macro" << std::endl;

	/// reset execution status
	/** This sets the propety _executed to false.
	 *  Per default, all depending objects are resetted too, but setting
	 *  the propagate parameter to false, only the object itself will
	 *  be resetted.
	 *  \param propagate    reset dependent objects too
	 */
	void resetExecuted(bool propagate = true);

	/// Class name getter.
	inline const std::string& getClassName() const {
		return _className;
	}

	/// Get instanceName
	inline const std::string& getName() const {
		return _instanceName;
	}

	/// get template type as string
	/** Returns the template type of a derived class of ParameteredObject.
	 *  Most of the derived classes are templated.
	 *  \return Template type as std::string
	 */
	virtual const std::string getTemplateType() const;

	/// Get pointer to some slot (by name)
	/** \param slotName     Name of the slot to look for
	 *  \throws std::invalid_argument if slot does not exist
	 *  \returns requested slot
	 */
	Slot* getSlot(const std::string& slotName);

	/// Const version
	/** \param slotName     Name of the slot to look for
	 *  \throws std::invalid_argument if slot does not exist
	 *  \returns requested slot
	 */
	const Slot* getSlot(const std::string& slotName) const;

	/// Get all input slots as map
	/** Returns a map linking the names of the input slots to the Slots itself.
	 *  \returns Map containing all input slots
	 */
	const std::map<std::string, Slot*>& getInputSlots() const;


	/// Get all output slots as map
	/** Returns a map linking the names of the output slots to the Slots itself.
	 *  \returns Map containing all output slots
	 */
	const std::map<std::string, Slot*>& getOutputSlots() const;

	/// Get all Parameters as a map
	/** Returns a map linking the names of the parameters to the parameters
	 *  itself.
	 *  \returns Map containing all output slots
	 */
	const std::map<std::string, AbstractParameter*>& getParameters() const;

	/// Save parameters to parameter file
	/** Saves parameters of the current parametered object
	 *  to the given parameter file.
	 *  All registered parameters are saved to the ParameterFile.
	 *  Slot connections are NOT stored.
	 *  This function does only touch the current object and no
	 *  connected one.
	 *  \param pf           ParameterFile to save to.
	 */
	virtual void saveParameters(ParameterFile& pf) const;

	/// Load parameter from parameter file
	/** Loads parameters of the current object
	 *  from the given parameter file.
	 *  All registered parameters
	 *  are loaded from the ParameterFile.
	 *  Slot connections are NOT loaded.
	 *  This function does only touch the current object and no
	 *  connected one.
	 *  \param pf           ParameterFile to load from.
	 */
	virtual void loadParameters(const ParameterFile& pf);

	/// \name slot management
	//  \{

	/// Get connected neighbours of the current object.
	/** This is based on the "real" slot connections,
	 *  i.e. independend of some parameter file content.
	 *  \returns set of neighbours
	 */
	std::set<std::string> getNeighbours() const;

	/// Get connected neighbours of the current object.
	/** This is based on the content of the given ParameterFile,
	 *  i.e. the connected objects need not to exist really.
	 *  This is also independend of the existing connections of the
	 *  current object, it is only based on the connections saved
	 *  in the parameter file.
	 *  Nonexistent objects are created.
	 *  \param pf           ParameterFile to read connections from.
	 *  \returns set of neighbours
	 */
	std::set<std::string> getNeighbours(const ParameterFile& pf) const;

	/// Check if object is ready for work.
	/** Check if all slots with property <i>optional</i> set to false
	 *  are connected.
	 *  \retval true all neccessary slots are connected
	 */
	virtual bool connected() const;

	
	/// throw an exception with information about the ParameteredObject
	/** Throws an excption of class std::runtime_error with 
	 *  additional information about the class type, template type 
	 *  and the ParameteredObject.
	 * \param message	Error message which will appended to the 
	 *		what c-string of the std::exception
	 */
	void raise(const std::string& message) const ;


	AbstractParameter & getParameter(const std::string & name) const;

	static void setCreateMetadata(bool c);
	static bool getCreateMetadata();

	template <typename T>
	charon_DEPRECATED void setParameter(std::string name, T value);
	//  \}
};

/// Base class for templated classes derived from ParameteredObject.
/** Implements the method getTemplateType, so it does not have to be
 *  implemented in every plugin.
 */
template <typename T>
class charon_core_PUBLIC TemplatedParameteredObject :
			public ParameteredObject {
public:
	/// Default constructor.
	/** Init class name with given string.
	 *  Also generates a unique instance name if necessary.
	 *  \param className    Initialization value for className property.
	 *  \param name         Instance name (auto generation if empty)
	 *  \param doc  Class docstring (for metadata)
	 */
	TemplatedParameteredObject(const std::string& className,
		const std::string& name = "", const std::string& doc = "");

	virtual const std::string getTemplateType() const;

        virtual ~TemplatedParameteredObject();
};


#endif // _ParameteredObject_H
