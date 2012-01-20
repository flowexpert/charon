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
/** @file PluginManager.cpp
 *  Implementation of class PluginManager.
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
 *
 *  @date 24.08.2009
 */
#include <cassert>
#include <cstdlib>
#include <set>
#include <algorithm>
#include "../include/charon-core/PluginManager.h"
#include "../include/charon-core/ParameteredObject.hxx"
#include <strstream>

std::vector<std::string> AbstractPluginLoader::pluginPaths;
std::string AbstractPluginLoader::libSuffix;

PluginManager::PluginManager(
			const std::vector<std::string>& paths, bool dbg):
		_defaultTemplateType(ParameteredObject::TYPE_DOUBLE)
{
	if(paths.size() == 0) {
		throw std::invalid_argument("PluginLoader: Empty paths list given!");
	}


	AbstractPluginLoader::pluginPaths = paths;
	AbstractPluginLoader::libSuffix = dbg ? "_d" : "";



}



PluginManager::PluginManager(
			const std::string& path1, const std::string& path2, bool dbg) :
		_defaultTemplateType(ParameteredObject::TYPE_DOUBLE)
{
    std::vector<std::string> paths;
	if (path2.size() > 0) {
		// put local path (if any) in front of global path
                paths.push_back(path2);
	}
	if (path1.size() == 0) {
		throw std::invalid_argument(
				"PluginManger: at least one non-emtpy path has to be given!");
	}
        paths.push_back(path1);
	AbstractPluginLoader::pluginPaths=paths;
	AbstractPluginLoader::libSuffix = dbg ? "_d" : "";


}

void PluginManager::_destroyAllInstances(PLUGIN_LOADER * loader) {
	std::vector<ParameteredObject *> v;

	// Collect items to destroy
	for (std::map<ParameteredObject *, PLUGIN_LOADER *>::iterator it =
			_instances.begin(); it != _instances.end(); it++) {
		if (it->second == loader) {
			v.push_back(it->first);
		}
	}

	// Destroy items
	for (unsigned int i = 0; i < v.size(); i++) {
		objects.erase(v[i]->getName());
		loader->destroyInstance(v[i]);
		_instances.erase(v[i]);
	}
}

void PluginManager::_unloadAllPlugins() {
	// destroy instances in reverse execution order
	std::list<ParameteredObject*> ordered = determineExecutionOrder();
	while (!ordered.empty()) {
		destroyInstance(ordered.back());
		ordered.pop_back();
	}
	// unload plugins
	for (std::map<std::string, PLUGIN_LOADER *>::iterator it =
			_loadedPlugins.begin(); it != _loadedPlugins.end(); it++) {
		_unloadPlugin(it->second, false);
	}
	objects.clear();
	_instances.clear();
	_loadedPlugins.clear();
}

void PluginManager::_unloadPlugin(PLUGIN_LOADER * loader, bool erase) {
	_destroyAllInstances(loader);

	loader->unload();
	if (erase) {
		_loadedPlugins.erase(loader->getName());
	}
	delete loader;
}

void PluginManager::loadPlugin(std::string name)
		throw (AbstractPluginLoader::PluginException) {
	name = StringTool::toLowerCase(name);
	if (!isLoaded(name)) {
		PLUGIN_LOADER * newPlugin = new PLUGIN_LOADER(name);

		try {
			newPlugin->load();
		} catch (const AbstractPluginLoader::PluginException& e) {
			delete newPlugin;
			throw e;
		}

		_loadedPlugins[name] = newPlugin;
		sout << "(II) Plugin \"" << name << "\" loaded successfully."
				<< std::endl;
	} else {
		throw(AbstractPluginLoader::PluginException(
				"A Plugin with the same name (\"" + name
						+ "\") is already loaded.", name,
				AbstractPluginLoader::PluginException::ALREADY_LOADED));
	}
}

void PluginManager::unloadPlugin(const std::string & name)
		throw (AbstractPluginLoader::PluginException) {
	if (_loadedPlugins.find(name) != _loadedPlugins.end()) {
		_unloadPlugin(_loadedPlugins[name]);
	} else {
		throw(AbstractPluginLoader::PluginException(
				"There is no Plugin loaded with the name \"" + name + "\".",
				name, AbstractPluginLoader::PluginException::PLUGIN_NOT_LOADED));
	}
}

