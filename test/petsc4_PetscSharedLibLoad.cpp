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
/** @file petsc4_PetscSharedLibLoad.cpp
 *  Load generated library with dlopen and execute the run function.
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date 25.10.2010
 */

#include <dlfcn.h>
#include <cassert>
#include <iostream>

/// test run
/** Check for errors on loading and unloading libmpi.so.0 and
 *  the self-compiled libpetsc4_lib.so.
 *  There are some errors on ubuntu with built-in petsc implementation,
 *  this test checks for this problem.
 *  This Unix-specific, so not available on Win platforms.
 *  \param argc,argv Command line options passed to MPI and PETSc
 */
int main(int argc, char** argv) {
	std::cout << "opening libmpi.so.0" << std::endl;
	void* handle = dlopen("libmpi.so.0", RTLD_LAZY | RTLD_GLOBAL);
	std::cout << "calling MPI_Init" << std::endl;
	void* func = dlsym(handle, "MPI_Init");
	assert(func);
	int (&MPI_Init)(int*, char***) = *((int (*)(int*, char***)) func);
	MPI_Init(&argc,&argv);
	std::cout << "calling MPI_Finalize" << std::endl;
	func = dlsym(handle, "MPI_Finalize");
	assert(func);
	int (&MPI_Finalize)(void) = *((int (*)(void))func);
	MPI_Finalize();
	std::cout << "closing libmpi.so.0" << std::endl;
	dlclose(handle);

	std::string fName = "libpetsc4_lib.so";
	std::cout << "opening PetscTestDLL (" << fName << ")" << std::endl;
	handle = dlopen(
			fName.c_str(),
			RTLD_LAZY | RTLD_GLOBAL);
	func = dlsym(handle, "run");
	assert(func);
	int (&run)(int&, char**&) = *((int (*)(int&, char**&)) func);
	run(argc, argv);
	std::cout << "closing PetscTestDLL" << std::endl;

	// some error occurs here (using ubuntu lucid)
	// fixed in petsc 3.1 used in maverick -> upstream
	dlclose(handle);

	return 0;
}
