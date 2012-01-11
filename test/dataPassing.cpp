/*  Copyright (C) 2011 Jens-Malte Gottfried

	This file is part of Charon.

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
/// @file dataPassing.cpp
/// Tests for Slot data passing mechanisms.

#include <cstdlib>
#include <iostream>

#include <charon-core/ParameteredObject.hxx>
#include <charon-core/ExceptionHandler.h>
#include <charon-core/DataManagerParameterFile.hxx>

// make sure that assert() works (nothing is tested otherwise)
#undef NDEBUG
#include <cassert>

/// sample ParameteredObject class.
class Reader : public ParameteredObject {
public:
	/// sample integer input slot
	InputSlot<int>   in1;
	/// sample float input slot
	InputSlot<float> in2;

	/// create a new sample object
	/// @param name             Object name
	Reader(const std::string& name = "") :
			ParameteredObject("Reader", name, "read slot data")
	{
		_addInputSlot (in1,  "in1",  "integer input slot");
		_addInputSlot (in2,  "in2",  "float input slot");
	}

protected:
	virtual void execute() {
		sout << "(II) \tRead slot data: "
			 << in1() << ", " << in2() << std::endl;
	}
};

/// sample ParameteredObject class.
class Outputgen : public ParameteredObject {

public:
	/// sample integer output slot
	OutputSlot<int>    out1;
	/// sample float output slot
	OutputSlot<float>  out2;

	/// create a new sample object
	/// @param name             Object name
	Outputgen(const std::string& name = "") :
			ParameteredObject("outputgen", name,
				"class to generate int and float output"),
			out1(10),   // set initial values
			out2(5.5f)  // ------- " --------
	{
		// slots
		_addOutputSlot(out1, "out1", "integer output slot");
		_addOutputSlot(out2, "out2", "float output slot");
	}

protected:
	virtual void execute() {
	}
};

int testDataManager() {
	bool success = true;
	Outputgen outGen;
	Slot::DataManager<int>* manInt =
			Slot::DataManagerFactory<int>::getManager(outGen.out1);
	if (!manInt) {
		sout << "(EE) Factory cannot handle int data" << std::endl;
		return EXIT_FAILURE;
	}
	const std::string& conf = manInt->getConfig();
	sout << "(II) manInt config: " << conf << std::endl;
	const int testData = 0x1234abcd;
	manInt->setData(testData);
	if (manInt->getData() != testData) {
		sout << "(EE) test data could not be read correctly" << std::endl;
		success = false;
	}
	delete manInt;
	manInt = Slot::DataManagerFactory<int>::getManager(outGen.out1, conf);
	if (manInt->getData() != testData) {
		sout << "(EE) test data could not be re-read correctly" << std::endl;
		success = false;
	}
	delete manInt;
	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

/// Main test application.
int test() {
	bool success = true;
	bool excpt = false;

	sout << "\ndry run (memory, as usual):" << std::endl;
	ohOutputgen outGen;
	Reader reader;
	reader.in1.connect(outGen.out1);
	reader.in2.connect(outGen.out2);
	reader.run();

	sout << "\ncheck type-check on setCache" << std::endl;
	excpt = false;
	try {
		outGen.out1.setCacheType((Slot::CacheType)127);
	}
	catch (const std::runtime_error& err) {
		sout << "(II) Caught expected exception. Message:\n(II) \t"
			<< err.what() << std::endl;
		excpt = true;
	}
	if (!excpt) {
		sout << "(EE) Expected runtime error exception!" << std::endl;
		success = false;
	}

	sout << "\nwith invalid cache (should fail):" << std::endl;
	outGen.resetExecuted();
	outGen.out1.setCacheType(Slot::CACHE_INVALID);

	excpt = false;
	try {
		reader.run();
	}
	catch (const std::runtime_error& err) {
		sout << "(II) Caught expected exception. Message:\n(II) \t"
			<< err.what() << std::endl;
		excpt = true;
	}
	if (!excpt) {
		sout << "(EE) Expected runtime error exception!" << std::endl;
		success = false;
	}

	sout << "\nwith managed cache:" << std::endl;
	outGen.resetExecuted();
	outGen.out1.setCacheType(Slot::CACHE_MANAGED);
	reader.run();

	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

int main() {
	sout.assign(std::cout);
	int ret = EXIT_SUCCESS;
	ret |= ExceptionHandler::run(testDataManager);
	ret |= ExceptionHandler::run(test);
	return ret;
}

