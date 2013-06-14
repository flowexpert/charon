/*  Copyright (C) 2012 David Stronczek

	This file is part of Tuchulcha.

	Tuchulcha is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Tuchulcha is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file   dynamictest.cpp
 *  \brief  Test for Dynamic Modules
 *  \date   11.04.2012
 *  \author <a href="mailto:stronczek@stud.uni-heidelberg.de">David Stronczek</a>
 */

#include <cstdlib>
#include <iostream>

#include <charon-core/ParameteredObject.hxx>
#include <charon-core/ExceptionHandler.h>
#include <charon-core/SplitStream.h>
#include <charon-core/PluginManager.h>

class DummyDynamicInputModule : public ParameteredObject {
public:
	InputSlot<int> in1, in2, in3, in4;

	Parameter<int> param1, param2, param3, param4;

	DummyDynamicInputModule(const std::string& name = "") :
		ParameteredObject("DummyDynamicInputModule", name,
			"dummy dynamic input module") {

		_addInputSlot(in1, "in1", "input slot 1");
		_addParameter(param1, "parameters", "Number of Parameters", 2);
		_addParameter(param2, "length", "Number of Slots", 1);
		_setDynamic(true);
	}

	virtual void prepareDynamicInterface(const ParameterFile& file) {
		param2.load(file);
		if (param2() >= 2) {
			_addInputSlot(in2, "in2", "input slot 2");
		}
		param1.load(file);
		if (param1() >= 3) {
			_addParameter(param3, "param3", "parameter 3", 0);
		}
		if (param1() >= 4) {
			_addParameter(param4, "param4", "parameter 4", 0);
		}
	}

protected:
	virtual void execute() {
		sout << "parameters = " << param1() << "\n";
		sout << "length = " << param2() << "\n";
		if (param1() >= 3) {
			sout << "param3 = " << param3() << "\n";
		}
		if (param1() >= 4) {
			sout << "param4 = " << param4() << "\n";
		}
	}
};

class DummyDynamicOutputModule : public ParameteredObject {
public:
	OutputSlot<int> out1, out2, out3, out4;

	int length;

	DummyDynamicOutputModule(const std::string& name = "") :
		ParameteredObject("DummyDynamicOutputModule", name,
			"dummy dynamic output module") {

		_addOutputSlot(out1, "out1", "output slot 1");
		length = 1;
		_setDynamic(true);
	}

	virtual void prepareDynamicInterface(const ParameterFile& file) {
		if (file.isSet(getName() + ".length")) {
			length = file.get<int>(getName() + ".length");
			if (length >= 2) {
				_addOutputSlot(out2, "out2", "output slot 2");
			}
		}
	}

protected:
	virtual void execute() {
		sout << "length = " << length << "\n";
	}
};

void manualTest() {
	sout << "### Manual Test ###" << std::endl << std::endl;

	DummyDynamicInputModule modIn("input");
	DummyDynamicOutputModule modOut("output");

	modIn.in1.connect(modOut.out1);

	modIn.run();
	sout << std::endl;

	ParameterFile file;
	file.set("input.type", "DummyDynamicInputModule");
	file.set("input.length", 2);
	file.set("output.length", 2);
	file.set("input.parameters", 4);
	file.set("input.param4", 987);
	
	modIn.getMetadata().save("DummyDynamicInputModule.before.wrp");
	modOut.getMetadata().save("DummyDynamicOutputModule.before.wrp");

	modIn.prepareDynamicInterface(file);
	modIn.loadParameters(file);
	modOut.prepareDynamicInterface(file);
	
	modIn.getMetadata().save("DummyDynamicInputModule.after.wrp");
	modOut.getMetadata().save("DummyDynamicOutputModule.after.wrp");

	modOut.resetExecuted();
	sout << std::endl;

	if (modIn.connected()) {
		throw "everything is connected!";
	}

	modIn.in2.connect(modOut.out2);
	if (modIn.connected()) {
		sout << "everything is connected" << std::endl;
	}

	modIn.run();
	sout << std::endl << "### End of Test ###" << std::endl << std::endl;
}

void pluginManagerTest() {
	sout << "### PluginManager Test ###" << std::endl << std::endl;

	PluginManager* man = new PluginManager(MODULE_DIR);

	ParameterFile file;
	// setup a dummy module that gets data from dynamic dummy module
	file.set("dynamic.type", "dynamicdummymodule");
	file.set("dummy.type", "dummymodule");
	file.set("dynamic.input", 0);
	file.set("dynamic.output", 1);
	file.set("dynamic.out1", "dummy.in");
	file.set("dummy.in", "dynamic.out1");

	man->loadParameterFile(file);

	sout << std::endl;
	man->runWorkflow();
	sout << std::endl;

	sout << "~~~~~~~" << std::endl;
	
	// add dynamic dummy module and connect it to first dynamic module
	file.set("dynamic2.type", "dynamicdummymodule");
	file.set("dynamic.input", 2);
	file.set("dynamic2.input", 0);
	file.set("dynamic2.output", 2);
	file.set("dynamic2.parameters", 5);
	file.set("dynamic2.param4", 999);
	file.set("dynamic.in1", "dynamic2.out1");
	file.set("dynamic2.out1", "dynamic.in1");
	file.set("dynamic.in2", "dynamic2.out2");
	file.set("dynamic2.out2", "dynamic.in2");

	man->loadParameterFile(file);

	sout << std::endl;
	man->runWorkflow();
	sout << std::endl;

	delete man;

	sout << std::endl << "### End of Test ###" << std::endl << std::endl;
}

int main() {
	sout.assign(std::cout);

	int ret = EXIT_SUCCESS;

	ret |= ExceptionHandler::run(manualTest);
	ret |= ExceptionHandler::run(pluginManagerTest);

	return ret;
}
