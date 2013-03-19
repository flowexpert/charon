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
/** \file petsc2_Plugins.cpp
 *  Check flow plugins.
 *  \see PetscSolver.h
 *  \author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  \date 8.09.2009
 *
 *  \test
 *  Try loading and unloading of some flow plugins.
 *  Some of them use PETSc (PetscSolver).
 *  This is not done via the PluginManager but via direct linking to
 *  the shared libs.
 */

#include <charon/Solvers/PetscSolver.h>
#include <charon/Stencils/Gbcce.h>
#include <charon/Stencils/L2Norm.h>
#include <charon/BrightnessModels/Constant.h>
#include <charon/MotionModels/LocalConstant.h>

/// try to load some plugins and check for errors
int main() {
	// test creation of all necessary objects
	delete new BrightnessModels::Constant<float>("brightnessConst");
	delete new MotionModels::LocalConstant<float>("motionConstant");
	delete new Gbcce<float>("bcce");
	delete new L2Norm<float>("l2Norm");
	delete new PetscSolver<float>("solver");

	return 0;
}
