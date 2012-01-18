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
/** @file AbstractPluginLoader.cpp
 *  Implementation of class AbstractPluginLoader.
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
 *
 *  @date 24.08.2009
 */
#include "../include/charon-core/AbstractPluginLoader.h"

AbstractPluginLoader::AbstractPluginLoader(const std::string & n) :
	pluginName(StringTool::toLowerCase(n)) {
	create = NULL;
	destroy = NULL;
}

const std::string& AbstractPluginLoader::getName() const {
	return pluginName;
}

ParameteredObject * AbstractPluginLoader::createInstance(
		const std::string & name, template_type t) throw (PluginException) {
	if (create) {
		return create(name, t);
	} else {
		throw PluginException("Plugin \"" + pluginName + "\" is not loaded.",
				pluginName, PluginException::PLUGIN_NOT_LOADED);
	}
}

void AbstractPluginLoader::destroyInstance(ParameteredObject * toDestroy)
		throw (PluginException) {

	if (destroy) {
		destroy(toDestroy);
	} else {
		throw PluginException("Plugin \"" + pluginName + "\" is not loaded.",
				pluginName, PluginException::PLUGIN_NOT_LOADED);
	}
}

AbstractPluginLoader::~AbstractPluginLoader() {
}

AbstractPluginLoader::PluginException::PluginException(const std::string & n,
		const std::string & pl, error_code e) :
	runtime_error(n), nameOfPlugin(pl), err(e) {
}

const std::string & AbstractPluginLoader::PluginException::getPluginName() const {
	return nameOfPlugin;
}

error_code AbstractPluginLoader::PluginException::getErrorCode() const {
	return err;
}

AbstractPluginLoader::PluginException::~PluginException() throw () {
}
