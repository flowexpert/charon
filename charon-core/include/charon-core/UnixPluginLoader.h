/*  This file is part of Charon.

 Charon is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Charon is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Charon.  If not, see <http://www.gnu.org/licenses/>.
 */
/**
 * @file UnixPluginLoader.h
 * Implements a plugin Loader for Unix systems.
 * @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">
 *     Cornelius Ratsch</a>
 *
 * @date 20.08.2009
 */
#ifndef LINUXPLUGINLOADER_H_
#define LINUXPLUGINLOADER_H_

#include "AbstractPluginLoader.h"

/**
 * Handles ParameteredObject based plugins on Unix based systems.
 * The UnixPluginLoader can load plugins based on the ParameteredObject class
 * on Unix systems like Linux or Mac OS X. It it based on the abstract class
 * AbstractPluginLoader.
 * You usually don't need to use this class directly. The PluginManager class
 * handles this for you.
 * @see AbstractPluginLoader
 * @see WindowsPluginLoader
 * @see PluginManager
 */
class charon_plugins_DLL_PUBLIC UnixPluginLoader: public AbstractPluginLoader {
private:
	/**
	 * Points to the loaded library
	 */
	void * libHandle;

public:
	/**
	 * Default constructor.
	 * \copydetails AbstractPluginLoader::AbstractPluginLoader(
	 *     const std::string&, std::vector<std::string>&,
	 *     std::string&,PluginManagerInterface::VersionInfo)
	 */
	UnixPluginLoader(
			const std::string& name,
			std::vector<std::string>& plpaths,
			std::string& lSuffix,
			PluginManagerInterface::PluginVersionCheckLevel versionCheck =
					PluginManagerInterface::PluginVersionIgnore);
	virtual void load() throw (PluginException);
	virtual void unload() throw (PluginException);

	/**
	 * Default destructor.
	 * Unloads the plugin from the memory
	 */
	virtual ~UnixPluginLoader();
};

#endif /* LINUXPLUGINLOADER_H_ */
