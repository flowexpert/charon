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
		libHandle = dlopen(path.c_str(), RTLD_LAZY);
	} else {
		path = additionalPluginPath + "/lib" + pluginName + LIBRARY_EXTENSION;
		std::string oldDir = FileTool::getCurrentDir();
		FileTool::changeDir(additionalPluginPath);
		libHandle = dlopen(path.c_str(), RTLD_LAZY);
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

void UnixPluginLoader::compileAndLoad(const std::string & sourceFile,
		std::vector<std::string> &references, const std::string & metadataPath)
		throw (PluginException) {

	ParameterFile p;
	try {
		p.load(_pathsConfig());
	} catch (std::string e) {
		throw PluginException(e);
	}
	std::string charon_core = p.get<std::string> ("charon-core-install");
	std::string compiler_call = p.get<std::string> ("compiler-call");
#ifdef APPLE
	std::vector<std::string> x11_libs_vector = p.getList<std::string>("x11-libs");
	std::string X11_libs(" ");
	for (unsigned int i = 0; i < x11_libs_vector.size(); i++) {
		X11_libs = X11_libs + " " + x11_libs_vector[i];
	}
#else
	std::string X11_libs = "";
#endif /* APPLE */

	//Preserve the current working directory and changing it to the plugin path
	std::string oldDir = FileTool::getCurrentDir();
	FileTool::changeDir(additionalPluginPath);

	//Collect referenced plugins
	std::string refs = "";
	for (unsigned int i = 0; i < references.size(); i++) {
		refs = refs + "-l" + StringTool::toLowerCase(references[i]) + " ";
	}

	//Compile the plugin
	std::cout << "Invoking C++ compiler to to compile the plugin \""
			+ pluginName
			+ "\" and creating metadata information.\nPlease be patient, this "
			+ "could take some time." << std::endl;

	std::string sysCall = compiler_call + " -fPIC -I \"" + charon_core
			+ "/include/charon-core\" -L \"" + charon_core + "/lib\" -L \""
			+ pluginPath + "\" -L \"" + additionalPluginPath + "\" "
			+ DL_COMPILER_FLAG + " -lcharon-core -lpthread " + refs
			+ " -Wl,-rpath,\"" + pluginPath + "\" -Wl,-rpath,\""
			+ additionalPluginPath + "\" -o \""
			+ (additionalPluginPath.size() ? additionalPluginPath : pluginPath)
			+ "/lib" + pluginName + LIBRARY_EXTENSION + "\" \"" + sourceFile
			+ "\" " + X11_libs + " > error.log";
#ifndef NDEBUG
	std::cout << "Compiler call:\n" << sysCall << std::endl;
#endif
	if (system(sysCall.c_str())) {
		throw PluginException("Error in compiling plugin \"" + pluginName
				+ "\". Compiler output:\n" + FileTool::readFile("error.log"), pluginName, PluginException::COMPILE_ERROR);
	}
	try {
		load();
		if (metadataPath.size()) {
			//create metadata
			ParameteredObject::setCreateMetadata(true);
			FileTool::changeDir(metadataPath);
			destroyInstance(createInstance("temp", 0));
			ParameteredObject::setCreateMetadata(false);
		}
	} catch (PluginException e) {
		throw e;
	}

	//Restore the old working directory
	FileTool::changeDir(oldDir);
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

std::string UnixPluginLoader::_pathsConfig() const {
	if (FileTool::exists("Paths.config"))
		return "Paths.config";
	else if (FileTool::exists("share/charon-core/Paths.config"))
		return "share/charon-core/Paths.config";
	else if (FileTool::exists("/usr/local/share/charon-core/Paths.config"))
		return "/usr/local/share/charon-core/Paths.config";
	else if (FileTool::exists("/usr/share/charon-core/Paths.config"))
		return "/usr/share/charon-core/Paths.config";
	else
		throw std::runtime_error(
				"Could not find file Paths.config.\n"
				"Tried directories:\n\t./\n\t./share/charon-core/\n\t"
				"/usr/local/share/charon-core/\n\t"
				"/usr/share/charon-core/\n"
				"Working dir:\n\t" + FileTool::getCurrentDir());
}

UnixPluginLoader::~UnixPluginLoader() {
	if (libHandle) {
		unload();
	}
}
#endif /*UNIX*/
