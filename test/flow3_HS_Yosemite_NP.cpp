/*  Copyright (C) 2010 Jens-Malte Gottfried <jmgottfried@web.de>

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
/** \file flow3_HS_Yosemite_NP.cpp
 *  Horn&Schunck algo on Yosemite seq without shared libs.
 *  Test file performing the same as HornSchunckYosemite.cpp
 *  but without using the plugin mechanism. All needed modules are
 *  compiled-in using their .hxx implementations.
 *  This should simplify profiling to speed up the PetscSolver.
 *  \author Jens-Malte Gottfried <jmgottfried@web.de>
 *  \date 08.10.2010
 */

#ifndef TESTDIR
/// global testing directory
#define TESTDIR ""
#error TESTDIR not defined!
#endif

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>

#ifdef QT_GUI_LIB
#include <qdatastream.h>  // avoid macro crap with STATUS redefinition
#endif
#include <charon/Solvers/PetscSolver.hxx> // must be before CImg (macros!)
#include <charon-utils/FileReader.hxx>
#include <charon-utils/Roi.hxx>
#include <charon-utils/Crop.hxx>
#include <charon-utils/Convolution.hxx>
#include <charon/BrightnessModels/Constant.hxx>
#include <charon/MotionModels/LocalConstant.hxx>
#include <charon/Diff2D.hxx>
#include <charon/Stencils/Gbcce.hxx>
#include <charon/Stencils/L2Norm.hxx>
#include <charon/FlowComparator.hxx>

/// unit tests
int test() {
	sout.assign(std::cout);
	FileTool::changeDir(TESTDIR);
	ParameterFile testfileRW("flow2.wrp");
	const ParameterFile& tData = testfileRW;

	Roi<int> roi("roi");
	roi.loadParameters(tData);
	FileReader<double> flowreader("flowreader");
	flowreader.loadParameters(tData);
	FileReader<double> seqreader("seqreader");
	seqreader.loadParameters(tData);
	Crop<double> flowcrop("flowcrop");
	flowcrop.loadParameters(tData);
	Crop<double> seqcrop("seqcrop");
	seqcrop.loadParameters(tData);
	flowreader.out.connect(flowcrop.in);
	seqreader.out.connect(seqcrop.in);
	roi.out.connect(flowcrop.roi);
	roi.out.connect(seqcrop.roi);

	BrightnessModels::Constant<double> bm("bm");
	bm.loadParameters(tData);
	MotionModels::LocalConstant<double> mm("mm");
	mm.loadParameters(tData);
	Gbcce<double> bcce("bcce");
	bcce.loadParameters(tData);
	Diff2D<double> diff("diff");
	diff.loadParameters(tData);
	seqcrop.out.connect(bm.img);
	seqcrop.out.connect(diff.img);
	diff.dx.connect(mm.dx);
	diff.dy.connect(mm.dy);
	diff.dt.connect(mm.dt);
	bm.out.connect(bcce.brightnessIn);
	mm.out.connect(bcce.motionIn);

	Roi<int> solverroi("solverroi");
	solverroi.loadParameters(tData);
	L2Norm<double> l2norm("l2norm");
	l2norm.loadParameters(tData);
	PetscSolver<double> solver("solver");
	solver.loadParameters(tData);
	FlowComparator<double> analysis("analysis");
	analysis.loadParameters(tData);
	l2norm.out.connect(solver.stencils);
	bcce.out.connect(solver.stencils);
	solverroi.out.connect(solver.roi);
	solver.out.connect(analysis.result);
	flowcrop.out.connect(analysis.groundtruth);

	analysis.run();
	double err = analysis.getMeanEndpointError();
	std::cout << "mean endpoint error: " << err << std::endl;
	assert(err <= 0.12);

	return EXIT_SUCCESS;
}

/// start tests with exception handling
int main() {
	try{
		return test();
	}
	catch (const std::exception& err) {
		std::cout << err.what() << std::endl;
	}
	return EXIT_FAILURE;
}
