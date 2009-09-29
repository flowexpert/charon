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
/** @file ParameteredObject.h
 *  Declaration of the base class ParameteredObject.
 *  @author <a href="mailto:daniel.kondermann@iwr.uni-heidelberg.de">
 *      Daniel Kondermann</a>
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @author <a href="mailto:stephan-meister@arcor.de">Stephan Meister</a>
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg-de">Cornelius Ratsch</a>
 *
 *  Original file written by Daniel Kondermann, modified by Jens-Malte
 *  Gottfried and Stephan Meister.
 *
 *  In most cases, you do not want do include this file directly, use
 *  ParameteredObject.hxx instead.
 *
 *  @date 11.12.2008
 *
 *  Changes:
 *  -   added documentation
 *  -   renamed private members (left protected unchaged to maintain interface)
 *  -   split up into precompilable cpp part and header
 *  -   modified parametered object interface
 */

#ifndef _ParameteredObject_H
#define _ParameteredObject_H

#include "ParameterFile.hxx"
#include "Parameter.h"
#include "Slots.h"
#include <set>

#ifdef DLLEX
#undef DLLEX
#endif // DLLEX
#ifdef WINDOWS
#ifdef CREATE_SHARED
#define DLLEX __declspec(dllexport)
#else  // CREATE_SHARED
#define DLLEX __declspec(dllimport)
#endif // CREATE_SHARED
#else  // WINDOWS
#define DLLEX
#endif // WINDOWS

/// Integer which represents a template type.
typedef unsigned short int template_type;

class PluginManagerInterface;
template<typename T>
class TemplatedParameteredObject;


/// This is the base class for objects that should be able to save and
/// reload their settings in/from a ParameterFile.
/// Derive all savable/restorable classes from this one.
/// Reimplement the update routine to handle changed data input.
///
/// @remarks
/// -   Object names cannot be changed after instantiation because this
///     could mess up the parameter file store.
/// -   Objects know of each other, pointers to all existing objects
///     can be retrieved out of the ParameteredObject::_objectList.
/// -   If you define CREATE_METADATA, the objects will save information
///     about parameters and slot that can be used e.g. in the Tuchulcha
///     project for object collaboration visualization and editing.
/// -   An object factory can be created using the getInstance() function
///     which can even create new objects if a dummy object (named like the
///     className itself) exist.

class DLLEX ParameteredObject {
	friend class PluginManager;
	template<typename T>
	friend class TemplatedParameteredObject;

private:
    /// Count number of parametered objects with different class names.
    static std::map<std::string, unsigned int> _genericClassNameCount;

    /// Specifies if the ParameteredObject should create metadata information
    static bool _createMetadata;

    /// Class tracking.
    /// This parameterfile is used for metadata generation
    /// and stores the class names with their parameters
    /// and slots.
    ParameterFile _metadata;

    /// Create unique name.
    /// The name consists of the class name and a number.
    /// The number is increased on every function call.
    std::string _genericName();

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

    /// forbid copying
    /// Implementation needed to compile with Visual C++. Otherwise the following
    /// error max occur:
    /// Unresolved extern symbol ""__declspec(dllimport) private: __thiscall 
    /// ParameteredObject::ParameteredObject(class ParameteredObject const &)" 
    /// (__imp_??0ParameteredObject\@\@AAE\@ABV0\@\@Z)"
    ParameteredObject(const ParameteredObject&){}

    /// forbid instantiation without className etc.
    ParameteredObject();

    /// Load own content from the given parameter file.
    /// All registered parameters are loaded from the ParameterFile.
    /// This function does NOT touch any slot.
    /// InstanceName is used as prefix.
    /// @param pf           ParameterFile to save to.
    /// @param man          Pointer to the currently active PluginManager
    void _load(const ParameterFile& pf, const PluginManagerInterface * man);

    /// Common code for _addParameter, _addInputSlot, _addOutputSlot.
    /// This function does nothing, if CREATE_METADATA is not defined.
    /// @param extension    section of metadata file where to add
    ///                     (e.g. parameters, inputs, outputs)
    /// @param name         parameter name to pass to AbstractParameter::init()
    /// @param doc          parameter docstring (for metadata generation)
    /// @param type         Parameter type (string representation)
    /// @param defaultValue Default value, if any
    void _addSomething(const std::string& extension, const std::string& name,
        const std::string& doc, const std::string& type,
        const std::string& defaultValue = "");


