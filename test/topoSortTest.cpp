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
/** @file topoSortTest.cpp
 *  Testing PluginManager::determineExecutionOrder
 *  \date 15.09.2011
 *  \author <a href="mailto:jmgottfried@web.de>Jens-Malte Gottfried</a>
 */

#include <cstdlib>
#include <iostream>

#include "../include/charon-core/ParameteredObject.hxx"
#include "../include/charon-core/ExceptionHandler.h"
#include "../include/charon-core/PluginManager.h"

// make sure that assert() works (nothing is tested otherwise)
#undef NDEBUG
#include <cassert>

/// Main test application.
int test() {
	// output to standart out
	sout.assign(std::cout);

	// load dummy module
	PluginManager man(MODULE_DIR);
	man.loadPlugin("DummyModule");
	assert(man.getLoadedPluginsCount() == 1);
	assert(man.getInstancesCount() == 0);

	const size_t num = 3;
	std::list<ParameteredObject*> inst, tst;
	std::list<ParameteredObject*>::const_iterator iter;

	while (inst.size() < num) {
		std::ostringstream strm;
		strm << inst.size();
		inst.push_back(man.createInstance("DummyModule", strm.str()));
	}
	assert(man.getLoadedPluginsCount() == 1);
	assert(man.getInstancesCount() == num);

	man.connect("0.out", "1.in");
	man.connect("1.out", "2.in");
	sout << "Layout: 0-1-2" <<std::endl;

	tst = man.determineExecutionOrder();
	assert(inst == tst);

	man.disconnect("0.out", "1.in");
	man.disconnect("1.out", "2.in");
	man.connect("0.out", "2.in");
	man.connect("2.out", "1.in");
	sout << "Layout: 0-2-1" <<std::endl;
	tst = man.determineExecutionOrder();
	assert(inst != tst);
	std::ostringstream tstrm;
	for (iter=tst.begin(); iter!=tst.end(); iter++) {
		tstrm << (*iter)->getName();
	}
	sout << "Order: " << tstrm.str() << std::endl;
	assert(tstrm.str() == "021");

	man.disconnect("0.out", "2.in");
	man.disconnect("2.out", "1.in");
	man.connect("0.out", "1.in");
	man.connect("0.out", "2.in");
	sout << "Layout: 0-1\n         \\\n          2" <<std::endl;
	tst = man.determineExecutionOrder();
	assert(tst.size() == num);
	tstrm.str("");
	for (iter=tst.begin(); iter!=tst.end(); iter++) {
		tstrm << (*iter)->getName();
	}
	sout << "Order: " << tstrm.str() << std::endl;
	assert(tstrm.str() == "012" || tstrm.str() == "021");

	const size_t num2 = 6;
	while (inst.size() < num2) {
		std::ostringstream strm;
		strm << inst.size();
		inst.push_back(man.createInstance("DummyModule", strm.str()));
	}
	assert(man.getInstancesCount() == num2);

	man.connect("1.out", "3.in");
	man.connect("2.out", "3.in");
	man.connect("3.out", "4.in");
	man.connect("3.out", "5.in");
	sout << "Layout: 0-1-3-4\n         \\ / \\\n          2   5" << std::endl;
	tst = man.determineExecutionOrder();
	assert(tst.size() == num2);
	tstrm.str("");
	for (iter=tst.begin(); iter!=tst.end(); iter++) {
		tstrm << (*iter)->getName();
	}
	std::string part1 = tstrm.str().substr(0,4);
	std::string part2 = tstrm.str().substr(4, std::string::npos);
	sout << "Order: " << tstrm.str() << " (" << part1 << "; "
		<< part2 << ")" << std::endl;
	assert(part1 == "0123" || part1 == "0213");
	assert(part2 == "45" || part2 == "54");
	man.finalize();
	return EXIT_SUCCESS;
}

int main() {
	return ExceptionHandler::run(test);
}
