/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

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
/** \file cgflow_HS.cpp
 *  Iterated Horn&Schunck algo
 *  \author Michael Baron <michael.baron@iwr.uni-heidelberg.de>
 *  \date 01.02.2012
 */

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>

// needed CPP definitions
#ifndef GLOBAL_PLUGIN_DIR
/// set this to the global plugin path
#define GLOBAL_PLUGIN_DIR ""
#error GLOBAL_PLUGIN_DIR not defined!
#endif
#ifndef LOCAL_PLUGIN_DIR
/// set this to the plugin build path (local plugin path)
#define LOCAL_PLUGIN_DIR ""
#error LOCAL_PLUGIN_DIR not defined!
#endif
#ifndef TESTDIR
/// local test source directory
#define TESTDIR ""
#error  TESTDIR not defined!
#endif

#include <iomanip>
#include <charon-core/PluginManager.h>
#include <charon-core/ParameteredObject.h>
#include <charon-core/FileTool.h>
#include <charon-core/ExceptionHandler.h>
#include <charon/FlowComparator.h>
#include <charon/IteratorHelper.h>
#include <charon/SimpleIterator.h>

/// unit tests
int test() {
	std::ofstream log("cgflowHS.log", std::ios::trunc);
	assert(log.good());
	sout.assign(log);
	std::ofstream epeLog("cgflowHS.csv", std::ios::trunc);
	assert(epeLog.good());

	// load plugin manager
	PluginManager man(GLOBAL_PLUGIN_DIR, LOCAL_PLUGIN_DIR);

	// start tests
	FileTool::changeDir(TESTDIR);
	std::string testfile("cgflow_HS.wrp");
	std::cout << "Loading parameter file \"" << testfile;
	std::cout << "\"" << std::endl;
	man.loadParameterFile(testfile);

	// get test instances
	FlowComparator<double>* comparator =
	  dynamic_cast<FlowComparator<double>*>(man.getInstance("analysis"));
	assert(comparator);

	std::cout << "Executing workflow..." << std::endl;
	man.runWorkflow();
	std::cout << "Workflow execution finished.\n" << std::endl;

	std::cout << "mean endpoint error: "
			<< comparator->getMeanEndpointError() << std::endl;
	if (comparator->getMeanEndpointError() > 0.05) {
		throw "EndpointError too high!";
	}
	epeLog.close();

	sout.assign();
	log.close();

	return EXIT_SUCCESS;
}

/// start tests with exception handling
int main() {
	return ExceptionHandler::run(test);
}

