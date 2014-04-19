/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/// @file paramObjSample.cpp
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg-de">Cornelius Ratsch</a>
/// Sample usage of parametered objects

#include <iostream>
#include <set>

#include "ParameteredObject.hxx"

template <typename T>
class SamplePlugin : public ParameteredObject {
public:
	OutputSlot<std::string> out;
	Parameter<std::string> additionalOutput;

	SamplePlugin(std::string name = "") :
		ParameteredObject("sample", name, "some sample class"),
		out(typeid(T).name()) {
		_addOutputSlot(out, "out", "String output", "string");
	}

	virtual const std::string getTemplateType() const {
		return "";
	}
};

/// sample ParameteredObject class.
class ConsoleOutput: public ParameteredObject {
public:
	InputSlot<std::string> input;

	ConsoleOutput() :
		ParameteredObject("sample", "output", "some sample class"), input(true,
				true) //make input multi-type
	{
		_addInputSlot(input, "input", "sample string input slot", "string");
	}

	virtual const std::string getTemplateType() const {
		return "";
	}

	virtual void execute() {
		ParameteredObject::execute();
		std::set<Slot *>::iterator it;
		for (it = input.getTargets().begin(); it != input.getTargets().end(); it++) {
			OutputSlot<std::string> * str = (OutputSlot<std::string> *)(*it);
			std::cout << (*str)() << std::endl;
		}
	}
};


int main() {
	SamplePlugin<ParameteredObject> t;
	ConsoleOutput o;
	o.getInputSlots().find("input")->second->connect(t.getOutputSlots().find("out")->second);
	o.execute();
}
