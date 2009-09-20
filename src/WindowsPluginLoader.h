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
 * @file UnixPluginLoader.h
 * Implements a plugin Loader for Windows systems.
 * @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">
 *     Cornelius Ratsch</a>
 *
 * @date 20.08.2009
 */
#ifndef WINDOWSPLUGINLOADER_H_
#define WINDOWSPLUGINLOADER_H_

#include <windows.h>
#include "AbstractPluginLoader.h"

/**
 * Handles ParameteredObject based plugins on Windows based systems.
 * The UnixPluginLoader can load plugins based on the ParameteredObject class
 * on Windows systems. It it based on the abstract class AbstractPluginLoader.
 * You usually don't need to use this class directly. The PluginManager class
 * handles this for you.
 * @see AbstractPluginLoader
 * @see UnixPluginLoader
 * @see PluginManager
 */
class WindowsPluginLoader: public AbstractPluginLoader {
private:
	/**
	 * Points to the loaded library
	 */
	HINSTANCE hInstLibrary;

	virtual std::string _pathsConfig() const;
#ifdef MSVC
	/**
	 * @return Last error message as string
	 * @todo Currently only works with the Visual C++ compiler.
	 *       This is because the MinGW compiler doesn't know the strsafe.h.
	 *       Must be reimplemented for the MinGW compiler
	 */
	LPCTSTR lastError(LPTSTR) const;
#endif
public:
	/**
	 * Default constructor.
	 * @param name Name of the plugin
	 */
	WindowsPluginLoader(const std::string & name);
	virtual void load() throw (PluginException);
	virtual void compileAndLoad(const std::string & sourceFile,
			std::vector<std::string> &references, const std::string & metadataPath = "") throw (PluginException);
	virtual void unload() throw (PluginException);

	/**
	 * Default destructor.
	 * Unloads the plugin from the memory.
	 */
	virtual ~WindowsPluginLoader();
};

#endif /* WINDOWSPLUGINLOADER_H_ */