    /// Connect slots.
    /// String version for convenience.
    /// See _connect(ParameteredObject*, Slot&, Slot&).
    /// @param target       Target object.
    /// @param ownSlot      Slot of the *this object (in or out).
    /// @param targetSlot   Slot of the target (out or in).
    bool _connect(ParameteredObject* target,    const std::string& ownSlot,
                                                const std::string& targetSlot);

    /// Disconnect slots.
    /// This function has the same arguments as ParameteredObject::connect()
    /// but removes an established connection.
    /// @param target       Target object.
    /// @param ownSlot      Slot of the *this object (in or out).
    /// @param targetSlot   Slot of the target (out or in).
    bool _disconnect(ParameteredObject* target, const std::string& ownSlot,
                                                const std::string& targetSlot);

protected:
    /// Add parameters.
    /// If CREATE_METADATA is defined, the parameter is also recorded to the
    /// class information in the _metadata parameter file.
    /// To be recorded correctly, you have also to specify the parameter
    /// docstring and type (e.g. int, float, or arbitrary).
    /// If type is an empty string, AbstractParameter will guess the type,
    /// which works on basic types like int, float, ... (see class TypeDetector)
    /// This assigns the parameter to this object.
    /// Each parameter can only be assigned once, of assigned twice, an
    /// error occurs (raises exception in AbstractParameter::init())
    /// @param param        New parameter to add
    /// @param name         parameter name to pass to AbstractParameter::init()
    /// @param doc          parameter docstring (for metadata generation)
    /// @param type         Parameter type (string representation)
    void _addParameter(AbstractParameter& param, const std::string& name,
        const std::string& doc, const std::string& type = "");

    /// Same function but adding a default value.
    /// Useful if you want to initialize the parameters not in their
    /// constructor.
    /// @param param        New parameter to add
    /// @param name         parameter name to pass to AbstractParameter::init()
    /// @param doc          parameter docstring (for metadata generation)
    /// @param defVal       new default Value
    /// @param type         Parameter type (string representation)
    template<class T>
    void _addParameter(Parameter<T>& param, const std::string& name,
        const std::string& doc, const T& defVal, const std::string& type = "");

    /// @name Register input/output slots.
    /// These functions needs to be called by the derived class in order
    /// to register all objects which can be used as inputs or outputs.
    /// The object is only valid configured, if all input slots are connected
    /// to the output of some other object. Only one connection can be
    /// established per input source. Output slots can have an arbitrary
    /// number of targets. If the slot data (which is stored in the output slot)
    /// has been changed, children of this output will be informed by calling
    /// the update() function. To initiate this, call the outDataChanged()
    /// function.
    /// The call syntax is similar to _addParameter.
    ///
    /// _addInput/OutputSlot() has to be called multiple times to add more then
    /// one slot.

    //  @{
    /// Register input slot.
    /// @param slot         New slot to add
    /// @param name         parameter name to pass to AbstractParameter::init()
    /// @param doc          parameter docstring (for metadata generation)
    /// @param type         Parameter type (string representation)
    void _addInputSlot(Slot& slot, const std::string& name,
        const std::string& doc, const std::string& type = "");

    /// Register output slot.
    /// @param slot         New slot to add
    /// @param name         parameter name to pass to AbstractParameter::init()
    /// @param doc          parameter docstring (for metadata generation)
    /// @param type         Parameter type (string representation)
    void _addOutputSlot(Slot& slot, const std::string& name,
        const std::string& doc, const std::string& type = "");
    //  @}

    // /// Change/set up instance name.
    // /// If name is an empty string, a new name is generated using
    // /// _genericName().
    // /// @param name         Instance name or empty string for auto generation.
    // void _setName(std::string name = "");

    /// Default constructor.
    /// Init class name with given string.
    /// Also generates a unique instance name if necessary.
    /// @param className    Initialization value for className property.
    /// @param name         Instance name (auto generation if empty)
    /// @param doc  Class docstring (for metadata)
    ParameteredObject(const std::string& className,
        const std::string& name = "", const std::string& doc = "");

public:
    /// The template type of the instance is double
    static const template_type TYPE_DOUBLE = 0;

    /// The template type of the instance is float
    static const template_type TYPE_FLOAT = 1;

