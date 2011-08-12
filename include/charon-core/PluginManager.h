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
/**
 * @file PluginManager.h
 * Implements a class which handles plugins and their instances.
 * @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">
 *     Cornelius Ratsch</a>
 *
 * @date 20.08.2009
 */
#ifndef PLUGINMANAGER_H_
#define PLUGINMANAGER_H_

#ifdef UNIX
#include "UnixPluginLoader.h"
#include <set>
#define PLUGIN_LOADER UnixPluginLoader
#define DEPRECATED __attribute__((deprecated))

#ifdef APPLE
#define PLUGIN_EXTENSION ".dylib"
#else /* APPLE */
#define PLUGIN_EXTENSION ".so"
#endif /* APPLE */

#elif defined(WINDOWS)
#include "WindowsPluginLoader.h"
#define PLUGIN_LOADER WindowsPluginLoader
#define PLUGIN_EXTENSION ".dll"
#define DEPRECATED __declspec(deprecated)
#endif

#include <iostream>
#include <map>
#include "PluginManagerInterface.h"

/**
 * Manages ParameteredObject based plugins and their instances.
 * Offers methods to
 * - load and unload plugins
 * - create and destroy instances of plugins
 * - load parameter files and execute the discribed workflow
 * - write parameter files
 * Also, some methods which were previously members of the ParameteredObject
 * class are now members of this class.
 */
class PluginManager: public PluginManagerInterface {
private:
	/**
	 * Saves the currently loaded plugins
	 */
	std::map<std::string, PLUGIN_LOADER*> loadedPlugins;

	/**
	 * Links the instances to their PluginLoader, so the PluginManager can
	 * determine of which type the instance is
	 */
	std::map<ParameteredObject *, PLUGIN_LOADER *> instances;

	/**
	 * Saves the target points of the current workflow.
	 */
	std::vector<ParameteredObject *> targetPoints;

	/**
	 * Saves the current default template type.
	 */
	template_type defaultTemplateType;

	/**
	 * Deletes all instances of a loaded plugin.
	 * @param p Plugin loader which has created the instances
	 */
	void _destroyAllInstances(PLUGIN_LOADER * p);

	/**
	 * Unloads all Plugins and destroys all instances.
	 */
	void _unloadAllPlugins();

	/**
	 * Unloads a plugin and destroys instances
	 * @param p Plugin zo unload
	 * @param erase If true, erases the plugin from map
	 */
	void _unloadPlugin(PLUGIN_LOADER * p, bool erase = true);

	/**
	 * Creates metadata information for a specific plugin.
	 * Loads the plugin (if not already loaded), creates an instance of the
	 * plugin and destroys it immediately. If the plugin was not already loaded,
	 * unloads the plugin afterwards.
	 *
	 * @param pluginName Name of the plugin (without prefix or extension)
	 */
	void _createMetadataForPlugin(const std::string& pluginName);

	/**
	 * Iterates through the currently existing instances and looks for target
	 * points. Every instance which has no connected output slots is considered
	 * to be a target point.
	 */
	void _determineTargetPoints();

	/// @name Formerly part of the ParameteredObject class
	//  @{

	/// Recurse into object list and find connected objects.
	/// @param visited      List of visited objects (= result)
	/// @param cur          List of objects to look for neighbours
	void _getConnected(std::set<std::string>& visited, const std::set<
			std::string>& cur) const;

	/// Recurse into object list and find connected objects.
	/// This search is based on the parameterfile content instead
	/// of the real existing connections.
	/// @param visited      List of visited objects (= result)
	/// @param cur          List of objects to look for neighbours
	/// @param pf           ParameterFile to read connections from.
	void _getConnected(std::set<std::string>& visited, const std::set<
			std::string>& cur, const ParameterFile& pf) const;

	// @}
public:

	/// default lib suffix
#ifdef NDEBUG
	#define DEFAULT_DEBUG_SUFFIX false
#else
	#define DEFAULT_DEBUG_SUFFIX true
#endif

	/// default constructor
	/**
	 * Creates a new PluginManager instance and sets the path to the plugins to
	 * the value of pluginPaths.
	 * \param pluginPaths Paths where the plugins are stored.
	 *                    Search order as given.
	 * \param debugSuffix Look for libraries with debug suffix (<tt>_d</tt>),
	 *                    fallback to libs without suffix.
	 */
	PluginManager(
			const std::vector<std::string>& pluginPaths,
			bool debugSuffix = DEFAULT_DEBUG_SUFFIX);

	/// default constructor
	/**
	 * Creates a new PluginManager instance and sets the path to the plugins to
	 * the value of path1 and path2. This is a convenience function to
	 * preserve compatibility to the old non-vector style call.
	 * \param globalPath,localPath
	 *                    Paths where the plugins are stored.
	 *                    localPath is searched first.
	 * \param debugSuffix Look for libraries with debug suffix (<tt>_d</tt>),
	 *                    fallback to libs without suffix.
	 */
	PluginManager(
			const std::string& globalPath, const std::string& localPath = "",
			bool debugSuffix = DEFAULT_DEBUG_SUFFIX);

