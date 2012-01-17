/*  This file is part of Charon.

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
/** \file flow1_HS_Sinus.cpp
 *  Horn&Schunck algo on Sinus seq
 *  \see PetscSolver.h
 *  \author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  \date 8.09.2009
 */

#include <charon-core/PluginManager.h>
#include <charon-core/ExceptionHandler.h>
#include <charon-utils/CImg.h>

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
// make assert work for testing
#ifdef NDEBUG
#undef NDEBUG
#endif

/// unit tests
int test() {
	std::ofstream log("flow1_HS_Sinus.log", std::ios::trunc);
	assert(log.good());
	sout.assign(log);

	PluginManager man(GLOBAL_PLUGIN_DIR, LOCAL_PLUGIN_DIR);
	man.loadParameterFile(TESTDIR "/flow1.wrp");

	man.runWorkflow();
	assert(FileTool::exists("flow1_HS_Sinus.cimg"));
	assert(FileTool::exists("flow1_HS_Sinus_calc.cimg"));

	// load original and calculated flow
	cimg_library::CImgList<double> flowOrig("flow1_HS_Sinus.cimg");
	cimg_library::CImgList<double> flowCalc("flow1_HS_Sinus_calc.cimg");

	assert(flowOrig.size() == 2);
	assert(flowOrig.is_sameNXYZC(flowCalc));

	// check results
	double meanError = sqrt(((flowOrig[0]-flowCalc[0]).sqr()
		+(flowOrig[1]-flowCalc[1]).sqr()).mean());
	std::cout << "mean of difference norm: " << meanError << std::endl;
	assert(meanError < 0.1);

	man.reset();

	sout.assign(std::cout);
	log.close();

	return 0;
}

/// start tests with exception handling
int main() {
	return ExceptionHandler::run(test);
}
