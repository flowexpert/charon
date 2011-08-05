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

#include <charon-core/WindowsPluginLoader.h>
#include <charon-core/ParameterFile.h>

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
	std::string pluginFullPath;
#ifdef MSVC
	pluginFullPath = pluginPath + "\\" + pluginName + ".dll";
#else
	pluginFullPath = pluginPath + "\\lib" + pluginName + ".dll";
#endif
	if (FileTool::exists(pluginFullPath)) {
		hInstLibrary = LoadLibrary(pluginFullPath.c_str());
	} else {
#ifdef MSVC
		pluginFullPath = additionalPluginPath + "\\" + pluginName + ".dll";
#else
		pluginFullPath = additionalPluginPath + "\\lib" + pluginName + ".dll";
#endif
		if (!FileTool::exists(pluginFullPath)) {
			throw PluginException("Failed to load the plugin \"" + pluginName
				+ "\". The file " + pluginName
				+ ".dll could not be found.", pluginName,
				PluginException::FILE_NOT_FOUND);
		}

		std::string oldDir = FileTool::getCurrentDir();
		FileTool::changeDir(additionalPluginPath);
		hInstLibrary = LoadLibrary(pluginFullPath.c_str());
		FileTool::changeDir(oldDir);
	}

	sout << "File: " << pluginFullPath << std::endl;

	if (!hInstLibrary) {
		// error loading dll file
#ifdef MSVC
		std::string addInfo;
		if (GetLastError() == 0) {
			addInfo += "This is usually caused by missing dll dependencies.";
		}
		else {
			addInfo += "Maybe the file is damaged.\n";
			addInfo += "Description of the error:\n";
			addInfo += lastError("load()");
		}
		throw PluginException(
				"Failed to load the plugin \"" + pluginName
				+ "\".\n" + addInfo,
				pluginName, PluginException::FILE_DAMAGED);
#else
		throw PluginException("Failed to load the plugin \"" + pluginName
				+ "\". Maybe the file is damaged.", pluginName,
				PluginException::FILE_DAMAGED);
#endif
	}

	create  = (ParameteredObject*(*)(const std::string&, template_type))
				GetProcAddress(hInstLibrary,"create");
	if (!create) {
		FreeLibrary(hInstLibrary);
		hInstLibrary = NULL;
#ifdef MSVC
		std::string errorMsg;
		if (GetLastError() == ERROR_PROC_NOT_FOUND) {
			errorMsg += "This is no Plugin, function \"create\" is missing.";
		}
		else {
			errorMsg += "Invalid plugin format.\nDescription of the error:\n";
			errorMsg += lastError("load()");
		}
		throw PluginException(
				"Failed to get plugin constructor of \""
				+ pluginName + "\".\n" + errorMsg, pluginName,
				PluginException::INVALID_PLUGIN_FORMAT);
#else
		throw PluginException(
				"Failed to get plugin constructor of \""
						+ pluginName + "\". Invalid plugin format.",
				pluginName, PluginException::INVALID_PLUGIN_FORMAT);
#endif
	}

	destroy = (void(*)(ParameteredObject*))
				GetProcAddress(hInstLibrary,"destroy");
	if (!destroy) {
		create = NULL;
		FreeLibrary(hInstLibrary);
		hInstLibrary = NULL;
#ifdef MSVC
		std::string errorMsg;
		if (GetLastError() == ERROR_PROC_NOT_FOUND) {
			errorMsg += "This is no Plugin, function \"destroy\" is missing.";
		}
		else {
			errorMsg += "Invalid plugin format.\nDescription of the error:\n";
			errorMsg += lastError("load()");
		}
		throw PluginException(
				"Failed to get plugin destructor of \""
				+ pluginName + "\".\n" + errorMsg, pluginName,
				PluginException::INVALID_PLUGIN_FORMAT);
#else
		throw PluginException(
				"Failed to get plugin destructor of \""
						+ pluginName + "\". Invalid plugin format.",
				pluginName, PluginException::INVALID_PLUGIN_FORMAT);
#endif
	}

	getBuildType  = (ParameteredObject::build_type(*)())
				GetProcAddress(hInstLibrary,"getBuildType");
	if (!getBuildType) {
#ifdef MSVC
		std::string errorMsg;
		if (GetLastError() == ERROR_PROC_NOT_FOUND) {
			errorMsg += "This Plugin is missing the \"getBuildType\" "
				"function. No checks if it runtime libraries are "
				"compatible are perfomed";
		}
#endif
	}
	else
	{
		if(getBuildType() == ParameteredObject::DEBUG_BUILD)
		{
#ifndef _DEBUG
		FreeLibrary(hInstLibrary);
		hInstLibrary = NULL;
		throw PluginException(
			"The Plugin \"" + pluginName + "\" is build in DEBUG "
			"configuration while charon-core is in RELEASE Mode.\n"
			"Plugin will not be used as runtime libraries are incompatible",
			pluginName, PluginException::INCOMPATIBLE_BUILD_TYPE) ;
#endif 
		}
		else if(getBuildType() == ParameteredObject::RELEASE_BUILD)
		{
#ifdef _DEBUG
		FreeLibrary(hInstLibrary);
		hInstLibrary = NULL;
		throw PluginException(
			"The Plugin \"" + pluginName + "\" is build in RELEASE "
			"configuration while charon-core is in DEBUG Mode.\n"
			"Plugin will not be used as runtime libraries are incompatible",
			pluginName, PluginException::INCOMPATIBLE_BUILD_TYPE) ;

#endif
		}
	}
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

WindowsPluginLoader::~WindowsPluginLoader() {
	if (hInstLibrary) {
		unload();
	}
}
#endif
