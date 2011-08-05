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
/** @file UnixPluginLoader.cpp
 *  Implementation of class UnixPluginLoader.
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
 *
 *  @date 24.08.2009
 */
#ifdef UNIX
#ifdef APPLE
#define LIBRARY_EXTENSION ".dylib"
#define DL_COMPILER_FLAG "-dynamiclib"
#else
#define LIBRARY_EXTENSION ".so"
#define DL_COMPILER_FLAG "-shared"
#endif /*APPLE*/

#include <dlfcn.h>
#include <charon-core/UnixPluginLoader.h>

UnixPluginLoader::UnixPluginLoader(const std::string & n) :
	AbstractPluginLoader(n) {
	libHandle = NULL;

}

void UnixPluginLoader::load() throw (PluginException) {
	std::string path = pluginPath + "/lib" + pluginName + LIBRARY_EXTENSION;
	if (FileTool::exists(path)) {
		libHandle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
	} else {
		path = additionalPluginPath + "/lib" + pluginName + LIBRARY_EXTENSION;
		std::string oldDir = FileTool::getCurrentDir();
		FileTool::changeDir(additionalPluginPath);
		libHandle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
		FileTool::changeDir(oldDir);
	}

	if (!libHandle) {
		if (!FileTool::exists(pluginPath + "/lib" + pluginName
				+ LIBRARY_EXTENSION) && !FileTool::exists(additionalPluginPath
				+ "/lib" + pluginName + LIBRARY_EXTENSION)) {
			throw PluginException("Failed to load the plugin \"" + pluginName
					+ "\". The file lib" + pluginName + LIBRARY_EXTENSION
					+ " could not be found. \n Description of the error: \n"
					+ dlerror(), pluginName, PluginException::FILE_NOT_FOUND);
		} else {
			throw PluginException("Failed to load the plugin \"" + pluginName
					+ "\". Maybe the file is damaged."
					+ "\n Description of the error: \n" + dlerror(),
					pluginName, PluginException::FILE_DAMAGED);
		}
	}

	sout << "Loading " << path << std::endl;

	create = (ParameteredObject*(*)(const std::string &, template_type)) dlsym(
			libHandle, "create");

	destroy = (void(*)(ParameteredObject *)) dlsym(libHandle, "destroy");

	if (!create) {
		dlclose(libHandle);
		libHandle = NULL;
		throw PluginException(
				"Failed to create the function pointer to the Constructor of the plugin \""
						+ pluginName + "\". Invalid plugin format.",
				pluginName, PluginException::INVALID_PLUGIN_FORMAT);
	}

	if (!destroy) {
		dlclose(libHandle);
		create = NULL;
		libHandle = NULL;
		throw PluginException(
				"Failed to create the function pointer to the Destructor of the plugin \""
						+ pluginName + "\". Invalid plugin format.",
				pluginName, PluginException::INVALID_PLUGIN_FORMAT);
	}
}

void UnixPluginLoader::unload() throw (PluginException) {
	if (libHandle) {
		dlclose(libHandle);
		if (dlerror()) {
			throw PluginException("Error unloading plugin \"" + pluginName
					+ "\". Description of the error:\n" + dlerror(),
					pluginName, PluginException::PLUGIN_NOT_LOADED);
		}
		libHandle = NULL;
		create = NULL;
		destroy = NULL;
		sout << "Successfully unloaded plugin \"" << pluginName << "\"."
				<< std::endl;
	} else {
		throw PluginException("Plugin \"" + pluginName + "\" is not loaded.",
				pluginName, PluginException::PLUGIN_NOT_LOADED);
	}
}

UnixPluginLoader::~UnixPluginLoader() {
	if (libHandle) {
		unload();
	}
}
#endif /*UNIX*/
