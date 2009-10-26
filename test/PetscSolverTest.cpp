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

int main() {
	//create all necessary objects
	FileReader<float> reader("reader");
	ChannelConverter<float> converter("converter");
	LinearFilter<float> derivative("derivative");
	Mask1D<float> diff("diff");
	BrightnessModels::Constant<float> brightnessConst("brightnessConst");
	MotionModels::LocalConstant<float> motionConstant("motionConstant");
	Gbcce<float> gbcce("GBCCE");
	L2Norm<float> l2norm("L2Norm");
	PetscSolver<float> solver("Solver");
	FileWriter<float> writer("writer");
	
	return 0;
}
