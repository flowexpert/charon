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
/** \file fit1_SampleGeneration.cpp
 *  Generate MixtureOfGaussian Sample using MCMC Sampler
 *  \author <a href="Jens-Malte.Gottfried@iwr.uni-heidelberg.de>
 *      Jens-Malte Gottfried </a>
 *  \date 19.11.2010
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

#include <charon-core/PluginManager.h>
#include <charon-core/ExceptionHandler.h>
#include <charon/MixtureOfGaussians.h>

/// unit tests
int test() {
	std::ofstream log("fit2_CdFitter.log", std::ios::trunc);
	assert(log.good());
	sout.assign(log);

	// load plugin manager
	PluginManager man(GLOBAL_PLUGIN_DIR, LOCAL_PLUGIN_DIR);

	// start tests
	std::string testfile(TESTDIR "/fit2.wrp");
	std::cout << "Loading parameter file \"" << testfile;
	std::cout << "\"" << std::endl;
	man.loadParameterFile(testfile);

	// get test instances
	MixtureOfGaussians* gsm = dynamic_cast<MixtureOfGaussians*>(
			man.getInstance("gsm"));
	assert(gsm);

	// print results to stdout
	std::cout << "Parameters on start:" << std::endl;
	gsm->execute();
	sout.assign(std::cout);
	gsm->printResults();
	sout.assign(log);

	man.executeWorkflow();

	std::cout << "Parameters after fitting:" << std::endl;
	sout.assign(std::cout);
	gsm->printResults();
	sout.assign(log);

	// since sampling is a random process, this may fail
	// but only in rare cases.
	assert(gsm->fitParameters[0]>0.2);
	assert(gsm->fitParameters[0]<0.3);
	assert(gsm->fitParameters[1]>0.7);
	assert(gsm->fitParameters[1]<0.8);

	man.reset();
	sout.assign();
	log.close();

	return EXIT_SUCCESS;
}

/// start tests with exception handling
int main() {
	return ExceptionHandler::run(test);
}
