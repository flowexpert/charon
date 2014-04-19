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
/** \file fit3_GSM.cpp
 *  \author <a href="Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried </a>
 *  \date 19.11.2010
 *  \test Check MixtureOfGaussian properties
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
	sout.assign(std::cout);

	// load plugin manager
	PluginManager man(GLOBAL_PLUGIN_DIR, LOCAL_PLUGIN_DIR);

	// start tests
	std::string testfile(TESTDIR "/fit1.wrp");
	std::cout << "Loading parameter file \"" << testfile;
	std::cout << "\"" << std::endl;
	man.loadParameterFile(testfile);

	// get test instances
	MixtureOfGaussians* gsm = dynamic_cast<MixtureOfGaussians*>(
			man.getInstance("gsm"));
	assert(gsm);

	// print results to stdout
	gsm->run();
	gsm->printParameters();

	std::ofstream vals;
	vals.exceptions(std::ios::failbit|std::ios::badbit|std::ios::eofbit);
	vals.open("fit3_GSM_values.dat",std::ios::trunc);

	vals << "# gsm values" << std::endl;

	const double r = gsm->rangeHint();
	const double stepsize = r/200.;
	const Diff2Function& p = *dynamic_cast<Diff2Function*>(gsm->probability());
	const Diff2Function& e = *dynamic_cast<Diff2Function*>(gsm->energy());
	const Diff2LinFunction& el =
			*dynamic_cast<Diff2LinFunction*>(gsm->energy());
	std::vector<double> c(1,0.);
	const double off = e(c);
	double elMin = el.diff2Linearized(c);
	for(c[0] = -r; c[0] <= r; c[0] += stepsize) {
		const double elCur=el.diff2Linearized(c);
		if(elCur < elMin)
			elMin = elCur;
		vals << c[0]                    // 1
				<< "\t" << p(c)         // 2
				<< "\t" << p.diff(c)    // 3
				<< "\t" << p.diff2(c)   // 4
				<< "\t" << e(c)-off     // 5
				<< "\t" << e.diff(c)    // 6
				<< "\t" << e.diff2(c)   // 7
				<< "\t" << elCur        // 8
				<< std::endl;
	}

	// check for bug causing negative values of diff2Lin
	if(elMin < 0)
		throw std::runtime_error("negative values of linearized gsm energy!");

	return EXIT_SUCCESS;
}

/// start tests with exception handling
int main() {
	return ExceptionHandler::run(test);
}
