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
/** @file PetscInit.cpp
 *  Implementation of initialization class PetscInit.
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date 04.11.2009
 */

#include "PetscInit.h"
#include <charon-core/ParameteredObject.hxx>
#include <petsc.h>
#include <stdexcept>
#include <cstring>

bool PetscInit::_initialized = false;

PetscInit::PetscInit(const std::string& name) :
	ParameteredObject("PetscInit", name, "initialization of petsc library"),
	_argc(0), _argv(0)
{
	_addParameter(commandLine, "commandLine", "petsc command line");
	_addOutputSlot(self, "self", "self pointer", "PetscInit");
	self = this;
}

PetscInit::~PetscInit() {
	if(_initialized) {
		_initialized = false;
		_argc = 0;
		_argv = 0;
		// call petsc finish
		sout << "\tfinalizing Petsc" << std::endl;
		PetscErrorCode ierr = PetscFinalize();
		if (ierr) {
			sout << "Got petsc error code during destructor:\n" 
				<< PetscError(__LINE__,__FUNCT__,__FILE__,__SDIR__,ierr,0," ") << std::endl;
		}
		int initialized = 0;
		MPI_Initialized(&initialized);
		if (initialized)
			MPI_Finalize();
	}
}

void PetscInit::execute() {
	ParameteredObject::execute();

	// avoid duplicate initialization
	if(_initialized) {
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__ << "\n\t";	
		msg << "duplicate initialization of petsc!";
		throw std::runtime_error(msg.str().c_str());
	}

	// convert parameters into argument vector
	std::vector<std::string> args;
	StringTool::explode(commandLine(), ' ', args);

	_argc = args.size();
	_argv = new char* [_argc];
	for (std::vector<std::string>::size_type i=0; i < args.size(); i++) {
		const unsigned int s = args[i].length()+1;
		_argv[i] = new char[s];
		memset(_argv[i], '\0', s);
		args[i].copy(_argv[i], s);
	}

	_initialized = true;

	// call petsc init	
	sout << "\tinitializing Petsc" << std::endl;
	PetscErrorCode ierr = MPI_Init(&_argc,&_argv);
	if (ierr) {
		sout << "Got petsc error code during initialization:\n" 
			<< PetscError(__LINE__,__FUNCT__,__FILE__,__SDIR__,ierr,0," ") << std::endl;
	}
	PetscInitialize(&_argc,&_argv,PETSC_NULL,PETSC_NULL);
}

int PetscInit::argc() const {
	return _argc;
}

const char* const* PetscInit::argv() const {
	return _argv;
}

/// Creates an instance of the plugin
extern "C" petscinit_DECLDIR ParameteredObject* create(const std::string& name,
		template_type /*t*/) {
	return new PetscInit(name);
}

/// Deletes an instance of the plugin
extern "C" petscinit_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}
