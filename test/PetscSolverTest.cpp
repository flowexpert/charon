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

#include <PetscSolver.hxx>
#include <Roi.hxx>
#include <Gbcce.hxx>
#include <L2Norm.hxx>
#include <Constant.hxx>
#include <LocalConstant.hxx>
#include <bin2.hxx>
#include <ChannelConverter.hxx>
#include <FileReader.hxx>
#include <FlieWriter.hxx>

int main() {
	//create all necessary objects
	FileReader<float> reader("reader");
	ChannelConverter<float> converter("converter");
	bin2<float> derivative("derivative");
	Constant<float> brightnessConst("brightnessConst");
	LocalConstant<float> motionConstant("motionConstant");
	Gbcce<float> gbcce("GBCCE");
	L2Norm<float> l2norm("L2Norm");
	PetscSolver<float> solver("Solver");
	FileWriter<float> writer("writer");
	
	return 0;
}