bool PluginManager::isLoaded(const std::string & name) const {
	return _loadedPlugins.find(name) != _loadedPlugins.end();
}

size_t PluginManager::getLoadedPluginsCount() const {
	return _loadedPlugins.size();
}

size_t PluginManager::getInstancesCount() const {
	return _instances.size();
}

ParameteredObject * PluginManager::getInstance(
		const std::string & instanceName) const
		throw (AbstractPluginLoader::PluginException) {
	std::map<std::string, ParameteredObject *>::const_iterator objIt =
			objects.find((instanceName));
	if (objIt != objects.end()) {
		return objIt->second;
	} else {
		throw(AbstractPluginLoader::PluginException("The instance \""
				+ instanceName + "\" does not exist.", "unknown",
				AbstractPluginLoader::PluginException::NO_SUCH_INSTANCE));
	}
}

ParameteredObject * PluginManager::createInstance(
		std::string pluginName, const template_type t,
		const std::string& instanceName)
		throw (AbstractPluginLoader::PluginException) {
	pluginName = StringTool::toLowerCase(pluginName);
	if (instanceName == "" || objects.find(instanceName) == objects.end()) {
                if (_loadedPlugins.find(pluginName) == _loadedPlugins.end()) {
			try {
				loadPlugin(pluginName);
			} catch (AbstractPluginLoader::PluginException e) {
				throw e;
			}
		}
		PLUGIN_LOADER * loader = _loadedPlugins[pluginName];
		ParameteredObject * newInstance = loader->createInstance(instanceName,
				t);
		_instances[newInstance] = loader;
		objects[newInstance->getName()] = newInstance;
		sout << "(II) Created Instance \"" << newInstance->getName()
				<< "\" of the plugin \"" << pluginName << "\", type "
				<< newInstance->getTemplateType() << std::endl;
		return newInstance;
	} else {
		throw(AbstractPluginLoader::PluginException(
			"An instance with the name \"" + instanceName
					+ "\" already exists.", pluginName,
			AbstractPluginLoader::PluginException::DUPLICATE_INSTANCE_NAME));
	}
}

ParameteredObject * PluginManager::createInstance(
		const std::string & pluginName, const std::string & instanceName)
		throw (AbstractPluginLoader::PluginException) {
	try {
		return createInstance(pluginName, _defaultTemplateType, instanceName);
	} catch (AbstractPluginLoader::PluginException e) {
		throw e;
	}
}

void PluginManager::destroyInstance(ParameteredObject* toDestroy)
		throw (AbstractPluginLoader::PluginException) {
	std::string cur = toDestroy->getName(), curPlugin;
	if (_instances.find(toDestroy) != _instances.end()) {
		objects.erase(toDestroy->getName());
		curPlugin = _instances[toDestroy]->getName();
		_instances[toDestroy]->destroyInstance(toDestroy);
		_instances.erase(toDestroy);
	} else {
		throw(AbstractPluginLoader::PluginException(
				"This instance does not exist.", "unknown",
				AbstractPluginLoader::PluginException::NO_SUCH_INSTANCE));
	}
	sout << "(II) Deleted Instance \"" << cur << "\" of the plugin \""
		<< curPlugin << "\"" << std::endl;
}

