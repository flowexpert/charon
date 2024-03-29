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
 * @file PluginManagerInterface.h
 * Provides an interface for a plugin manager.
 * @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">
 *     Cornelius Ratsch</a>
 *
 * @date 20.08.2009
 */
#ifndef PLUGINMANAGERINTERFACE_H_
#define PLUGINMANAGERINTERFACE_H_

#include "ParameteredObject.h"

/// Interface for a plugin manager.
class charon_core_PUBLIC PluginManagerInterface {
public:
	/// Get an existing instance of a loaded plugin.
	/** \param instanceName Name of the instance
	 *  \return Pointer to the requested instance
	 */
	virtual ParameteredObject*
			getInstance(const std::string & instanceName) const = 0;

	/// Recurse into object list and find connected objects.
	/** This is based on the content of the given ParameterFile,
	 *  i.e. the connected objects need not to exist really.
	 *  This is also independend of the existing connections of the
	 *  current object, it is only based on the connections saved
	 *  in the parameter file.
	 *  Nonexistent objects are created, so you have to make sure,
	 *  that the object factory is up and running.
	 *  The root object itself is also part of the connected component.
	 *  \param root         Starting point for connected object search
	 *  \param pf           ParameterFile to read connections from.
	 *  \returns            set of connected object names
	 */
	virtual std::set<std::string> getConnected (
			const std::string & root,
			const ParameterFile & pf) const = 0;

	/// Default destructor
	virtual ~PluginManagerInterface() {}

	virtual bool initializePluginOnLoad() const = 0;
	virtual void setInitiailizePluginOnLoad (bool initOnLoad) = 0;

	/// info how to handle version information
	enum PluginVersionCheckLevel {
		PluginVersionIgnore  = 0,
		PluginVersionWarn    = 1,
		PluginVersionDiscard = 2
	};

	/// Converts template_type to std::string
	/** \param t            template type to convert
	 *  \returns            string representation of t */
	virtual std::string templateTypeToString(
			ParameteredObject::template_type t) const = 0;
};

#endif /* PLUGINMANAGERINTERFACE_H_ */
