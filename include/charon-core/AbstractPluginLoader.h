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
/** @file AbstractPluginLoader.h
 *  Declares an abstract base class for a plugin loader. Also includes an
 *  exception class for plugin operations, nested inside the abstract class.
 *
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">
 *      Cornelius Ratsch</a>
 *
 *  @date 20.08.2009
 */
#ifndef ABSTRACTPLUGINLOADER_H_
#define ABSTRACTPLUGINLOADER_H_

#include "ParameteredObject.hxx"
#include <iostream>
#include <stdexcept>

/// Used by PluginLoadException
typedef unsigned short int error_code;

/**
 * Abstract base class for a plugin loader. If you want to develop an own
 * plugin loader, implement this class.
 */
class AbstractPluginLoader {
protected:
	/**
	 * The name of the Plugin (without prefix "lib" and without extension)
	 */
	std::string pluginName;

	/**
	 * Function pointer to the constructor of the plugin
	 */
	ParameteredObject* (*create)(
		const std::string&, ParameteredObject::template_type);

	/**
	 * Function pointer to the destructor of the plugin
	 */
	void (*destroy)(ParameteredObject *);

	/**
	 * Function pointer to the getBuildType function of the plugin
	  */
	ParameteredObject::build_type (*getBuildType)() ;
	/**
	 * Paths where the plugins are stored
	 */
	std::vector<std::string> &pluginPaths;


	/// Lib suffix e.g. <tt>_d</tt> for debug builds
	std::string &libSuffix;

public:


	/**
	 * Exception class
	 * Providing information about the reason of the error
	 */
	class PluginException: public std::runtime_error {
	private:
		/**
		 * Name of the plugin causing the error
		 */
		std::string nameOfPlugin;

		/**
		 * Saves the reason of the error
		 */
		error_code err;
	public:
		/**
		 * Plugin file doesn't exist or is placed in the wrong directory
		 */
		static const error_code FILE_NOT_FOUND = 1;

		/**
		 * File is damaged, is not a shared object or the user operation
		 * permissions are wrong
		 */
		static const error_code FILE_DAMAGED = 2;

		/**
		 * File is a shared object but misses required functions
		 */
		static const error_code INVALID_PLUGIN_FORMAT = 3;

		/**
		 * A plugin with the same name is already loaded
		 */
		static const error_code ALREADY_LOADED = 4;

		/**
		 * While unloading a plugin or deleting an instance: The plugin is
		 * not loaded
		 */
		static const error_code PLUGIN_NOT_LOADED = 5;

		/**
		 * Requested instance doesn't exist
		 */
		static const error_code NO_SUCH_INSTANCE = 6;

		/**
		 * An instance with the same name already exists.
		 */
		static const error_code DUPLICATE_INSTANCE_NAME = 7;

		/**
		 * Compiling a plugin failed.
		 */
		static const error_code COMPILE_ERROR = 7;

		/**
		 * Plugin was linked against incompatible runtime library
		 */
		static const error_code INCOMPATIBLE_BUILD_TYPE = 8 ;

		/**
		 * Other reason
		 */
		static const error_code OTHER = 0;

		/**
		 * Constructor
		 *
		 * @param message Exception message
		 * @param pluginName Name of the plugin causing the exception
		 * @param err Reason of the exception
		 */
		PluginException(const std::string & message,
				const std::string & pluginName = "", error_code err = OTHER);

		/**
		 * @return Name of the plugin causing the exception
		 */
		const std::string & getPluginName() const;

		/**
		 * @return Reason of the exception
		 */
		error_code getErrorCode() const;

		/**
		 * Destructor
		 */
		virtual ~PluginException() throw ();
	};

	/**
	 * Default constructor.
	 * Transforms the given plugin name to lowercase
	 *
	 * @param name The name of the Plugin (without prefix "lib" and without
	 *             extension)
	 * @param plpaths list of paths which are searched for the plugin
	 * @param lSuffix suffix which will be appended to the plugin name before search operations.
	 *        This is typically used to append _d for searching debug libraries.
	 */
	AbstractPluginLoader(const std::string& name,std::vector<std::string> &plpaths,std::string &lSuffix);

	/**
	 * Loads the plugin.
	 * Loads the shared library referring to the given plugin name into the
	 * memory and initializes the function pointers to the constructor and
	 * destructor of the plugin.
	 * @throws AbstractPluginLoader::PluginException
	 */
	virtual void load() throw (PluginException) = 0;

	/**
	 * Unloads the plugin.
	 * Removes the shared library from the memory. Existing instances of the
	 * plugin will become obsolete.
	 *
	 * @throws AbstractPluginLoader::PluginException
	 */
	virtual void unload() throw (PluginException) = 0;

	/**
	 * Returns the name of the plugin.
	 * @return Name of the plugin
	 */
	virtual const std::string& getName() const;

	/**
	 * Creates an instance of the loaded plugin and returns a pointer to it.
	 *
	 * @warning Do NOT call delete on an instance of a loaded plugin. Use
	 *          destroyInstance(ParameteredObject *) instead.
	 * @see destroyInstance(ParameteredObject * p)
	 * @param name Name of the new instance
	 * @param t Template type.
	 *          - ParameteredObject::TYPE_DOUBLE for a double type
	 *          - ParameteredObject::TYPE_FLOAT for a float type
	 *          - ParameteredObject::TYPE_INT for an integer type
	 * @return Pointer to the new instance
	 * @throws AbstractPluginLoader::PluginException
	 */
	virtual ParameteredObject * createInstance(const std::string & name,
		ParameteredObject::template_type t = ParameteredObject::TYPE_DOUBLE)
		throw (PluginException);

	/**
	 * Deletes an instance of the loaded plugin.
	 * @warning Do NOT call delete on an instance of a loaded plugin.
	 *
	 * @param p Pointer to the instance that will be deleted
	 * @throws AbstractPluginLoader::PluginException
	 */
	virtual void destroyInstance(ParameteredObject * p)
		throw (PluginException);

	/**
	 * Default Destructor.
	 * Unloads the plugin from the memory.
	 */
	virtual ~AbstractPluginLoader();
};

#endif /* ABSTRACTPLUGINLOADER_H_ */
