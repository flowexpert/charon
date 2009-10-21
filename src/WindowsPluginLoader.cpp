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
/** @file WindowsPluginLoader.cpp
 *  Implementation of class WindowsPluginLoader.
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
 *
 *  @date 24.08.2009
 */
#ifdef WINDOWS
#include <string>
#include <io.h>
#include <cstdlib>
#include <windows.h>

#ifdef MSVC
#include <strsafe.h>
#endif

#include "WindowsPluginLoader.h"
#include "ParameterFile.h"

WindowsPluginLoader::WindowsPluginLoader(const std::string & n) :
	AbstractPluginLoader(n) {
	hInstLibrary = NULL;
}

#ifdef MSVC
LPCTSTR WindowsPluginLoader::lastError(LPTSTR func) const {
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
			| FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dw, MAKELANGID(LANG_NEUTRAL,
					SUBLANG_DEFAULT), (LPTSTR) & lpMsgBuf, 0, NULL);
	lpDisplayBuf
	= (LPVOID) LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR) lpMsgBuf)
					+ lstrlen((LPCTSTR) func) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR) lpDisplayBuf, LocalSize(lpDisplayBuf)
			/ sizeof(TCHAR), TEXT("%s failed with error %d: %s"), func, dw,
			lpMsgBuf);

	return (LPCTSTR) lpDisplayBuf;
}
#endif

void WindowsPluginLoader::load() throw (PluginException) {
#ifdef MSVC
	if (FileTool::exists(pluginPath + "\\" + pluginName + ".dll")) {
		hInstLibrary = LoadLibrary(
			(pluginPath + "\\" + pluginName + ".dll").c_str());
#else
	if (FileTool::exists(pluginPath + "\\lib" + pluginName + ".dll")) {
		hInstLibrary = LoadLibrary(
			(pluginPath + "\\lib" + pluginName + ".dll").c_str());
#endif
	} else {
		std::string oldDir = FileTool::getCurrentDir();
		FileTool::changeDir(additionalPluginPath);
		hInstLibrary = LoadLibrary(
#ifdef MSVC
			(additionalPluginPath + "\\" + pluginName + ".dll").c_str());
#else
			(additionalPluginPath + "\\lib" + pluginName + ".dll").c_str());
#endif
		FileTool::changeDir(oldDir);
	}

	if (!hInstLibrary) {

#ifdef MSVC
		if (!FileTool::exists(pluginPath + "\\" + pluginName + ".dll")) {
			throw PluginException("Failed to load the plugin \"" + pluginName
					+ "\". The file " + pluginName
					+ ".dll could not be found. \nDescription of the error: \n"
					+ lastError("load()"), pluginName,
					PluginException::FILE_NOT_FOUND);
		} else {
			throw PluginException("Failed to load the plugin \"" + pluginName
					+ "\". Maybe the file is damaged."
					+ "\nDescription of the error: \n" + lastError("load()"),
					pluginName, PluginException::FILE_DAMAGED);
		}
#else
		if (!FileTool::exists(pluginPath + "\\lib" + pluginName + ".dll")) {
			throw PluginException("Failed to load the plugin \"" + pluginName
					+ "\". The file lib" + pluginName
					+ ".dll could not be found.", pluginName,
					PluginException::FILE_NOT_FOUND);
		} else {
			throw PluginException("Failed to load the plugin \"" + pluginName
					+ "\". Maybe the file is damaged.", pluginName,
					PluginException::FILE_DAMAGED);
		}
#endif
	}

	create
			= (ParameteredObject * (*)(const std::string &, template_type)) GetProcAddress(
					hInstLibrary, "create");

	destroy = (void(*)(ParameteredObject *)) GetProcAddress(hInstLibrary,
			"destroy");

	if (!create) {
		FreeLibrary(hInstLibrary);
		hInstLibrary = NULL;
#ifdef MSVC
		throw PluginException(
				"Failed to create the function pointer to the Constructor of the plugin \""
				+ pluginName
				+ "\". Invalid plugin format.\nDescription of the error:\n"
				+ lastError("load()"), pluginName,
				PluginException::INVALID_PLUGIN_FORMAT);
#else
		throw PluginException(
				"Failed to create the function pointer to the Constructor of the plugin \""
						+ pluginName + "\". Invalid plugin format.",
				pluginName, PluginException::INVALID_PLUGIN_FORMAT);
#endif
	}
	if (!destroy) {
		create = NULL;
		FreeLibrary(hInstLibrary);
		hInstLibrary = NULL;
#ifdef MSVC
		throw PluginException(
				"Failed to create the function pointer to the Destructor of the plugin \""
				+ pluginName
				+ "\". Invalid plugin format.\nDescription of the error:\n"
				+ lastError("load()"), pluginName,
				PluginException::INVALID_PLUGIN_FORMAT);
#else
		throw PluginException(
				"Failed to create the function pointer to the Destructor of the plugin \""
						+ pluginName + "\". Invalid plugin format.",
				pluginName, PluginException::INVALID_PLUGIN_FORMAT);
#endif
	}
}

