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
/// os dependent shlib extension (.so or .dylib)
#ifdef APPLE
#define LIBRARY_EXTENSION ".dylib"
#else
#define LIBRARY_EXTENSION ".so"
#endif /*APPLE*/

#include <dlfcn.h>
#include <charon-core/UnixPluginLoader.h>

UnixPluginLoader::UnixPluginLoader(const std::string & n) :
	AbstractPluginLoader(n) {
	libHandle = NULL;
}

void UnixPluginLoader::load() throw (PluginException) {
	std::string path, pathS;

	for(std::vector<std::string>::const_iterator  cur = pluginPaths.begin();
			cur != pluginPaths.end(); cur++) {
		path = *cur + "/lib" + pluginName + LIBRARY_EXTENSION;
		if(libSuffix.size() > 0) {
			pathS = *cur + "/lib" + pluginName + libSuffix + LIBRARY_EXTENSION;
			if (FileTool::exists(pathS)) {
				// prefer suffixed over plain version
				path = pathS;
			}
		}
		if (FileTool::exists(path)) {
			sout << "(II) Loading " << path << std::endl;
			libHandle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
			break;
		}
	}
	if (!libHandle) {
		if (!FileTool::exists(path)) {
			throw PluginException("Failed to load the plugin \"" + pluginName
					+ "\". The file lib" + pluginName + LIBRARY_EXTENSION +
					+ " could not be found. (Possible suffix: " + libSuffix
					+ ")\nDescription of the error:\n"
					+ dlerror(), pluginName, PluginException::FILE_NOT_FOUND);
		} else {
			throw PluginException("Failed to load the plugin \"" + pluginName
					+ "\". Maybe the file is damaged."
					+ "\n Description of the error:\n" + dlerror(),
					pluginName, PluginException::FILE_DAMAGED);
		}
	}

	// get function pointers from shared library
	*reinterpret_cast<void**>(&create)  = dlsym(libHandle, "create");
	*reinterpret_cast<void**>(&destroy) = dlsym(libHandle, "destroy");
	// This way, there is no cast between a
	// pointer-to-function and pointer-to-object
	// (-> not allowed by ISO C++!)
	//
	// The *adress* of the target function (which is an object)
	// is casted to void**, then dereferenced immediatly, so the
	// left hand side is an object pointer with same content as
	// the function pointers create/destroy.

	if (!create) {
		dlclose(libHandle);
		libHandle = NULL;
		throw PluginException(
				"function \"create\" missing in shared object file",
				pluginName, PluginException::INVALID_PLUGIN_FORMAT);
	}

	if (!destroy) {
		dlclose(libHandle);
		create = NULL;
		libHandle = NULL;
		throw PluginException(
				"function \"destroy\" missing in shared object file",
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
		sout << "(II) Successfully unloaded plugin \"" << pluginName << "\"."
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
