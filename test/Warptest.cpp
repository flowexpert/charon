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
/** \file SimpleIteratorTest.cpp
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
#ifndef CMAKE_INTDIR
/// suffix to local plugin dir
/** in MSVC this is set to "Debug"
 *  on debug builds and to "Release"
 *  on release builds.
 */
#define CMAKE_INTDIR
#endif

#include <charon-core/PluginManager.h>
#include <charon-core/ParameteredObject.h>
#include <charon-core/FileTool.h>
#include <charon-core/ExceptionHandler.h>
#include <charon-utils/Crop.h>
#include<limits>

int test() {
	std::ofstream log("Warptest.log", std::ios::trunc);
	assert(log.good());
	sout.assign(log);

	// load plugin manager
	PluginManager man(GLOBAL_PLUGIN_DIR, LOCAL_PLUGIN_DIR "/" CMAKE_INTDIR);

	// start tests
	std::cout << "Loading parameter file \"" << WARPWRPFILE;
	std::cout << "\"" << std::endl;
	man.loadParameterFile(WARPWRPFILE);
	std::cout << "Executing workflow..." << std::endl;
	man.executeWorkflow();
	std::cout << "Workflow execution finished.\n" << std::endl;

	// get test instances
	Crop<double>* cropOriginal = dynamic_cast<Crop<double>*>(
			man.getInstance("crop"));
	Crop<double>* cropWarp = dynamic_cast<Crop<double>*>(
			man.getInstance("crop2"));
	assert(cropOriginal);
	assert(cropWarp);
	
	assert((cropOriginal->out()[0] - cropWarp->out()[0]).abs().max() <= std::numeric_limits<float>::min());

	man.reset();
	sout.assign();
	log.close();

	return EXIT_SUCCESS;
}

int main() {
	cimg_library::CImgList<float> penguinflow(2, 60, 60, 1, 1);
	penguinflow[0].fill(2.0f);
	penguinflow[1].fill(2.0f);
	penguinflow.save("penguinflow.cimg");
	return ExceptionHandler::run(test);
}