void WindowsPluginLoader::compileAndLoad(const std::string & sourceFile, std::vector<
										 std::string> &references, const std::string & metadataPath) throw (PluginException) {

	//Load paths from the path file
	ParameterFile p;
	try {
		p.load(_pathsConfig());
	} catch (std::string e) {
		throw PluginException(e);
	}
	std::string charon_core = p.get<std::string> ("charon-core-install");
	std::string sdk_root = p.get<std::string> ("SDK-root");
	std::string vc_root = p.get<std::string> ("VC-root");
	std::string compiler_call = p.get<std::string> ("compiler-call");

	//Collect referenced plugins
	std::string refs = "";
	for (unsigned int i = 0; i < references.size(); i++) {
#ifdef MSVC
		refs += StringTool::toLowerCase(references[i]) + ".lib ";
#else
		refs += "-l" + StringTool::toLowerCase(references[i]) + " ";
#endif
	}

	//Preserve the current working directory and changing it to the plugin path
	std::string oldDir = FileTool::getCurrentDir();
	FileTool::changeDir(pluginPath);

	/*
	 * Now compile and load the final version with optimizing but without the
	 * CREATE_METADATA-flag
	 */
	sout << "Invoking C++ compiler to to compile the plugin and creating "
		"metadata information.\nPlease be patient, this could take some "
		"time." << std::endl;
#ifdef MSVC
	// syscall for compiling with VisualStudio
	std::string syscall =	"cl /wd4290 /wd4251 /O2 /Oi /GL /I \""
							+ charon_core + "/include/charon-core\" /I \"" + sdk_root + "\\Include\" /I \"" + vc_root
							+ "\\include\" " + "/D \"WIN32\" /D \"NDEBUG\" /D \"_WINDOWS\" "
							+ "/D \"WINDOWS\" /D \"HANDLE_DLL\" "
							+ "/D \"" + pluginName + "_EXPORTS\" /D \"MSVC\" /FD "
							+ "/EHsc /MD /Gy "
							+ "/W3 /nologo /TP \"" + sourceFile
							+ "\" /link /LIBPATH:\"" + vc_root + "\\lib\" /LIBPATH:\""
							+ sdk_root + "\\Lib\" /LIBPATH:\"" + charon_core
							+ "\\bin\" /LIBPATH:\"" + pluginPath + "\" /LIBPATH:\"" + additionalPluginPath + "\" /OUT:\"" + (additionalPluginPath.size() ? additionalPluginPath : pluginPath) + "\\" + pluginName
							+ ".dll\" /INCREMENTAL:NO /NOLOGO /DLL /MANIFEST /MANIFESTFILE:\""
							+ (additionalPluginPath.size() ? additionalPluginPath : pluginPath) + "\\" + pluginName
							+ ".dll.manifest\" /MANIFESTUAC:\"level='asInvoker' "
							+ "uiAccess='false'\" /SUBSYSTEM:WINDOWS "
							+ "/DYNAMICBASE /NXCOMPAT /IMPLIB:\""
							+ (additionalPluginPath.size() ? additionalPluginPath : pluginPath) + "\\" + pluginName + ".lib\" "
							+ refs + "kernel32.lib user32.lib gdi32.lib winspool.lib "
							+ "comdlg32.lib advapi32.lib shell32.lib ole32.lib "
							+ "oleaut32.lib uuid.lib odbc32.lib odbccp32.lib " + charon_core + "\\lib\\charon-core.lib > error.log ";
	std::string manifestCall = "mt /nologo /outputresource:\"" + (additionalPluginPath.size() ? additionalPluginPath : pluginPath) + "\\"
							+ pluginName + ".dll;#2\" -manifest \"" + (additionalPluginPath.size() ? additionalPluginPath : pluginPath) + "\\"
							+ pluginName + ".dll.manifest\" >> error.log";
#else
	// syscall for compiling with MinGW
	std::string syscall =	compiler_call + " -I" + charon_core + "/src"
							+ " -I" + charon_core + "/imgmanip"
							+ " -L" + charon_core + "/bin"
							+ " -L" + pluginPath + " -shared"
							+ " -o " + pluginPath + "/lib" + pluginName + ".dll"
							+ " -lgdi32 -lcharon-core -shared " + refs + " "
							+ sourceFile;
#endif

#ifndef NDEBUG
	sout << "Compiler call:\n" << syscall << std::endl;
	sout << "Manifest tool call:\n" << manifestCall << std::endl;
#endif // NDEBUG

	std::string temp = FileTool::getCurrentDir();
	if (system(syscall.c_str())) {
		throw PluginException("Error in compiling plugin \"" + pluginName
			+ "\". Compiler output:\n" + FileTool::readFile("error.log"), pluginName, PluginException::COMPILE_ERROR);
	}
	if (system(manifestCall.c_str()))
		throw PluginException("Error in invoking the manifest tool for the plugin \"" + pluginName
		+ "\". Console output:\n" + FileTool::readFile("error.log"), pluginName, PluginException::COMPILE_ERROR);
	
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

void WindowsPluginLoader::unload() throw (PluginException) {
	if (hInstLibrary) {
		FreeLibrary(hInstLibrary);
		hInstLibrary = NULL;
		create = NULL;
		destroy = NULL;
		sout << "Successfully unloaded plugin \"" << pluginName << "\"."
				<< std::endl;
	} else {
		throw PluginException("Plugin \"" + pluginName + "\" is not loaded.",
				pluginName, PluginException::PLUGIN_NOT_LOADED);
	}
}

std::string WindowsPluginLoader::_pathsConfig() const {
#ifdef MSVC
	char* buffer;
	_dupenv_s(&buffer, 0, "ProgramFiles");
	std::string programFiles = buffer;
	assert(buffer);
	free(buffer);
#else
	std::string programFiles = getenv("ProgramFiles");
#endif
	if (FileTool::exists("./Paths.config")) {
		return "./Paths.config";
	} else if (FileTool::exists(programFiles + "\\charon-core\\share\\charon-core\\Paths.config")) {
		return (programFiles + "\\charon-core\\share\\charon-core\\Paths.config");
	} else {
		return "";
	}
}

WindowsPluginLoader::~WindowsPluginLoader() {
	if (hInstLibrary) {
		unload();
	}
}
#endif
