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

/** \def PLUGIN_EXTENSION
 *  system plugin file extension (dll,so,dylib)
 */

/** \def PLUGIN_LOADER
 *  plugin loader classname (unix/win)
 */

#if defined(_WIN32) || defined(__WIN32__)
	#include "WindowsPluginLoader.h"
	#define PLUGIN_LOADER WindowsPluginLoader
	#define PLUGIN_EXTENSION ".dll"
#else // UNIX
	#include "UnixPluginLoader.h"
	#include <set>
	#define PLUGIN_LOADER UnixPluginLoader

	#ifdef APPLE
		#define PLUGIN_EXTENSION ".dylib"
	#else /* APPLE */
		#define PLUGIN_EXTENSION ".so"
	#endif /* APPLE */
#endif

#include <iostream>
#include <map>
#include <list>
#include "PluginManagerInterface.h"
#include "ParameteredObject.h"

/**
 * Manages ParameteredObject based plugins and their instances.
 * Offers methods to
 * - load and unload plugins
 * - create and destroy instances of plugins
 * - load parameter files and execute the discribed workflow
 * - write parameter files
 *
 * Also, some methods which were previously members of the ParameteredObject
 * class are now members of this class.
 *
 * \section pluginPaths Plugin Manager Paths
 * There may be multiple paths where Plugins are located.
 * These paths have to be given to the PluginManager constructor.
 * For compatibility reasons, there are constructors taking two paths
 * (global and local path) as arguments, but arbitrary numbers of paths
 * may be given to the stringlist versions of the constructor.
 *
 * If using the two-paths version, note that the local path (if given)
 * gets a higher priority than the global path. I.e. if the same Module
 * exists twice (in global and local path) the local one is found first and
 * will be used in workflows.
 *
 * If using the stringlist version of the constructor,
 * the paths are searched in the order as given.
 *
 * \section debugSuffix Plugins with Debug suffix
 * To be able to use plugins in their debug and release build configuration
 * side by side, it is possible to name the debug library with the suffix
 * <tt>_d</tt>. Regarding a plugin called <tt>example</tt>, the libraries
 * could be named e.g. <tt>example.dll</tt> and <tt>example_d.dll</tt>
 * (Win/MSVC) or e.g. <tt>libexample.so</tt> and <tt>libexample_d.so</tt>
 * (Linux/GCC). Since the names are different, both versions may be placed
 * side by side in the same folder.
 *
 * A mechanism to create the libraries easily this way is provided by the
 * CMake variable CMAKE_DEBUG_SUFFIX
 * (see <a href="http://www.cmake.org/cmake/help/documentation.html">
 * CMake Documentation</a> for more information).
 *
 * This line should be added to your main <tt>CMakeLists.txt</tt>
 * (i.e. located in the top folder) to split up your debug/release libraries:
 * \code
 *   PROJECT(...)
 *   [...]
 *   SET(CMAKE_DEBUG_POSTFIX _d)
 *   [...]
 * \endcode
 *
 * Which version of the libraries is preferred may be given to the
 * PluginManager constructor using the <b>debugSuffix</b> parameter:
 *
 * If this parameter is set to \c true (default on Debug builds), libraries
 * with debug suffix are preferred (i.e. if both versions exist, the suffixed
 * module will be loaded. If only one of them exists, there is no choice,
 * the found one will be loaded).
 *
 * If debugSuffix is set to \c false (default on Release builds),
 * suffixed modules are completely ignored
 * (even if only a suffixed file is found, it will <em>not</em> be loaded).
 *
 * \warning If the plugins are built with MSVC, it is essential, that
 * libraries and executables built in Debug/Release mode are not mixed
 * with each other. Otherwise there may be application crashes at runtime
 * because of different versions of the STL implementations (e.g. std::string).
 * The debug suffix mechanism provides a way to avoid this mixture.
 * The default value for the debugSuffix parameter is set to true on debug
 * builds and to false on release builds. Specifying this parameter manually
 * is useful only on unix builds where this mixture causes less or no problems.
 */
class PluginManager: public PluginManagerInterface {
private:
	/**
	 * Paths where the plugins are stored
	 */
	std::vector<std::string> pluginPaths;


	/// Lib suffix e.g. <tt>_d</tt> for debug builds
	std::string libSuffix;
	/**
	 * Saves the currently loaded plugins
	 */
	std::map<std::string, PLUGIN_LOADER*> _loadedPlugins;

	/**
	 * Links the instances to their PluginLoader, so the PluginManager can
	 * determine of which type the instance is
	 */
	std::map<ParameteredObject*, PLUGIN_LOADER*> _instances;

	/**
	 * Saves the current default template type.
	 */
	ParameteredObject::template_type _defaultTemplateType;

	/**
	 * load plugin regardless of version information in the library file
	 */
	bool _ignoreVersion ;

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
	  * Check if parametered object is an internal object
	  */
	bool isInternal(ParameteredObject* obj);

