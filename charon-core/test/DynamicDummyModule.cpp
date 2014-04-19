/*  Copyright (C) 2012 David Stronczek

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
/** @file DynamicDummyModule.cpp
 *  DynamicDummyModule plugin class for PluginManager tests
 *  \date 12.04.2012
 *  \author <a href="mailto:stronczek@stud.uni-heidelberg.de">David Stronczek</a>
 */

/** \def dynamicdummymodule_DLL_PUBLIC dll import/export flags */
#ifdef _MSC_VER
	#ifdef dynamicdummymodule_EXPORTS
		#define dynamicdummymodule_DLL_PUBLIC __declspec(dllexport)
	#else
		#define dynamicdummymodule_DLL_PUBLIC __declspec(dllimport)
	#endif
#else
	#define dynamicdummymodule_DLL_PUBLIC
#endif

#include <charon-core/ParameteredObject.hxx>

/// dummy ParameteredObject class
class dynamicdummymodule_DLL_PUBLIC DynamicDummyModule : public ParameteredObject {
public:
	/// dummy input slots
	InputSlot<std::string>   in1, in2, in3;
	/// dummy output slots
	OutputSlot<std::string>  out1, out2, out3;
	/// parameters
	Parameter<int> param1, param2, param3, param4, param5;

	/// create a new sample object
	/// \param name             Object name
	explicit DynamicDummyModule(const std::string& name = "");

	/// update slot number
	virtual void prepareDynamicInterface(const ParameterFile& file);
protected:
	virtual void execute();
};

DynamicDummyModule::DynamicDummyModule(const std::string& name) :
		ParameteredObject("DynamicDummyModule", name, "some dummy class")
{
	_addOutputSlot(out1, "out1", "dummy output slot 1");
	_addParameter(param1, "paramNum", "Number of Parameters", 3);
	_addParameter(param2, "input", "Number of Input Slots", 0);
	_addParameter(param3, "output", "Number of Output Slots", 1);
	_setDynamic(true);
}

void DynamicDummyModule::prepareDynamicInterface(const ParameterFile& file) {
	param1.load(file);
	if (param1() >= 4) {
		_addParameter(param4, "param4", "parameter 4", 0);
	}
	if (param1() >= 5) {
		_addParameter(param5, "param5", "parameter 5", 0);
	}

	param2.load(file);
	if (param2() >= 1) {
		_addInputSlot (in1,  "in1",  "dummy input slot 1");
	}
	if (param2() >= 2) {
		_addInputSlot (in2,  "in2",  "dummy input slot 2");
	}
	if (param2() >= 3) {
		_addInputSlot (in3,  "in3",  "dummy input slot 3");
	}

	param3.load(file);
	if (param3() >= 2) {
		_addOutputSlot (out2,  "out2",  "dummy output slot 2");
	}
	if (param3() >= 3) {
		_addOutputSlot (out3,  "out3",  "dummy output slot 3");
	}
}

void DynamicDummyModule::execute() {
	sout << getName() << ".paramNum = " << param1() << std::endl;
	sout << getName() << ".input = " << param2() << std::endl;
	sout << getName() << ".output = " << param3() << std::endl;
	if (param1() >= 4) {
		sout << "param4 = " << param4() << "\n";
	}
	if (param1() >= 5) {
		sout << "param5 = " << param5() << "\n";
	}
}

/// Creates an instance of the plugin
extern "C" dynamicdummymodule_DLL_PUBLIC ParameteredObject* create(
		const std::string & name, ParameteredObject::template_type /*t*/) {
	return new DynamicDummyModule(name);
}

/// Deletes an instance of the plugin
extern "C" dynamicdummymodule_DLL_PUBLIC void destroy(ParameteredObject * b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" dynamicdummymodule_DLL_PUBLIC
	ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD;
#else
	return ParameteredObject::RELEASE_BUILD;
#endif
}
