/*  Copyright (C) 2009 Jens-Malte Gottfried <jmgottfried@web.de>

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
/** \file iterator2_nested.cpp
 *  Unit tests for nested iterations
 *  \author Jens-Malte Gottfried <jmgottfried@web.de>
 *  \date 02.02.2010
 */

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>

// needed CPP definitions
#ifndef GLOBAL_PLUGIN_DIR
/// set this to the global plugin path
#define GLOBAL_PLUGIN_DIR ""
#error  GLOBAL_PLUGIN_DIR not defined!
#endif
#ifndef LOCAL_PLUGIN_DIR
/// set this to the plugin build path (local plugin path)
#define LOCAL_PLUGIN_DIR ""
#error  LOCAL_PLUGIN_DIR not defined!
#endif
#ifndef TESTDIR
/// local test source directory
#define TESTDIR ""
#error  TESTDIR not defined!
#endif

#include <charon-core/PluginManager.h>
#include <charon-core/ParameteredObject.h>
#include <charon-core/FileTool.h>
#include <charon-core/ExceptionHandler.h>

#include <charon/SimpleIterator.h>
#include <charon/IteratorHelper.h>

int test() {
	std::ofstream log("iteratorTest2_nested.log", std::ios::trunc);
	assert(log.good());
	sout.assign(log);

	// load plugin manager
	PluginManager man(GLOBAL_PLUGIN_DIR, LOCAL_PLUGIN_DIR);

	// start tests
	std::string testfile(TESTDIR "/iteratorTest2.wrp");
	std::cout << "Loading parameter file \"" << testfile;
	std::cout << "\"" << std::endl;
	man.loadParameterFile(testfile);
	std::cout << "Executing workflow..." << std::endl;
	sout << std::endl;
	man.executeWorkflow();
	std::cout << "Workflow execution finished.\n" << std::endl;

	// get test instances
	std::cout << "Try to get some instances from workflow:" << std::endl;
	SimpleIterator<double>* iterator =
			dynamic_cast<SimpleIterator<double>*>(
					man.getInstance("iterator"));
	assert(iterator);
	std::cout << "\titerator:  " << (void*) iterator << std::endl;
	SimpleIterator<double>* iterator2 =
			dynamic_cast<SimpleIterator<double>*>(
					man.getInstance("iterator2"));
	assert(iterator2);
	std::cout << "\titerator2: " << (void*) iterator2 << std::endl;
	IteratorHelper<double>* helper =
			dynamic_cast<IteratorHelper<double>*>(
					man.getInstance("helper"));
	assert(helper);
	std::cout << "\thelper:    " << (void*) helper << std::endl;
	IteratorHelper<double>* helper2 =
			dynamic_cast<IteratorHelper<double>*>(
					man.getInstance("helper2"));
	assert(helper);
	std::cout << "\thelper2:   " << (void*) helper2 << std::endl;

	std::cout << "\nTry to get some parameters:" << std::endl;
	std::cout << "\tmaxRuns1: " << iterator->maxRuns() << "\n";
	// check if there should be 3 inner iterations
	assert(iterator->maxRuns() == 3u);
	// check if there should be 2 outer iterations
	assert(iterator2->maxRuns() == 2u);

	// check if flow change criterion has been deactivated
	assert(iterator->epsilon() == 0.);
	assert(iterator2->epsilon() == 0.);

	// check if there were really 6 iterations
	assert(helper->countAll() == 3u*2u);
	std::cout << "Inner iteration: " << helper->countAll() << " loops.\n";
	std::cout << "Outer iteration: " << helper2->countAll()<< " loops.\n";

	man.reset();
	sout.assign();
	log.close();

	return EXIT_SUCCESS;
}

int main() {
	return ExceptionHandler::run(test);
}
