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

#include <Solvers/PetscSolver.h>
#include <charon-utils/Roi.h>
#include <Stencils/Gbcce.h>
#include <Stencils/L2Norm.h>
#include <BrightnessModels/Constant.h>
#include <MotionModels/LocalConstant.h>
#include <charon-utils/LinearFilter.h>
#include <charon-utils/Mask1D.h>
#include <charon-utils/ChannelConverter.h>
#include <charon-utils/FileReader.h>
#include <charon-utils/FileWriter.h>
#include <charon-core/PluginManager.h>

#ifndef CMAKE_INTDIR
/// suffix to local plugin dir
/**	in MSVC this is set to "Debug"
 *	on debug builds and to "Release"
 *	on release builds.
 */
#define CMAKE_INTDIR
#endif

int main() {
	// test creation of all necessary objects
	delete new FileReader<float>("reader");
	delete new ChannelConverter<float>("converter");
	delete new LinearFilter<float>("derivative");
	delete new Mask1D<float>("diff");
	delete new BrightnessModels::Constant<float>("brightnessConst");
	delete new MotionModels::LocalConstant<float>("motionConstant");
	delete new Gbcce<float>("GBCCE");
	delete new L2Norm<float>("L2Norm");
	delete new PetscSolver<float>("Solver");
	delete new FileWriter<float>("writer");

	std::ofstream log("bcceTestLog.txt", std::ios::trunc);
	assert(log.good());
	sout.assign(std::cout, log);

	PluginManager man(GLOBAL_PLUGIN_DIR, LOCAL_PLUGIN_DIR "/" CMAKE_INTDIR);
	man.loadParameterFile(BCCE_TESTFILE);
	try {
		man.executeWorkflow();
	}
	catch (std::exception e) {
		sout << "caught exception of type " << typeid(e).name() << std::endl;
		sout << "Message:" << std::endl;
		sout << e.what() << std::endl;
	}
	catch (cimg_library::CImgException e) {
		sout << "caught exception of type " << typeid(e).name() << std::endl;
		sout << "Message:" << std::endl;
		sout << e.message << std::endl;
	}
	catch (std::string e) {
		sout << "caught exception!" << std::endl;
		sout << "Message:" << std::endl;
		sout << e << std::endl;
	}
	catch (...) {
		sout << "caught unknown exception" << std::endl;
	}

	sout.assign();
	log.close();
	
	return 0;
}
