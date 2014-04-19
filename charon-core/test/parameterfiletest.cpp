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
/** @file   parameterfiletest.cpp
 *  @brief  Unit tests for ParameterFile class.
 *  @date   12.08.2008
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include <charon-core/ParameterFile.hxx>
#include <charon-core/ExceptionHandler.h>

/// Test application for class ParameterFile.
int test() {
	sout.assign(std::cout);
	ParameterFile* paramtest = new ParameterFile();

	// test setting and getting of values
	paramtest->set("param1", "value1 test");
	paramtest->set("param2", 10);
	std::string	val1 = paramtest->get<std::string>("param1");
	int			val2 = paramtest->get<int>("param2");
	std::cout << "val1 = " << val1 << std::endl;
	std::cout << "val2 = " << val2 << std::endl;
	if((val1 != "value1 test") || (val2 != 10))
		throw "Values do not match!";

	// modification
	paramtest->set("param2", 15);
	val2 = paramtest->get<int>("param2");
	std::cout << "val2 = " << val2 << std::endl;
	if (val2 != 15)
		throw "Failure loading modified value!";

	// information about set/modified parameters
	std::cout << "Output from showSetParams():" << std::endl;
	paramtest->showSetParams ();
	std::cout << "Size: " << paramtest->getKeyList().size() << std::endl;
	if(paramtest->getKeyList().size() != 2)
		throw "Failure loading KeyList!";

	// test save and reload
	std::cout << "Saving to file \"test.txt\"." << std::endl;
	paramtest->save("test.txt");
	delete paramtest;

	std::cout << "Reloading file" << std::endl;
	paramtest = new ParameterFile("test.txt");

	std::cout << "Checking values..." << std::flush;
	val1 = paramtest->get<std::string>("param1");
	val2 = paramtest->get<int>("param2");
	if((val1 != "value1 test") || (val2 != 15))
		throw "reloaded values do not match!";
	std::cout << "ok" << std::endl;

	// test value deletion
	std::cout << "deleting param1..." << std::flush;
	paramtest->erase("param1");
	if(paramtest->isSet("param1"))
		throw "deleted value still set!";
	if(paramtest->getKeyList().size() != 1)
		throw "length is not 1!";
	std::cout << "ok" << std::endl;

	std::cout << "deleting param2..." << std::flush;
	paramtest->erase("param2");
	if(paramtest->isSet("param2"))
		throw "deleted value still set!";
	if(paramtest->getKeyList().size() != 0)
		throw "length is not 0!";
	std::cout << "ok" << std::endl;

	std::cout << "checking mixedCase..." << std::flush;
	paramtest->set<std::string>("parMix","blah");
	if (paramtest->get<std::string>("pArMiX") != "blah") {
		throw "failed to get parameter case insensitive";
	}
	paramtest->set<std::string>("pArMix","blubb");
	if (paramtest->get<std::string>("paRMix") != "blubb") {
		throw "failed to get parameter case insensitive";
	}
	paramtest->erase("PaRmIx");
	if(paramtest->getKeyList().size() != 0)
		throw "length is not 0!";
	std::cout << "ok" << std::endl;
	std::cout << "Output from showSetParams():" << std::endl;
	paramtest->showSetParams ();

	std::cout << "checking exceptions..." << std::flush;
	bool fail = true;
	try {
		val1 = paramtest->get<std::string>("param1");
	}
	catch(const ParameterFile::Unset&) {
		fail = false;
	}
	if (fail)
		throw "no exception from get caught!";

	fail = true;
	try {
		paramtest->erase("param2");
	}
	catch(const ParameterFile::Unset&) {
		fail = false;
	}
	if (fail)
		throw "no exception from erase caught!";

	fail = true;
	try {
		paramtest->getList<float>("bla");
	}
	catch(const ParameterFile::Unset&) {
		fail = false;
	}
	if (fail)
		throw "no exception from getList caught!";

	std::cout << "ok" << std::endl;
	delete paramtest;

	std::ostringstream tmp;
	sout.assign(tmp);
	std::cout << "loading prepared parameter file..." << std::flush;
	paramtest = new ParameterFile(TEST_WRP_FILE);
	sout.assign(std::cout);
	if (paramtest->getKeyList().size() != 9) {
		throw "lenght mismatch in loaded file";
	}
	std::cout << "ok" << std::endl;
	std::cout << "Output from showSetParams():" << std::endl;
	paramtest->showSetParams ();

	std::cout << "checking content..." << std::flush;
	if (paramtest->get<std::string>("param1") != "test") {
		throw "param1 failed";
	}
	if (paramtest->get<std::string>("param2") != "test2") {
		throw "param2 failed";
	}
	if (paramtest->get<std::string>("param3") != "value3") {
		throw "param3 failed";
	}
	if (paramtest->get<int>("param4") != 10) {
		throw "param4 failed";
	}
	if (paramtest->getList<float>("param5")[2] != 7) {
		throw "param5 failed";
	}
	if (paramtest->get<std::string>("param6") != "value1;value2;value3") {
		throw "param6 failed";
	}
	if (paramtest->get<int>("mixedCaseParam") != 5) {
		throw "mixedCaseParam failed";
	}
	if (paramtest->get<std::string>("paramEmpty") != "") {
		throw "paramEmpty failed";
	}
	std::cout << "ok" << std::endl;
	std::cout << "check for duplicate warning..." << std::flush;
	std::string loadOutput = tmp.str();
	if (loadOutput.find("(WW)") != std::string::npos &&
		loadOutput.find("Duplicate Key") != std::string::npos) {
		std::cout << "ok" << std::endl;
	}
	else {
		std::cout << "failed" << std::endl;
		throw "duplicate parameter not detected correctly";
	}

	return 0;
}

/// main routine
int main() {
	return ExceptionHandler::run(test);
}