void PluginManager::loadParameterFile(const ParameterFile & paramFile) {
	reset();

	// Determine default template type
	if (paramFile.isSet("global.templatetype")) {
		std::string templateType = paramFile.get<std::string> (
				"global.templatetype");
		if (templateType == "int") {
			_defaultTemplateType = ParameteredObject::TYPE_INT;
		} else if (templateType == "float") {
			_defaultTemplateType = ParameteredObject::TYPE_FLOAT;
		} else {
			_defaultTemplateType = ParameteredObject::TYPE_DOUBLE;
		}
	}

	std::vector<std::string> keys = paramFile.getKeyList();

	try {
		// Load Plugins and create _instances
		for (unsigned int i = 0; i < keys.size(); i++) {
			if (keys[i].substr(keys[i].find_last_of(".") + 1,
					keys[i].find_first_of(" ")) == "type") {
				std::string pluginName = paramFile.get<std::string> (keys[i]);
                                if (!isLoaded(pluginName)) {
                                        loadPlugin(pluginName);
                                }
                                std::string instanceName = keys[i].substr(0,
                                                keys[i].find_first_of("."));

                                template_type templateType = _defaultTemplateType;
                                if (paramFile.isSet(instanceName + ".templatetype")) {
                                        std::string type = paramFile.get<std::string> (
                                                instanceName + ".templatetype");
                                        if (type == "int") {
                                                templateType = ParameteredObject::TYPE_INT;
                                        } else if (type == "float") {
                                                templateType = ParameteredObject::TYPE_FLOAT;
                                        } else {
                                                templateType = ParameteredObject::TYPE_DOUBLE;
                                        }
                                }
                                createInstance(pluginName, templateType, instanceName);
			}
		}
	} catch (AbstractPluginLoader::PluginException e) {
		sout << "(EE) Error during load: " << e.what() << std::endl;
	} catch (std::string s) {
		sout << "(EE) Error during load: " << s << std::endl;
	} catch (...) {
		sout << "(EE) caught unknown exception during load" << std::endl;
	}

	// load parameters and connect slots for all objects
	std::map<std::string, ParameteredObject*>::const_iterator objIter;

	for (objIter = objects.begin(); objIter != objects.end(); objIter++) {
		objIter->second->_load(paramFile, this);
	}
}
void PluginManager::loadParameterFile(const std::string & path) {
	loadParameterFile(ParameterFile(path));
}

void PluginManager::saveParameterFile(ParameterFile & pf) const {
	pf.set<std::string> ("global.templatetype",
			ParameteredObject::templateTypeToString(_defaultTemplateType));

	// load list of all connected objects
	if (objects.size() > 0) {
		std::map<std::string, ParameteredObject*>::const_iterator objIter;

		// save parameters and slots for all these objects
		for (objIter = objects.begin(); objIter != objects.end(); objIter++) {
			//Save template type parameter if unqeual to _defaultTemplateType
			if (objIter->second->getTemplateType()
					!= ParameteredObject::templateTypeToString(
							_defaultTemplateType)) {
				pf.set<std::string> (objIter->second->getName()
						+ ".templatetype", objIter->second->getTemplateType());
			}
			//Save other parameters and slots
			objIter->second->save(pf);
		}
	}
}

void PluginManager::saveParameterFile(const std::string & path) const {
	ParameterFile pf;
	saveParameterFile(pf);
	pf.save(path);
}

template_type PluginManager::getDefaultTemplateType() const {
	return _defaultTemplateType;
}

void PluginManager::setDefaultTemplateType(const template_type t) {
	if (t < 3) {
		_defaultTemplateType = t;
	}
}

void PluginManager::executeWorkflow() {
	std::list<ParameteredObject*> tPoints = _determineTargetPoints();
	std::list<ParameteredObject*>::const_iterator iter;

	if (tPoints.empty()) {
		throw AbstractPluginLoader::PluginException(
			"Could not execute workflow:\n\t"
			"No valid target point found.\n\tPlease check if "
			"all required plugins could be loaded,\n\tthen check if this is "
			"a valid parameter file.", "unknown",
			AbstractPluginLoader::PluginException::OTHER);
	}

	for (iter = tPoints.begin(); iter != tPoints.end(); iter++) {
//            if(isGroup((*iter)))
//            {
//                AbstractBaseGroupIntf* grpIntf=dynamic_cast<AbstractBaseGroupIntf*>((*iter));
//                grpIntf->loadWorkflow(AbstractPluginLoader::pluginPaths);
//            }
		(*iter)->run();
	}
}

void PluginManager::_getConnected(std::set<std::string>& visited,
		const std::set<std::string>& cur) const {
	// nothing to do, if current list empty
	if (!cur.size())
		return;

	std::set<std::string>::const_iterator curIter;
	std::set<std::string>::const_iterator tmpIter;
	std::set<std::string> newCur;
	std::set<std::string> tmp;

	// mark current objects as visited
	visited.insert(cur.begin(), cur.end());

	// iterate over all current items
	for (curIter = cur.begin(); curIter != cur.end(); curIter++) {
		// search new neighbours (which are not yet visited)
		tmp = getNeighbours(*curIter);
		for (tmpIter = tmp.begin(); tmpIter != tmp.end(); tmpIter++) {
			if (visited.find(*tmpIter) == visited.end())
				newCur.insert(*tmpIter);
		}
	}
	// and apply the same procedure to these new items
	_getConnected(visited, newCur);
}

