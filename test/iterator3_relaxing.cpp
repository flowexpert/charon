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
/** \file iterator3_relaxing.cpp
 *  Unit tests for relaxing iterations
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

#include <iomanip>

#include <charon-core/PluginManager.h>
#include <charon-core/ParameteredObject.h>
#include <charon-core/FileTool.h>
#include <charon-core/ExceptionHandler.h>

#include <charon/Stencil.h>
#include <charon/IteratorHelper.h>
#include <charon/RelaxingIterator.h>

/// unit tests
int test() {
	std::ofstream log("iteratorTest3_relaxing.log", std::ios::trunc);
	assert(log.good());
	sout.assign(log);

	// load plugin manager
	PluginManager man(GLOBAL_PLUGIN_DIR, LOCAL_PLUGIN_DIR);

	// start tests
	std::string testfile(TESTDIR "/iterator3.wrp");
	std::cout << "Loading parameter file \"" << testfile;
	std::cout << "\"" << std::endl;
	man.loadParameterFile(testfile);

	// get test instances
	std::cout << "Try to get some instances from workflow:" << std::endl;
	RelaxingIterator<double>* iterator =
			dynamic_cast<RelaxingIterator<double>*>(
					man.getInstance("iterator"));
	assert(iterator);
	const double wi = iterator->initialWeight();
	const double wf = iterator->finalWeight();
	const double runs = iterator->maxRuns();
	std::cout << "\titerator:       " << (void*) iterator
			<< " (relaxing from " << wi << " to " << wf << " in "
			<< runs << " iterations)" << std::endl;

	IteratorHelper<double>* helper =
			dynamic_cast<IteratorHelper<double>*>(
					man.getInstance("helper"));
	assert(helper);
	std::cout << "\thelper:         " << (void*) helper << std::endl;

	Stencil::Base<double>* stencil1 = dynamic_cast<Stencil::Base<double>*>(
			man.getInstance("stencil1"));
	assert(stencil1);
	const double li1 = stencil1->lambda();
	std::cout << "\tstencil1:       " << (void*) stencil1
			<< " (lambda = " << li1 << ")" << std::endl;

	Stencil::Base<double>* stencil2 = dynamic_cast<Stencil::Base<double>*>(
			man.getInstance("stencil2"));
	assert(stencil2);
	const double li2 = stencil2->lambda();
	std::cout << "\tstencil2:       " << (void*) stencil2
			<< " (lambda = " << li2 << ")" << std::endl;

	FileTool::changeDir(TESTDIR);

	std::cout << "Starting iterations" << std::endl;
	iterator->initialize();
	bool error = false;
	double count, l, c, l1, l2, e1, e2;
	bool cont;
	do {
		cont = iterator->singleStep();
		count = helper->count();

		// calculate expected values
		l  = (runs > 1.) ? ((count-1) / (runs-1.)) : 0.;
		c  = wf * l + wi * (1.-l);
		e1 = li1 * (1.-c);
		e2 = li2 * c;

		l1 = stencil1->lambda();
		l2 = stencil2->lambda();
		std::cout << "\titeration " << count << ":    expected "
				<< std::setw(5) << e1 << " "
				<< std::setw(5) << e2;
		std::cout << "    got "
				<< std::setw(5) << l1 << " "
				<< std::setw(5) << l2;

		if(e1 != l1 || e2 != l2) {
			std::cout << "   ***Error";
			error = true;
		}
		std::cout << std::endl;
	} while (cont);
	iterator->finalize();

	man.reset();
	sout.assign();
	log.close();

	return error ? EXIT_FAILURE : EXIT_SUCCESS;
}

/// start tests with exception handling
int main() {
	return ExceptionHandler::run(test);
}
