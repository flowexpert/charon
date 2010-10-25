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
/** @file PetscTestDLL.cpp
 *  Library to perform simple calls to petsc.
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date 25.10.2010
 */

#include <petscksp.h>
#include <iostream>

/// do something arbitrary with petsc
extern "C" {
	int run(int& argc, char**& argv) {
		// testing Petsc Initialization
		std::cout << "entering PetscTestDLL" << std::endl;
		PetscErrorCode ierr = MPI_Init(&argc,&argv);
		CHKERRQ(ierr);
		ierr = PetscInitialize(&argc,&argv,PETSC_NULL,PETSC_NULL);
		CHKERRQ(ierr);
		Vec	x;
		ierr = VecCreate(PETSC_COMM_WORLD,&x);
		CHKERRQ(ierr);
		ierr = PetscFinalize();
		CHKERRQ(ierr);
		int initialized = 0;
		MPI_Initialized(&initialized);
		if (initialized) {
			ierr = MPI_Finalize();
			CHKERRQ(ierr);
		}
		std::cout << "leaving PetscTestDLL" << std::endl;
		return 0;
	}
}
