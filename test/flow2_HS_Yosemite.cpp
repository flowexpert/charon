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
/** \file flow2_HS_Yosemite.cpp
 *  Horn&Schunck algo on Yosemite seq
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
#error GLOBAL_PLUGIN_DIR not defined!
#endif
#ifndef LOCAL_PLUGIN_DIR
/// set this to the plugin build path (local plugin path)
#define LOCAL_PLUGIN_DIR ""
#error LOCAL_PLUGIN_DIR not defined!
#endif
#ifndef TESTDIR
/// global testing directory
#define TESTDIR ""
#error TESTDIR not defined!
#endif

#include <charon-core/PluginManager.h>
#include <charon-core/ParameteredObject.h>
#include <charon-core/FileTool.h>
#include <charon-core/ExceptionHandler.h>
#include <charon/FlowComparator.h>

int test() {
	std::ofstream log("flow2_HS_Yosemite.log", std::ios::trunc);
	assert(log.good());
	sout.assign(log);

	// load plugin manager
	PluginManager man(GLOBAL_PLUGIN_DIR, LOCAL_PLUGIN_DIR);

	// start tests
	std::cout << "Loading parameter file \"" << TESTDIR "/flow2.wrp";
	std::cout << "\"" << std::endl;
	FileTool::changeDir(TESTDIR);
	man.loadParameterFile(TESTDIR "/flow2.wrp");
	std::cout << "Executing workflow..." << std::endl;
	man.executeWorkflow();
	std::cout << "Workflow execution finished.\n" << std::endl;


	// get test instances
	FlowComparator<double>* comparator = dynamic_cast<FlowComparator<double>*>(
			man.getInstance("analysis"));
	assert(comparator);
	std::cout << "mean endpoint error: "
			<< comparator->getMeanEndpointError() << std::endl;
	assert(comparator->getMeanEndpointError() <= 0.12);

	man.reset();
	sout.assign();
	log.close();

	return EXIT_SUCCESS;
}

int main() {
	return ExceptionHandler::run(test);
}