void PluginManager::_getConnected(std::set<std::string>& visited,
		const std::set<std::string>& cur, const ParameterFile& pf) const {
	// nothing to do, if current list empty
	if (!cur.size())
		return;

	std::set<std::string>::const_iterator curIter;
	std::set<std::string>::const_iterator tmpIter;
	std::set<std::string> newCur;
	std::set<std::string> tmp;

	// mark current objects as visited
	visited.insert(cur.begin(), cur.end());

	// iterate over all current items
	for (curIter = cur.begin(); curIter != cur.end(); curIter++) {
		// search new neighbours (which are not yet visited)
		tmp = getNeighbours(*curIter, pf);
		for (tmpIter = tmp.begin(); tmpIter != tmp.end(); tmpIter++) {
			if (visited.find(*tmpIter) == visited.end())
				newCur.insert(*tmpIter);
		}
	}
	// and apply the same procedure to these new items
	_getConnected(visited, newCur, pf);
}

std::set<std::string> PluginManager::getNeighbours(
		const std::string& rootStr) const {
	std::map<std::string, ParameteredObject*>::const_iterator rootIter;
	rootIter = objects.find(rootStr);
	assert(rootIter != objects.end());
	const ParameteredObject* root = rootIter->second;
	return root->getNeighbours();
}

std::set<std::string> PluginManager::getNeighbours(
		const std::string& rootStr,
		const ParameterFile& pf) const {
	std::map<std::string, ParameteredObject*>::const_iterator rootIter;
	rootIter = objects.find(rootStr);
	assert(rootIter != objects.end());
	const ParameteredObject* root = rootIter->second;
	return root->getNeighbours(pf);
}

std::set<std::string> PluginManager::getConnected(
		const std::string& root,
		const ParameterFile& pf) const {
	std::set<std::string> visited;
	visited.insert(root);
	std::set<std::string> cur = getNeighbours(root, pf);
	_getConnected(visited, cur, pf);
	return visited;
}

std::set<std::string> PluginManager::getConnected(
		const std::string& root) const {
	std::set<std::string> visited;
	visited.insert(root);
	std::set<std::string> cur = getNeighbours(root);
	_getConnected(visited, cur);
	return visited;
}

bool PluginManager::connect(Slot& slot1, Slot& slot2) {
	ParameteredObject* obj1 = &slot1.getParent();
	ParameteredObject* obj2 = &slot2.getParent();
	std::string obj1sl = slot1.getName();
	std::string obj2sl = slot2.getName();

	// connect those objects
	bool ret = obj1->_connect(obj2, obj1sl, obj2sl);
	ret = obj2->_connect(obj1, obj2sl, obj1sl) && ret;
	return ret;
}

bool PluginManager::connect(Slot * slot1, Slot * slot2) {
	return connect(*slot1, *slot2);
}

bool PluginManager::connect(
		const std::string& slot1, const std::string& slot2) {
	// extract objects and slots
	std::string::size_type pos = slot1.find(".");
	assert(pos != std::string::npos);
	std::string obj1st = slot1.substr(0, pos);
	std::string obj1sl = slot1.substr(pos + 1);

	pos = slot2.find(".");
	assert(pos != std::string::npos);
	std::string obj2st = slot2.substr(0, pos);
	std::string obj2sl = slot2.substr(pos + 1);

	// find the corresponding objects
	std::map<std::string, ParameteredObject*>::const_iterator objIter;
	objIter = objects.find(obj1st);
	assert(objIter != objects.end());
	ParameteredObject* obj1 = objIter->second;
	objIter = objects.find(obj2st);
	assert(objIter != objects.end());
	ParameteredObject* obj2 = objIter->second;

	// connect those objects
	bool ret = obj1->_connect(obj2, obj1sl, obj2sl);
	ret = obj2->_connect(obj1, obj2sl, obj1sl) && ret;
	return ret;
}