	/**
	 * Loads a plugin stored in the previously declared folder.
	 * @warning Do NOT call delete on an instance of a loaded plugin, use the
	 *          given method destroyInstance(ParameteredObject *) instead.
	 *
	 * @see unloadPlugin(const std::string &)
	 * @see AbstractPluginLoader::load()
	 * @see UnixPluginLoader::load()
	 * @see WindowsPluginLoader::load()
	 * @param name the file name of the plugin
	 */
	void loadPlugin(const std::string & name)
			throw (AbstractPluginLoader::PluginException);

	/**
	 * Unloads a plugin.
	 * @warning All instances of the plugin become obsolete and will be deleted
	 * therefore.
	 *
	 * @see loadPlugin(const std::string &)
	 * @see AbstractPluginLoader::unload()
	 * @see UnixPluginLoader::unload()
	 * @see WindowsPluginLoader::unload()
	 * @param name Name of the plugin
	 */
	void unloadPlugin(const std::string & name)
			throw (AbstractPluginLoader::PluginException);

	/**
	 * Checks if a plugin is loaded
	 *
	 * @param name name of the plugin
	 * @return true if the plugin is loaded
	 */
	bool isLoaded(const std::string & name) const;

	/**
	 * @return number of currently loaded plugins.
	 */
	int getLoadedPluginsCount() const;

	/**
	 * @return number of existing instances of loaded plugins.
	 */
	int getInstancesCount() const;

	/**
	 * Returns an existing instance of a loaded plugin.
	 *
	 * @param instanceName Name of the instance
	 * @return Pointer to the requested instance
	 */
	ParameteredObject * getInstance(const std::string & instanceName) const
			throw (AbstractPluginLoader::PluginException);

	/**
	 * Creates a new instance of a plugin.
	 * If the requested plugin isn't loaded yet, PluginManager will try to load
	 * it. If the instance name is an empty string, a unique name will be
	 * generated.
	 * @warning: Do NOT call delete on an instance of a loaded plugin, use the
	 * given method destroyInstance() instead.
	 *
	 *
	 * @see destroyInstance(ParameteredObject *)
	 * @see AbstractPluginLoader::createInstance()
	 * @see UnixPluginLoader::createInstance()
	 * @see WindowsPluginLoader::createInstance()
	 * @param pluginName Name of the plugin
	 * @param t Template type.
	 *          - ParameteredObject::TYPE_DOUBLE for a double type
	 *          - ParameteredObject::TYPE_FLOAT for a float type
	 *          - ParameteredObject::TYPE_INT for an integer type
	 * @param instanceName Name of the instance
	 * @return Pointer to a new instance
	 */
	ParameteredObject* createInstance(std::string pluginName,
			const template_type t, const std::string& instanceName = "")
			throw (AbstractPluginLoader::PluginException);

	/**
	 * Same Method, but allowing to leave the template type out. Current
	 * default template type will be used (initially, it's
	 * ParameteredObject::TYPE_DOUBLE).
	 *
	 * @see createInstance(const std::string &, const std::string &, const
	 *      template_type t)
	 * @param pluginName Name of the plugin
	 * @param instanceName Name of the instance
	 * @return Pointer to a new instance
	 */
	ParameteredObject * createInstance(const std::string & pluginName,
			const std::string & instanceName = "")
			throw (AbstractPluginLoader::PluginException);

	/**
	 * Deletes an instance of a loaded plugin.
	 * Calls the Destructor the instance.
	 *
	 * @warning Do NOT call delete on an instance of a loaded plugin, use this
	 * method instead.
	 * You also don't have to call this method because all existing instances
	 * will be deleted as a plugin is unloaded or as you call delete on an
	 * instance of the PluginManager.
	 *
	 * @see createInstance()
	 * @param toDestroy Pointer to an instance of a plugin
	 */
	void destroyInstance(ParameteredObject * toDestroy)
			throw (AbstractPluginLoader::PluginException);

	/**
	 * Reads a parameter file.
	 * - reads out the default template type property (if given)
	 * - loads the required plugins
	 * - creates the requested instances
	 * - connects the slots
	 * - reads out the target points (if existing)
	 *
	 *
	 * @param pf ParameterFile to load from
	 * @return Map linking the instance names to the created instances
	 */
	void loadParameterFile(const ParameterFile & pf);

	/**
	 * Same method, but loads a ParameterFile from the given path.
	 *
	 * @param path Path to the parameter file
	 * @return Map linking the instance names to the created instances
	 */
	void loadParameterFile(const std::string & path);

	/**
	 * Save content of all currently loaded instances to the given
	 * ParameterFile. All registered parameters and slot connections are saved.
	 *
	 * @warning This deletes the content of the given ParameterFile.
	 * @param paramFile      ParameterFile to save to
	 */
	void saveParameterFile(ParameterFile& paramFile) const;

	/**
	 * Same method, but writes the file to the given path.
	 * @warning This deletes the content of the given ParameterFile.
	 * @param path           Path where the parameter file should be saved.
	 */
	void saveParameterFile(const std::string & path) const;

