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

int main() {
	// test creation of all necessary objects
	std::ofstream log("PetscDLLTest.txt", std::ios::trunc);
	assert(log.good());
	sout.assign(log);

	PluginManager man(GLOBAL_PLUGIN_DIR, LOCAL_PLUGIN_DIR);
	man.loadPlugin("PetscSolver");
	ParameteredObject* solver = man.createInstance("PetscSolver", "solver");
	assert(! solver->connected());
	man.destroyInstance(solver);
	return 0;
}
