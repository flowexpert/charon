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
/** \file petsc4_PetscSharedLibLoad.cpp
 *  Load generated library with dlopen and execute the run function.
 *  \author <a href="Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried </a>
 *  \date 25.10.2010
 *
 *  \test
 *  Check for some possible problems on ubuntu with built-in
 *  petsc implementation (hopefully not longer present with current versions):
 *  - loading and unloading libmpi.so.0 and using it's MPI_Init and MPI_Finalize
 *  - loading/unloading and using a self-compiled libpetsc4_lib.so which
 *    uses simple petsc calls, similar to petsc1_Init.cpp.
 *  \note
 *  This Unix-specific, so not available on Win platforms.
 */

#include <dlfcn.h>
#include <cassert>
#include <iostream>
#include <cstdlib>

/// test run
/** \param argc,argv Command line options passed to MPI and PETSc
 *  \returns zero in case of success
 */
int main(int argc, char** argv) {
	std::cout << "opening libmpi.so" << std::endl;
	void* handle = dlopen("libmpi.so", RTLD_LAZY | RTLD_GLOBAL);
	if (!handle) {
		std::cerr << dlerror() << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "calling MPI_Init" << std::endl;
	void* func = dlsym(handle, "MPI_Init");
	if (!func) {
		std::cerr << dlerror() << std::endl;
		return EXIT_FAILURE;
	}
	int (*MPI_Init)(int*, char***);
	*reinterpret_cast<void**>(&MPI_Init) = func;
	MPI_Init(&argc,&argv);
	std::cout << "calling MPI_Finalize" << std::endl;
	func = dlsym(handle, "MPI_Finalize");
	if (!func) {
		std::cerr << dlerror() << std::endl;
		return EXIT_FAILURE;
	}
	int (*MPI_Finalize)(void);
	*reinterpret_cast<void**>(&MPI_Finalize) = func;
	MPI_Finalize();
	std::cout << "closing libmpi.so.0" << std::endl;
	dlclose(handle);

#ifdef NDEBUG
	std::string fName = "./libpetsc4_lib.so";
#else
	std::string fName = "./libpetsc4_lib_d.so";
#endif
	std::cout << "opening PetscTestDLL (" << fName << ")" << std::endl;
	handle = dlopen(
			fName.c_str(),
			RTLD_LAZY | RTLD_GLOBAL);
	if (!handle) {
		std::cerr << dlerror() << std::endl;
		return EXIT_FAILURE;
	}
	func = dlsym(handle, "run");
	if (!func) {
		std::cerr << dlerror() << std::endl;
		return EXIT_FAILURE;
	}
	int (*run)(int&, char**&);
	*reinterpret_cast<void**>(&run) = func;
	run(argc, argv);
	std::cout << "closing PetscTestDLL" << std::endl;

	dlclose(handle);

	return EXIT_SUCCESS;
}
