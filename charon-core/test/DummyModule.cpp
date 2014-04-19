/*  Copyright (C) 2011 Jens-Malte Gottfried

	This file is part of Charon.

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
/** @file DummyModule.cpp
 *  DummyModule plugin class for PluginManager tests
 *  \date 15.09.2011
 *  \author <a href="mailto:jmgottfried@web.de>Jens-Malte Gottfried</a>
 */

/** \def dummymodule_DLL_PUBLIC dll import/export flags */
#ifdef _MSC_VER
	#define dummymodule_DLL_PUBLIC __declspec(dllexport)
#else
	#define dummymodule_DLL_PUBLIC
#endif

#include <charon-core/ParameteredObject.hxx>

/// dummy ParameteredObject class
class dummymodule_DLL_PUBLIC DummyModule : public ParameteredObject {
public:
	/// dummy input
	InputSlot<std::string>   in;
	/// dummy output
	OutputSlot<std::string>  out;

	/// create a new sample object
	/// \param name             Object name
	explicit DummyModule(const std::string& name = "");
protected:
	virtual void execute() {}
};

DummyModule::DummyModule(const std::string& name) :
		ParameteredObject("DummyModule", name, "some dummy class"),
		in(true, true)    // optional multislot
{
	// slots
	_addInputSlot (in,  "in",  "dummy input");
	_addOutputSlot(out, "out", "dummy output");
}

/// Creates an instance of the plugin
extern "C" dummymodule_DLL_PUBLIC ParameteredObject* create(
		const std::string & name, ParameteredObject::template_type /*t*/) {
	return new DummyModule(name);
}

/// Deletes an instance of the plugin
extern "C" dummymodule_DLL_PUBLIC void destroy(ParameteredObject * b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" dummymodule_DLL_PUBLIC
	ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}