    /// The template type of the instance is integer
    static const template_type TYPE_INT = 2;

    /**
     * Converts template_type to std::string
     */
    static std::string templateTypeToString(template_type t);

    /// Delete parametered object.
    /// Updates the parametered object list (_objectList)
    virtual ~ParameteredObject();

    /// Save own content to the given parameter file.
    /// All registered parameters are saved to the ParameterFile.
    /// This function does NOT touch any slot.
    /// InstanceName is used as prefix.
    /// @param pf           ParameterFile to save to.
    void save(ParameterFile& pf) const;

    /// Update object.
    ///
    /// The default implementation only calls execute() on all InputSlots.
    /// Reimplement this function to react on changed input data.
    virtual void execute();

    /// Class name getter.
    inline const std::string& getClassName() const {
        return _className;
    }

    /// Get instanceName
    inline const std::string& getName() const {
        return _instanceName;
    }

    /**
     * Returns the template type of a derived class of ParameteredObject.
     * Most of the derived classes are templated.
     * @return Template type as std::string
     */
    virtual const std::string getTemplateType() const;

    /// Get pointer to some slot (by name)
    /// @param slotName     Name of the slot to look for
    /// @throws std::string Error message if slot does not exist
    Slot* getSlot(const std::string& slotName);

    /// Const version
    /// @param slotName     Name of the slot to look for
    /// @throws std::string Error message if slot does not exist
    const Slot* getSlot(const std::string& slotName) const;

    /**
     * Gets the input slots.
     * Returns a map linking the names of the input slots to the Slots itself.
     * @return Map containing all input slots
     */
    const std::map<std::string, Slot *> & getInputSlots() const;

    /**
     * Gets the output slots.
     * Returns a map linking the names of the output slots to the Slots itself.
     * @return Map containing all output slots
     */
    const std::map<std::string, Slot *> & getOutputSlots() const;

    /// Save parameters of the current parametered object
    /// to the given parameter file.
    /// All registered parameters are saved to the ParameterFile.
    /// Slot connections are NOT stored.
    /// This function does only touch the current object and no
    /// connected one.
    /// @param pf           ParameterFile to save to.
    virtual void saveParameters(ParameterFile& pf) const;

    /// Load parameters of the current object
    /// from the given parameter file.
    /// All registered parameters
    /// are loaded from the ParameterFile.
    /// Slot connections are NOT loaded.
    /// This function does only touch the current object and no
    /// connected one.
    /// @param pf           ParameterFile to load from.
    virtual void loadParameters(const ParameterFile& pf);

    /// @name slot management
    //  @{

    /// Get connected neighbours of the current object.
    /// This is based on the "real" slot connections,
    /// i.e. independend of some parameter file content.
    std::set<std::string> getNeighbours() const;

    /// Get connected neighbours of the current object.
    /// This is based on the content of the given ParameterFile,
    /// i.e. the connected objects need not to exist really.
    /// This is also independend of the existing connections of the
    /// current object, it is only based on the connections saved
    /// in the parameter file.
    /// Nonexistent objects are created, so you have to make sure,
    /// that the object factory is up and running.
    /// @param pf           ParameterFile to read connections from.
    std::set<std::string> getNeighbours(const ParameterFile& pf) const;

    /// Check if object is ready for work.
    /// Check if all input slots are connected.
    virtual bool connected() const;

    AbstractParameter & getParameter(const std::string & name) const;

    static void setCreateMetadata(bool c);
    static bool getCreateMetadata();

    template <typename T>
    void setParameter(std::string name, T value);
    //  @}
};

/**
 * Base class for templated classes derived from ParameteredObject.
 * Implements the method getTemplateType, so it does not have to be implemented
 * in every plugin.
 */
template <typename T>
class TemplatedParameteredObject : public ParameteredObject {
public:
    /// Default constructor.
    /// Init class name with given string.
    /// Also generates a unique instance name if necessary.
    /// @param className    Initialization value for className property.
    /// @param name         Instance name (auto generation if empty)
    /// @param doc  Class docstring (for metadata)
    TemplatedParameteredObject(const std::string& className,
        const std::string& name = "", const std::string& doc = "");

    virtual const std::string getTemplateType() const;

	virtual void assign(T top, T left, T bottom, T right, T front = 0, T back = 1,
		T before = 0, T after = 1) { }
};

#endif // _ParameteredObject_H