	/**
	 * Unloads a plugin and destroys instances
	 * @param p Plugin zo unload
	 * @param erase If true, erases the plugin from map
	 */
	void _unloadPlugin(PLUGIN_LOADER * p, bool erase = true);

	/// Creates metadata information for a specific plugin.
	/** Loads the plugin (if not already loaded), creates an instance of the
	 *  plugin and destroys it immediately.
	 *  If the plugin was not already loaded, unloads the plugin afterwards.
	 *  The metadata information is stored in a parameter text file
	 *  called "${targetPath}/${pluginName}.wrp"
	 *  \param pluginName Name of the plugin (without prefix or extension)
	 *  \param filename   Path to a file to store the metadata info
	 */
	void _generateMetadataForPlugin(
		const std::string& pluginName,
		const std::string& filename);

	/// Look for execution target points
	/** Iterates through the currently existing instances and looks for target
	 *  points. Every instance which has no connected output slots is
	 *  considered to be a target point.
	 *  \returns list of target objects
	 */
	std::list<ParameteredObject*> _determineTargetPoints();

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

	/// exclude list for metadata generation
	std::vector<std::string> _excludeList;

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
			bool debugSuffix = DEFAULT_DEBUG_SUFFIX,
			bool initializeOnLoad=false,
			bool ignoreVersion = true
	);

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
			const std::string& globalPath,
			const std::string& localPath = "",
			bool debugSuffix = DEFAULT_DEBUG_SUFFIX,
			bool initializeOnLoad=false,
			bool ignoreVersion=true
	);

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
	void loadPlugin(std::string name)
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

	/// Gets the plugin paths
	const std::vector<std::string>& getPluginPaths() const;

	/**
	 * @return number of currently loaded plugins.
	 */
	size_t getLoadedPluginsCount() const;

	/**
	 * @return number of existing instances of loaded plugins.
	 */
	size_t getInstancesCount() const;

	/**
	 * Returns an existing instance of a loaded plugin.
	 *
	 * @param instanceName Name of the instance
	 * @return Pointer to the requested instance
	 */
	ParameteredObject* getInstance(const std::string & instanceName) const
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
	ParameteredObject* createInstance(
		std::string pluginName,
		ParameteredObject::template_type t,
		const std::string& instanceName = "")
			throw (AbstractPluginLoader::PluginException);

	/**
	  * Insert an existing parametered object instance.
	  * @warning instance will be managed by pluginmanager.
	  */
	void insertInstance(ParameteredObject* instance);

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
	ParameteredObject* createInstance(const std::string & pluginName,
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
	void setDefaultTemplateType(ParameteredObject::template_type t);

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
	ParameteredObject::template_type getDefaultTemplateType() const;

	/// run whole workflow
	/** Calls run() on every target point.
	 *  @see addTargetPoint(ParameteredObject *)
	 */
	void runWorkflow();

	/// for compatibility, use runWorkflow instead
	charon_DEPRECATED void executeWorkflow() {
		runWorkflow();
	}

	/// set exclude list
	void setExcludeList(const std::vector<std::string>& list);


	/// Set the executed flags of the objects to false.
	/** This function leaves the objects otherwise untouched */
	void resetExecuted();

	/**
	 * Iterates through the plugins available inside the plugin path and
	 * creates metadata information for them.
	 *
	 * @param targetPath Path to save the metadata files to. Defaults to the
	 *                   plugin path if empty.
	 */
	void createMetadata(const std::string & targetPath = "");

	/**
	 * Creates metadata of all dynamic plugins in given parameter file.
	 * FilePrefix should be some/file_prefix instead of some/file_prefix.wrp
	 * since this method writes the metadata into
	 * some/file_prefix_instance_name.wrp
	 *
	 * @param paramFile   ParameterFile containing parameters for plugin
	 * @param filePrefix  File prefix to determine save file names
	 */
	void createDynamicMetadata(const ParameterFile& paramFile,
		const std::string& filePrefix);

	/**
	 *  Same method, but loads a ParameterFile from the given path.
	 *
	 *  @param paramFile   path to ParameterFile
	 *  @param filePrefix  File prefix to determine save file names
	 */
	void createDynamicMetadata(const std::string& paramFile,
		const std::string& filePrefix);

	/**
	 * Creates metadata of dynamic plugin based on parameter file
	 *
	 * @param pluginName  Name of dynamic plugin
	 * @param paramFile   ParameterFile containing parameters for plugin
	 * @param fileName    File in which to save metadata
	 */
	void createDynamicMetadata(const std::string& pluginName,
		const ParameterFile& paramFile, const std::string& fileName);

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

	/// Determine execution order
	/** Get topological sorted list of module instance names,
	 *  i.e. an order of instances where each instance depends
	 *  on predecessors only.
	 *  This means that input slots of the objects are connected to
	 *  preceeding objects only.
	 *  If an object is executed multiple times, the first execution
	 *  counts.
	 *  \returns list of executed objects
	 */
	std::list<ParameteredObject*> determineExecutionOrder();
};

#endif /* PLUGINMANAGER_H_ */