bool PluginManager::disconnect(Slot& slot1, Slot& slot2) {
	ParameteredObject* obj1 = &slot1.getParent();
	ParameteredObject* obj2 = &slot2.getParent();
	std::string obj1sl = slot1.getName();
	std::string obj2sl = slot2.getName();

	// disconnect those objects
	bool ret = obj1->_disconnect(obj2, obj1sl, obj2sl);
	ret = obj2->_disconnect(obj1, obj2sl, obj1sl) && ret;
	return ret;
}

bool PluginManager::disconnect(const std::string& slot1,
		const std::string& slot2) {
	// extract objects and slots
	std::string::size_type pos = slot1.find(".");
	assert(pos != std::string::npos);
	std::string obj1st = slot1.substr(0, pos);
	std::string obj1sl = slot1.substr(pos + 1);

	pos = slot2.find(".");
	assert(pos != std::string::npos);
	std::string obj2st = slot2.substr(0, pos);
	std::string obj2sl = slot2.substr(pos + 1);

	// find the corresponding objects
	std::map<std::string, ParameteredObject*>::const_iterator objIter;
	objIter = objects.find(obj1st);
	assert(objIter != objects.end());
	ParameteredObject* obj1 = objIter->second;
	objIter = objects.find(obj2st);
	assert(objIter != objects.end());
	ParameteredObject* obj2 = objIter->second;

	// connect those objects
	bool ret = obj1->_disconnect(obj2, obj1sl, obj2sl);
	ret = obj2->_disconnect(obj1, obj2sl, obj1sl) && ret;
	return ret;
}

void PluginManager::createMetadata(const std::string& targetPath) {
	// Backup current configuration
	// (Working directory and metadata creation preference)
	bool wasEnabled = ParameteredObject::getCreateMetadata();
	ParameteredObject::setCreateMetadata(true);
	std::string pathBackup = FileTool::getCurrentDir();

#ifndef MSVC
	int start = 3;
#else
	int start = 0;
#endif

	// avoid double metadata generation (e.g. if lib and lib_d found)
	std::set<std::string> pluginsU;

	// Create metadata for all plugin paths

	for (std::vector<std::string>::const_iterator cur =
			AbstractPluginLoader::pluginPaths.begin();
			cur!=AbstractPluginLoader::pluginPaths.end(); cur++) {
		FileTool::changeDir(*cur);

		// Fetch list of existing plugins
		std::vector<std::string> plugins = FileTool::getFilesWithSuffix(
				PLUGIN_EXTENSION);
		std::vector<std::string>::iterator pIterW;
		for (pIterW=plugins.begin(); pIterW != plugins.end(); pIterW++) {
			// extract plugin name
			std::string& pName = *pIterW;
			pName = pName.substr(start, pName.find_last_of('.')-start);
			// strip debug extension, if any
			if (pName.substr(pName.size()-2) == "_d") {
				pName = pName.substr(0, pName.size()-2);
			}
		}
		// insert plugin names to plugin name set
		pluginsU.insert(plugins.begin(),plugins.end());
	}

	// Metadata shall be created in the plugin folder
	if (!targetPath.empty()) {
		FileTool::changeDir(targetPath);
	}

	// now generate metadata for all (unique) plugin names
	// which file is now used is handled by the plugin loader
	std::set<std::string>::const_iterator pIterU;
	for (pIterU=pluginsU.begin(); pIterU != pluginsU.end(); pIterU++) {
		_createMetadataForPlugin(*pIterU);
	}

	// restore former configuration
	FileTool::changeDir(pathBackup);
	ParameteredObject::setCreateMetadata(wasEnabled);
}

