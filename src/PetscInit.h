/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/** @file PetscInit.h
 *  Declaration initialization class PetscInit.
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date 04.11.2009
 */

#ifndef PETSC_INIT_H_
#define PETSC_INIT_H_

#if defined(HANDLE_DLL)
#ifdef petscinit_EXPORTS
///Visual C++ specific code
#define petscinit_DECLDIR __declspec(dllexport)
#else
#define petscinit_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define petscinit_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>

/// Handle Petsc initialization
class petscinit_DECLDIR PetscInit : public ParameteredObject {
public:
	/// Petsc command line options
	Parameter<std::string> commandLine;

	/// Self pointer
	OutputSlot<PetscInit*> self;

	/// default constructor
	PetscInit(const std::string& name="" /**[in] instance name*/);

	/// default destructor
	virtual ~PetscInit();

	/// initialize petsc
	virtual void execute();

	/// get command line argument counter
	int argc() const;

	/// get command line argument vector
	const char* const* argv() const;

private:
	/// guard to avoid duplicate initialization
	static bool _initialized;
	/// command line argument counter
	int _argc;
	/// command line argument vector
	char** _argv;
};

#endif // PETSC_INIT_H_
