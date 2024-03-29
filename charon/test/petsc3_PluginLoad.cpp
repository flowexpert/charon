/*  This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file petsc3_PluginLoad.cpp
 *  \author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *  \date 8.09.2009
 *
 *  \test
 *  check PetscSolver shared lib (module) using the PluginManager
 */

#include <charon-core/PluginManager.h>

// needed CPP definitions
#ifndef GLOBAL_PLUGIN_DIR
/// set this to the global plugin path
#define GLOBAL_PLUGIN_DIR ""
#error GLOBAL_PLUGIN_DIR not defined!
#endif
#ifndef LOCAL_PLUGIN_DIR
/// set this to the plugin build path (local plugin path)
#define LOCAL_PLUGIN_DIR ""
#error LOCAL_PLUGIN_DIR not defined!
#endif
#ifndef TESTDIR
/// global testing directory
#define TESTDIR ""
#error TESTDIR not defined!
#endif
// make assert work for testing
#ifdef NDEBUG
#undef NDEBUG
#endif

/// unit tests
/** Try to load the PetscSolver plugin using the plugin manager.
 *  \returns zero in case of success
 */
int main() {
	// test creation of all necessary objects
	sout.assign(std::cout);

	PluginManager man(GLOBAL_PLUGIN_DIR, LOCAL_PLUGIN_DIR);
	man.loadPlugin("PetscSolver");
	ParameteredObject* solver = man.createInstance("PetscSolver", "solver");
	assert(! solver->connected());
	man.destroyInstance(solver);
	return 0;
}
