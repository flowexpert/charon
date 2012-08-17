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
/** \file iterator1_simple.cpp
 *  Unit tests for class SimpleIterator
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

/// unit tests
int test() {
	std::ofstream log("iteratorTest1_simple.log", std::ios::trunc);
	assert(log.good());
	sout.assign(log);

	// load plugin manager
	PluginManager man(GLOBAL_PLUGIN_DIR, LOCAL_PLUGIN_DIR);

	// start tests
	std::string testfile(TESTDIR "/iterator1.wrp");
	std::cout << "Loading parameter file \"" << testfile;
	std::cout << "\"" << std::endl;
	man.loadParameterFile(testfile);

	// get test instances
	SimpleIterator<double>* iterator =
			dynamic_cast<SimpleIterator<double>*>(
					man.getInstance("iterator"));
	assert(iterator);
	std::cout << "Try to get some instances from workflow:" << std::endl;
	std::cout << "\titerator: " << (void*) iterator << std::endl;

	IteratorHelper<double>* helper =
			dynamic_cast<IteratorHelper<double>*>(
					man.getInstance("helper"));
	assert(helper);
	std::cout << "\thelper:   " << (void*) helper << std::endl;

	// check if data from sequence generator really reach the iteration
	// helper output slots after helper->run()
	// this detects a bug fixed in IteratorHelper (for regression testing)
	helper->run();
	const cimg_library::CImgList<double>& helperSeq =
			helper->sequence();
	assert(helperSeq.size() > 0);
	std::cout << "Sequence output: " << helperSeq.size() << " element(s) "
			<< "-- max=" << helperSeq.max() << ", min=" << helperSeq.min()
			<< std::endl;

	std::cout << "\nTry to get some parameters:" << std::endl;
	std::cout << "\tmaxRuns:  " << iterator->maxRuns() << "\n";
	std::cout << "\tcount:    " << helper->count() << std::endl;
	// check if there should be 3 iterations
	assert(iterator->maxRuns() == 3u);

	// execute whole workflow
	std::cout << "Executing workflow..." << std::endl;
	sout << std::endl;
	man.runWorkflow();
	std::cout << "Workflow execution finished.\n" << std::endl;

	// check if helper has been updated 3 times
	assert(helper->count() == 3u);
	// check if there were really 3 iterations
	assert(helper->countAll() == 3u);
	std::cout << "There were exactly 3 iterations." << std::endl;

	// rerun with flow change criterion
	std::cout << "\nnow rerunning to test flow change criterion";
	std::cout << std::endl;
	iterator->maxRuns() = 0u;
	iterator->epsilon() = 1.;
	iterator->resetExecuted();

	sout << std::endl;
	man.runWorkflow();
	sout << std::endl;

	// check if there was only 1 iteration (+3 iterations from above)
	assert(helper->count() == 1u); // local counter
	assert(helper->countAll() == 1u+3u); // global counter
	// check for flow change
	std::cout << "There was exaclty one iteration.";
	std::cout << std::endl;

	man.reset();
	sout.assign();
	log.close();

	return EXIT_SUCCESS;
}

/// start tests with exception handling
int main() {
	return ExceptionHandler::run(test);
}
