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
#include "../include/charon-core/UnixPluginLoader.h"

#ifdef USE_LIBELF
#include "../include/charon-core/configVersion.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <libelf.h>
#include <gelf.h>
#endif

UnixPluginLoader::UnixPluginLoader(
		const std::string & n,
		std::vector<std::string>& plpaths,
		std::string &lSuffix,
		PluginManagerInterface::PluginVersionCheckLevel versionCheck) :
	AbstractPluginLoader(n,plpaths,lSuffix,versionCheck),
	libHandle(NULL)
{
}

void UnixPluginLoader::load() throw (PluginException) {
	std::string path, pathS;

	// find plugin file
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
			sout << "(DD) File: " << path << std::endl;
			break;
		}
	}
	if (!FileTool::exists(path)) {
		throw PluginException("Failed to find the plugin \"" + pluginName
				+ "\". The file" + pluginName + LIBRARY_EXTENSION +
				+ " could not be found."
				+ (libSuffix.size() > 0 ?
						" (Possible suffix: " + libSuffix + ")" :
						" (suffix disabled)"),
				pluginName, PluginException::FILE_NOT_FOUND);
	}

#ifdef USE_LIBELF
	// try to determine charon-core version from plugin before dlopening
	// to avoid version mismatch crashes, this can be disabled in the
	// plugin manager configuration (PluginManager::_versionCheck)
#if defined(__x86_64__)
	typedef Elf64_Ehdr Elf_Ehdr;
	const unsigned char elfClass = ELFCLASS64;
	const std::string elfClassDesc = "64 bit";
#else
	typedef Elf32_Ehdr Elf_Ehdr;
	const unsigned char elfClass = ELFCLASS32;
	const std::string elfClassDesc = "32 bit";
#endif
	if (_versionCheck != PluginManagerInterface::PluginVersionIgnore) {
		int fd = open(path.c_str(),O_RDWR);
		if (fd < 0) {
			throw PluginException("Failed to open the plugin \"" + pluginName
				+ "\". Maybe you do not have read permissions.",
				pluginName, PluginException::FILE_DAMAGED);
		}
		struct stat fStats;
		if (fstat(fd, &fStats)) {
			close(fd);
			throw PluginException("Failed to fstat the plugin \"" + pluginName
				+ "\".", pluginName, PluginException::FILE_DAMAGED);
		}
		char* buf = new char[fStats.st_size];
		if (read(fd, buf, fStats.st_size) < fStats.st_size) {
			delete[] buf;
			close(fd);
			throw PluginException("Failed to read the plugin \"" + pluginName
				+ "\".", pluginName, PluginException::FILE_DAMAGED);
		}
		if (elf_version(EV_CURRENT) == EV_NONE) {
			sout << "(WW) Elf Library is out of date!" << std::endl;
		}
		Elf_Ehdr* eHeader = (Elf_Ehdr*) buf;
		// check ELF magic bytes
		if (
				eHeader->e_ident[EI_MAG0]!=ELFMAG0 ||
				eHeader->e_ident[EI_MAG1]!=ELFMAG1 ||
				eHeader->e_ident[EI_MAG2]!=ELFMAG2 ||
				eHeader->e_ident[EI_MAG3]!=ELFMAG3) {
			delete[] buf;
			close(fd);
			throw PluginException("ELF check failed (magic bytes): File "
				+ path + " is no ELF file.",
				pluginName, PluginException::FILE_DAMAGED);
		}
		// check for possible 32/64 bit mismatch
		if (eHeader->e_ident[EI_CLASS] != elfClass) {
			delete[] buf;
			close(fd);
			throw PluginException("ELF check failed (architecture): File "
				+ path + " is no " +  elfClassDesc + " ELF file.",
				pluginName, PluginException::FILE_DAMAGED);
		}
		// ELF version check
		if (eHeader->e_ident[EI_VERSION] != EV_CURRENT) {
			delete[] buf;
			close(fd);
			throw PluginException("ELF check failed (version): File "
				+ path + " was compiled with another ELF version.",
				pluginName, PluginException::FILE_DAMAGED);
		}
		Elf* elf = elf_begin(fd,ELF_C_READ,NULL);
		Elf_Scn* scn = 0;
		// iterate through elf sections and look for .charon-plugin
		while ((scn=elf_nextscn(elf,scn))!=0) {
			GElf_Shdr shdr;
			gelf_getshdr(scn,&shdr);
			std::string sName = elf_strptr(elf,eHeader->e_shstrndx,shdr.sh_name);
			if (sName == ".charon-plugin") {
				break;
			}
		}

		bool checkPassed = false;
		std::string checkFailMsg;
		PluginException::ErrorCode checkErrCode =
			PluginException::VERSION_INFORMATION_MISSING;

		if (!scn) {
			checkFailMsg = "No Plugin Section found in ELF file";
		}
		else {
			Elf_Data* eData = 0;
			eData = elf_getdata(scn, eData);
			// check data type and size
			if (eData->d_type != ELF_T_BYTE) {
				checkFailMsg = "Wrong Plugin Section Data Type";
			}
			else if (eData->d_size < 3) {
				checkFailMsg = "Wrong Plugin Section Data Size";
			}
			else {
				checkErrCode = PluginException::VERSION_MISSMATCH;
				const unsigned char* content =
						(const unsigned char*) eData->d_buf;
				// check version
				if (content[0] != CHARON_CORE_VERSION_MAJOR) {
					checkFailMsg = "plugin major version mismatch";
				}
				else if (content[1] != CHARON_CORE_VERSION_MINOR) {
					checkFailMsg = "plugin minor version mismatch";
				}
				else if (content[2] != CHARON_CORE_VERSION_PATCH) {
					checkFailMsg = "plugin patch version mismatch";
				}
				else {
					checkPassed = true;
					sout << "(DD) plugin ELF check successful" << std::endl;
				}
			}
		}
		delete[] buf;
		close(fd);

		if (!checkPassed) {
			switch (_versionCheck) {
			case PluginManagerInterface::PluginVersionWarn:
				sout << "(WW) ELF Check failed: " << checkFailMsg << std::endl;
				break;
			case PluginManagerInterface::PluginVersionDiscard:
				throw PluginException("ELF check failed: " + checkFailMsg,
					pluginName, checkErrCode);
				break;
			default:
				break;
			};
		}
	}
#endif

	// try to dlopen the shared library
	libHandle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
	if (!libHandle) {
		throw PluginException("Failed to load the plugin \"" + pluginName
				+ "\". Maybe the file is damaged."
				+ "\n Description of the error:\n" + dlerror(),
				pluginName, PluginException::FILE_DAMAGED);
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
			std::ostringstream msgs;
			msgs << "Error unloading plugin \"" << pluginName
				<< "\". Description of the error: " << dlerror();
			std::string msg = msgs.str();
			throw PluginException(
				msg, pluginName, PluginException::PLUGIN_NOT_LOADED);
		}
		libHandle = NULL;
		create = NULL;
		destroy = NULL;
		sout << "(DD) Successfully unloaded plugin \"" << pluginName << "\"."
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
