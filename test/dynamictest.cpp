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

#include <charon-core\ParameteredObject.hxx>
#include <charon-core\ExceptionHandler.h>
#include <charon-core\SplitStream.h>

class DummyDynamicInputModule : public ParameteredObject {
public:
	InputSlot<int> in1, in2, in3, in4;

	int length;

	DummyDynamicInputModule(const std::string& name = "") :
		ParameteredObject("DummyDynamicInputModule", name, "dummy dynamic input module") {

		_addInputSlot(in1, "in1", "input slot 1");
		length = 1;
		_setDynamic(true);
	}

	virtual void prepareDynamicInterface(const ParameterFile& file) {
		if (file.isSet(getName() + ".length")) {
			length = file.get<int>(getName() + ".length");
			if (length >= 2) {
				_addInputSlot(in2, "in2", "input slot 2");
			}
		}
	}

protected:
	virtual void execute() {
		sout << "length = " << length << "\n";
	}
};

class DummyDynamicOutputModule : public ParameteredObject {
public:
	OutputSlot<int> out1, out2, out3, out4;

	int length;

	DummyDynamicOutputModule(const std::string& name = "") :
		ParameteredObject("DummyDynamicOutputModule", name, "dummy dynamic output module") {

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
	ParameteredObject::setCreateMetadata(true);

	DummyDynamicInputModule modIn("input");
	DummyDynamicOutputModule modOut("output");

	modIn.in1.connect(modOut.out1);

	modIn.run();
	sout << "\n";

	ParameterFile file;
	file.set("input.length", 2);
	file.set("output.length", 2);
	
	modIn.saveMetadata("DummyDynamicInputModule.before.wrp");
	modOut.saveMetadata("DummyDynamicOutputModule.before.wrp");

	modIn.prepareDynamicInterface(file);
	modOut.prepareDynamicInterface(file);
	
	modIn.saveMetadata("DummyDynamicInputModule.after.wrp");
	modOut.saveMetadata("DummyDynamicOutputModule.after.wrp");

	modOut.resetExecuted();
	sout << "\n";

	if (modIn.connected()) {
		throw "everything is connected!";
	}

	modIn.in2.connect(modOut.out2);
	if (modIn.connected()) {
		sout << "everything is connected\n";
	}

	modIn.run();
	sout << "\n";
}

int main() {
	sout.assign(std::cout);

	int ret = EXIT_SUCCESS;

	ret |= ExceptionHandler::run(manualTest);

	return ret;
}
