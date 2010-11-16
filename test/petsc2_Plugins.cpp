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
/** @file petsc2_Plugins.cpp
 *  Check flow plugins.
 *  @see PetscSolver.h
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */

#include <charon/Solvers/PetscSolver.h>
#include <charon-utils/Roi.h>
#include <charon-utils/LinearFilter.h>
#include <charon-utils/Mask1D.h>
#include <charon-utils/ChannelConverter.h>
#include <charon-utils/FileReader.h>
#include <charon-utils/FileWriter.h>
#include <charon/Stencils/Gbcce.h>
#include <charon/Stencils/L2Norm.h>
#include <charon/BrightnessModels/Constant.h>
#include <charon/MotionModels/LocalConstant.h>

/// try to load some plugins and check for errors
int main() {
	// test creation of all necessary objects
	delete new FileReader<float>("reader");
	delete new ChannelConverter<float>("converter");
	delete new LinearFilter<float>("derivative");
	delete new Mask1D<float>("diff");
	delete new BrightnessModels::Constant<float>("brightnessConst");
	delete new MotionModels::LocalConstant<float>("motionConstant");
	delete new Gbcce<float>("bcce");
	delete new L2Norm<float>("l2Norm");
	delete new PetscSolver<float>("solver");
	delete new FileWriter<float>("writer");

	return 0;
}
