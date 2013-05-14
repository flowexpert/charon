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
#include <string>
#include <io.h>
#include <cstdlib>
#include <windows.h>

#ifdef MSVC
#include <strsafe.h>
#endif

#include <charon-core/WindowsPluginLoader.h>
#include <charon-core/ParameterFile.h>

WindowsPluginLoader::WindowsPluginLoader(const std::string & n,std::vector<std::string> &plpaths,std::string &lSuffix, bool ignoreVersion) :
	AbstractPluginLoader(n,plpaths,lSuffix,ignoreVersion) {
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
	std::string path, pathS;
	for (std::vector<std::string>::const_iterator cur = pluginPaths.begin();
			cur != pluginPaths.end(); cur++) {
#ifdef MSVC
		path = *cur + "\\" + pluginName + ".dll";
		pathS = *cur + "\\" + pluginName + libSuffix + ".dll";
#else
		path = *cur + "\\lib" + pluginName + ".dll";
		pathS = *cur + "\\lib" + pluginName + libSuffix + ".dll";
#endif
		// prefer suffixed version if available
		// load unsuffixed library as fallback
		if (FileTool::exists(pathS)) {
			path = pathS;
		}
		if (FileTool::exists(path)) {
			std::string oldDir = FileTool::getCurrentDir();
			FileTool::changeDir(*cur);
			
			if(!_ignoreVersion)
			{
				DWORD handle = 0; //dummy value
				DWORD size = GetFileVersionInfoSize(path.c_str(), &handle);
				if(!size)
				{
					throw PluginException(
					"The plugin contains no valid charon-core version information.", pluginName,
					PluginException::VERSION_INFORMATION_MISSING);
				}
				BYTE* versionInfo = new BYTE[size];
				if (!GetFileVersionInfo(path.c_str(), handle, size, versionInfo))
				{
					delete[] versionInfo;
#ifdef MSVC
					std::string addInfo = "The plugin contains no valid charon-core version information: ";
					if (GetLastError() == 0) {
						addInfo += lastError("load()");
					}
					throw PluginException(
							"Failed to load the plugin \"" + pluginName
							+ "\".\n(EE)\t" + addInfo,
							pluginName, PluginException::VERSION_INFORMATION_MISSING);
#else
					throw PluginException(
							"Failed to load the plugin \"" + pluginName
							+ "\". The plugin contains no valid charon-core version information.", 
							pluginName, PluginException::VERSION_INFORMATION_MISSING);
#endif
				}

				struct LANGANDCODEPAGE {
					WORD wLanguage;
					WORD wCodePage;
				} *lpTranslate;

				UINT	dwBytes; //size of contained code tables
				
				//get language and code page block from file version information
				BOOL res = VerQueryValue(versionInfo, 
					TEXT("\\VarFileInfo\\Translation"),
					(LPVOID*)&lpTranslate,
					&dwBytes);
				if(res == 0 || dwBytes == 0)
				{
					delete[] versionInfo;
					throw PluginException("Failed to load the plugin \"" + pluginName
						+ "\". The plugin contains no valid charon-core version information.", pluginName,
					PluginException::VERSION_INFORMATION_MISSING);
				}

				//usualy we should have only one block of version information per file
				//but theoretically there could be more
				for(int i=0; i < (dwBytes/sizeof(struct LANGANDCODEPAGE)); i++ )
				{
					LPTSTR subBlock ;
					subBlock = new char[50] ;
					BOOL hr = StringCchPrintf(subBlock, 50,
								TEXT("\\StringFileInfo\\%04x%04x\\ProductName"),
								lpTranslate[i].wLanguage,
								lpTranslate[i].wCodePage);
					if (FAILED(hr))
					{
						delete[] versionInfo;
						delete[] subBlock ;
						throw PluginException("Failed to load the plugin \"" + pluginName
							+ "\". The plugin contains no valid charon-core version information.", pluginName,
						PluginException::VERSION_INFORMATION_MISSING);
					}
					LPCTSTR infoBuffer ;
					UINT bufferLen = 0;
					// Retrieve file description for language and code page "i". 
					BOOL res = VerQueryValue(versionInfo, 
									subBlock, 
									(LPVOID*)&infoBuffer, 
									&bufferLen); 
					//check that the "ProductName" field is set to "Charon-Suite"
					if(res == 0 && bufferLen == 0 && std::string("Charon-Suite") != infoBuffer) {
						delete[] versionInfo;
						delete[] subBlock;
						throw PluginException("Did not load \"" + pluginName
							+ "\". The plugin contains no valid charon-core version information.", pluginName,
						PluginException::INVALID_PLUGIN_FORMAT);
					}

					hr = StringCchPrintf(subBlock, 50,
								TEXT("\\StringFileInfo\\%04x%04x\\ProductVersion"),
								lpTranslate[i].wLanguage,
								lpTranslate[i].wCodePage);
					if (FAILED(hr))
					{
						delete[] versionInfo;
						delete[] subBlock ;
						throw PluginException("Failed to load the plugin \"" + pluginName
							+ "\". The plugin contains no valid charon-core version information.", pluginName,
						PluginException::VERSION_INFORMATION_MISSING);
					}
					bufferLen = 0;
					res = VerQueryValue(versionInfo, 
									subBlock, 
									(LPVOID*)&infoBuffer, 
									&bufferLen); 
					if(res && bufferLen) {
						//parse the version string, assume it has the following form
						//major.minor.patch
						//we do a simple string comparison, as there is currently no plan for
						//upward/backward compatibility
						std::string version(infoBuffer) ;
						if(ParameteredObject::charon_core_version != version) {
							delete[] versionInfo;
							delete[] subBlock;
							throw PluginException("Plugin \"" + pluginName
								+ "\" was linked against charon-core version " + version
								+ "\n(EE)\tbut we are using charon-core version " 
								+ ParameteredObject::charon_core_version
								, pluginName,
								PluginException::VERSION_MISSMATCH);
						}
					}
					delete[] subBlock;
				}
				delete[] versionInfo;
			} //if(!_ignoreVersion)
			
			hInstLibrary = LoadLibrary(path.c_str());
			FileTool::changeDir(oldDir);
			break;
		}
	}
	if (!FileTool::exists(path)) {
		throw PluginException("Failed to load the plugin \"" + pluginName
			+ "\". The file " + pluginName
			+ ".dll could not be found. (Possible suffix: "+ libSuffix + ")",
			pluginName, PluginException::FILE_NOT_FOUND);
	}
	sout << "(DD) File: " << path << std::endl;

	if (!hInstLibrary) {
		// error loading dll file
#ifdef MSVC
		std::string addInfo;
		if (GetLastError() == 0) {
			addInfo += "(EE)\tThis is usually caused by missing dll dependencies.";
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

	create  = (ParameteredObject*(*)(const std::string&, ParameteredObject::template_type))
				GetProcAddress(hInstLibrary,"create");
	if (!create) {
		FreeLibrary(hInstLibrary);
		hInstLibrary = NULL;
#ifdef MSVC
		std::string errorMsg;
		if (GetLastError() == ERROR_PROC_NOT_FOUND) {
			errorMsg += "function \"create\" missing in dll file";
		}
		else {
			errorMsg += "Invalid plugin format.\nDescription of the error:\n";
			errorMsg += lastError("load()");
		}
		throw PluginException(
				errorMsg, pluginName,
				PluginException::INVALID_PLUGIN_FORMAT);
#else
		throw PluginException(
				"function \"create\" missing in dll file",
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
			errorMsg += "function \"destroy\" missing in dll file";
		}
		else {
			errorMsg += "Invalid plugin format.\nDescription of the error:\n";
			errorMsg += lastError("load()");
		}
		throw PluginException(
				errorMsg, pluginName,
				PluginException::INVALID_PLUGIN_FORMAT);
#else
		throw PluginException(
				"function \"destroy\" missing in dll file",
				pluginName, PluginException::INVALID_PLUGIN_FORMAT);
#endif
	}

	getBuildType  = (ParameteredObject::build_type(*)())
				GetProcAddress(hInstLibrary,"getBuildType");
	if (!getBuildType) {
		sout << "(WW) No Typecheck in Plugin " << pluginName << ":\n"
			<< "(WW) \tfunction \"getBuildType\" missing in dll file\n"
			<< "(WW) \tno checks for runtime compatibilty are perfomed"
			<< std::endl;
	}
	else
	{
		if(getBuildType() == ParameteredObject::DEBUG_BUILD)
		{
#ifndef _DEBUG
		FreeLibrary(hInstLibrary);
		hInstLibrary = NULL;
		throw PluginException(
			"This Plugin is build in DEBUG "
			"configuration while charon-core is in RELEASE Mode.\n"
			"(EE) Plugin will not be used as runtime libraries are incompatible",
			pluginName, PluginException::INCOMPATIBLE_BUILD_TYPE) ;
#endif 
		}
		else if(getBuildType() == ParameteredObject::RELEASE_BUILD)
		{
#ifdef _DEBUG
		FreeLibrary(hInstLibrary);
		hInstLibrary = NULL;
		throw PluginException(
			"This Plugin is build in RELEASE "
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
		sout << "(DD) Successfully unloaded plugin \"" << pluginName << "\"."
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