void PluginManager::_createMetadataForPlugin(const std::string& pluginName) {
	if (!pluginName.size()) {
		sout << "(EE) " << __FILE__ << ":" << __LINE__ << "\t"
			<< "emtpy pluginName given (metadata generation)!\n" << std::endl;
		return;
	}
	static std::vector<std::string> excludeList;
	if (excludeList.size() == 0) {
#ifdef _MSC_VER
		excludeList.push_back("charon-core");
		excludeList.push_back("msvc");
		excludeList.push_back("Qt");
		excludeList.push_back("phonon");
		excludeList.push_back("libpng");
		excludeList.push_back("libtiff");
		excludeList.push_back("zlib");
		excludeList.push_back("vigraimpex");
		excludeList.push_back("dll");
#endif
	}
	std::vector<std::string>::const_iterator iter;
	for(iter = excludeList.begin(); iter != excludeList.end(); iter++) {
		if (pluginName.find(*iter)!=std::string::npos) {
			sout << "(II) Discarding non-plugin file \"" << pluginName
				<< ".dll\" (matched pattern \"*" << *iter
				<< "*\" of exclude list)\n" << std::endl;
			return;
		}
	}
	try {
		bool alreadyLoaded = isLoaded(pluginName);

		if (!alreadyLoaded) {
			loadPlugin(pluginName);
		}
		destroyInstance(createInstance(pluginName));

		if (!alreadyLoaded) {
			unloadPlugin(pluginName);
		}
	} catch (const AbstractPluginLoader::PluginException& e) {
		switch(e.getErrorCode()) {
		case AbstractPluginLoader::PluginException::INVALID_PLUGIN_FORMAT:
			sout << "(WW) \"" << e.getPluginName()
				<< "\" is no charon plugin:\n(WW) \t"
				<< e.what() << std::endl;
			break;
		default:
			sout << "(EE) Exception during metadata generation of \""
				<< e.getPluginName()
				<< "\":\n(EE) \t"
				<< e.what() << std::endl;
			break;
		}
	}
	sout << std::endl;
}

void PluginManager::reset() {
	_unloadAllPlugins();
	_defaultTemplateType = ParameteredObject::TYPE_DOUBLE;

}

std::list<ParameteredObject*> PluginManager::_determineTargetPoints() {
	std::list<ParameteredObject*> targetPoints;
	std::map<std::string, ParameteredObject *>::const_iterator it;
	for (it = objects.begin(); it != objects.end(); it++) {
		bool hasChildren = false;
		std::map<std::string, Slot *> outputslots =
				it->second->getOutputSlots();
		std::map<std::string, Slot *>::const_iterator slotIter;
		for (slotIter = outputslots.begin(); !hasChildren && slotIter
				!= outputslots.end(); slotIter++) {
			hasChildren = slotIter->second->connected();
		}
		if (!hasChildren) {
			sout << "(II) Found target point \"" << it->second->getName()
					<< "\"" << std::endl;
			targetPoints.push_back(it->second);
		}
	}
	return targetPoints;
}

std::list<ParameteredObject*> PluginManager::determineExecutionOrder() {
	std::list<ParameteredObject*> waiting = _determineTargetPoints();
	std::reverse(waiting.begin(), waiting.end());
	std::list<ParameteredObject*> finished;
	ParameteredObject* cur;

	// start with target points as sinks
	while(!waiting.empty()) {
		// mark object as finished and replace
		// by preceeding objects in waiting stack
		finished.push_front(cur = waiting.front());
		waiting.pop_front();
		const std::map<std::string, Slot*>& inputs = cur->getInputSlots();
		std::map<std::string, Slot*>::const_iterator sIter;
		std::set<Slot*>::const_iterator tIter;
		for (sIter = inputs.begin(); sIter != inputs.end(); sIter++) {
			const std::set<Slot*>& targets = sIter->second->getTargets();
			for (tIter = targets.begin(); tIter != targets.end(); tIter++) {
				waiting.push_front(&((*tIter)->getParent()));
			}
		}
	}

	// now remove duplicate entries, keeping the first one
	std::list<ParameteredObject*>::iterator iBegin = finished.begin();
	std::list<ParameteredObject*>::iterator iEnd = finished.end();
	std::list<ParameteredObject*>::iterator iter = iBegin;
	while (iter != iEnd) {
		const ParameteredObject* curIt = *iter;
		iEnd = std::remove(++iter, iEnd, curIt);
	}

	// remove tail that may contain bogus elements
	if (iEnd != finished.end())
		finished.erase(iEnd, finished.end());

	// return resulting list
	return finished;
}

PluginManager::~PluginManager() {
	reset();
}

void PluginManager::execute() {
	this->executeWorkflow();
}

const std::vector<std::string>& PluginManager::getPluginPaths() const {

    return AbstractPluginLoader::pluginPaths;
}






