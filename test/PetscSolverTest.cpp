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
/** @file PetscSolverTest.cpp
 *  Test programm for PetscSolver, which implements Horn-Schunk.
 *  @see PetscSolver.h
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */

#include <charon-core/PluginManager.h>
#include <charon-utils/CImg.h>

// needed CPP definitions
#ifndef GLOBAL_PLUGIN_DIR
/// set this to the global plugin path
#define GLOBAL_PLUGIN_DIR ""
#warning GLOBAL_PLUGIN_DIR not defined!
#endif
#ifndef LOCAL_PLUGIN_DIR
/// set this to the plugin build path (local plugin path)
#define LOCAL_PLUGIN_DIR ""
#warning LOCAL_PLUGIN_DIR not defined!
#endif
#ifndef CMAKE_INTDIR
/// suffix to local plugin dir
/**	in MSVC this is set to "Debug"
 *	on debug builds and to "Release"
 *	on release builds.
 */
#define CMAKE_INTDIR
#endif

int main() {
	std::ofstream log("bcceTestLog.txt", std::ios::trunc);
	assert(log.good());
	sout.assign(log);


	PluginManager man(GLOBAL_PLUGIN_DIR, LOCAL_PLUGIN_DIR "/" CMAKE_INTDIR);
	man.loadParameterFile(BCCE_TESTFILE);
	try {
		man.executeWorkflow();
		assert(FileTool::exists("bcceTest_flow.cimg"));
		assert(FileTool::exists("bcceTest_flow_calc.cimg"));

		// load original and calculated flow
		cimg_library::CImgList<double> flowOrig("bcceTest_flow.cimg");
		cimg_library::CImgList<double> flowCalc("bcceTest_flow_calc.cimg");

		assert(flowOrig.size() == 2);
		assert(flowOrig.is_sameNXYZC(flowCalc));

		// check results
		double meanError = sqrt(((flowOrig[0]-flowCalc[0]).sqr()
			+(flowOrig[1]-flowCalc[1]).sqr()).mean());
		std::cout << "mean of difference norm: " << meanError << std::endl;
		assert(meanError < 0.1);

		man.reset();
	}
	catch (const std::exception& e) {
		std::cerr << "Caught exception of type " << typeid(e).name() << ".\n";
		std::cerr << "Message:\n" << e.what() << std::endl;
	}
	catch (const std::string& e) {
		std::cerr << "Caught exception of type std::string.\n";
		std::cerr << "Message:\n" << e << std::endl;
	}
	catch (const char*& e) {
		std::cerr << "Caught exception of type const char*.\n";
		std::cerr << "Message:\n" << e << std::endl;
	}
	catch (...) {
		std::cerr << "Caught unknown exception." << std::endl;
	}

	sout.assign(std::cout);
	log.close();

	return 0;
}