	/**
	 * Sets the default template type property.
	 * New instances created without explicitly specifying another tempate type
	 * are created using this template type. Initially, the value is set to
	 * ParameteredObject::TYPE_DOUBLE.
	 *
	 *
	 * @param t New default template type
	 *          - ParameteredObject::TYPE_DOUBLE for a double type
	 *          - ParameteredObject::TYPE_FLOAT for a float type
	 *          - ParameteredObject::TYPE_INT for an integer type
	 */
	void setDefaultTemplateType(const template_type t);

	/**
	 * Returns the current default template type property. Initially, it is set
	 * to ParameteredObject::TYPE_DOUBLE.
	 *
	 * @see setDefaultTemplateType()
	 * @return Default template type property
	 *          - ParameteredObject::TYPE_DOUBLE for a double type
	 *          - ParameteredObject::TYPE_FLOAT for a float type
	 *          - ParameteredObject::TYPE_INT for an integer type
	 */
	template_type getDefaultTemplateType() const;

	/**
	 * Calls execute() on every target point.
	 *
	 * @see addTargetPoint(ParameteredObject *)
	 */
	void executeWorkflow();

	/**
	 * Iterates through the plugins available inside the plugin path and
	 * creates metadata information for them.
	 *
	 * @param targetPath Path to save the metadata files to. Defaults to the
	 *                   plugin path if empty.
	 */
	void createMetadata(const std::string & targetPath = "");

	/**
	 * Resets this PluginManager instance to its initial state.
	 * Unloads all plugins, resets defaultTemplateType parameter and deletes
	 * target points.
	 */
	void reset();

	/**
	 * Deletes all existing instances of any loaded plugin and then unloads all
	 * plugins. Frees the memory.
	 */
	virtual ~PluginManager();

	/// \name Formerly part of the ParameteredObject class
	//  \{

	/// Convenience function to get neighbours of given object.
	/** \param root         name of the root object to start neighbour search
	 *  \returns            set of neighbour object names
	 */
	std::set<std::string> getNeighbours(const std::string& root) const;

	/// Convenience function to get neighbours of given object.
	/** \param root         name of the root object to start neighbour search
	 *  \param pf           ParameterFile to read connections from.
	 *  \returns            set of neighbour object names
	 */
	std::set<std::string> getNeighbours(const std::string& root,
			const ParameterFile& pf) const;

	/// Recurse into object list and find connected objects.
	/** This is based on the content of the given ParameterFile,
	 *  i.e. the connected objects need not to exist really.
	 *  This is also independend of the existing connections of the
	 *  current object, it is only based on the connections saved
	 *  in the parameter file.
	 *  Nonexistent objects are created, so you have to make sure,
	 *  that the object factory is up and running.
	 *  The root object itself is also part of the connected component.
	 *  \param root         Starting point for connected object search
	 *  \param pf           ParameterFile to read connections from.
	 *  \returns            set of connected object names
	 */
	virtual std::set<std::string> getConnected(const std::string& root,
			const ParameterFile& pf) const;

	std::set<std::string> getConnected(ParameterFile & pf,
			ParameteredObject * obj) const;

	/// Recurse into object list and find connected objects.
	/** This is based on the "real" slot connections,
	 *  i.e. independend of some parameter file content.
	 *  The root object itself is also part of the connected component.
	 *  \param root         Starting point for connected object search
	 *  \returns            set of connected object names
	 */
	std::set<std::string> getConnected(const std::string& root) const;

	/// Connect slots.
	/** Both slots are connected with each other.
	 *  \param slot1        Slot1 (in or out)
	 *  \param slot2        Slot2 (out or in)
	 *  \retval true        operation successful
	 */
	bool connect(Slot& slot1, Slot& slot2);

	/// Same method, but taking pointers.
	/** \param slot1        Slot1 (in or out)
	 *  \param slot2        Slot2 (out or in)
	 *  \retval true        operation successful
	 */
	bool connect(Slot* slot1, Slot* slot2);

	/// Connect slots by name.
	/** You have to provide the slot names as "objectname.slotname",
	 *  i.e. the same way you find them in the parameter file.
	 *  Both slots are connected with each other.
	 *  You have to make sure, that both objects already exist.
	 *  \param slot1        Slot1 (e.g. "object1.slot1")
	 *  \param slot2        Slot2 (e.g. "object2.slot1")
	 *  \retval true        operation successful
	 */
	bool connect(const std::string& slot1, const std::string& slot2);

	/// Disconnect slots.
	/** Both slots are disconnected from each other.
	 *  \param slot1        Slot1 (in or out)
	 *  \param slot2        Slot2 (out or in)
	 *  \retval true        operation successful
	 */
	bool disconnect(Slot& slot1, Slot& slot2);

	/// Disconnect slots by name.
	/** You have to provide the slot names as "objectname.slotname",
	 *  i.e. the same way you find them in the parameter file.
	 * Both slots are disconnected with each other.
	 *  \param slot1        Slot1 (e.g. "object1.slot1")
	 *  \param slot2        Slot2 (e.g. "object2.slot1")
	 *  \retval true        operation successful
	 */
	bool disconnect(const std::string& slot1, const std::string& slot2);

	// \}
};

#endif /* PLUGINMANAGER_H_ */
