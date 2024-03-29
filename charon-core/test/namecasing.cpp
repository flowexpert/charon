/*  Copyright (C) 2009 Jens-Malte Gottfried

	This file is part of Charon.

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
/// @file parametertest.cpp
/// Tests for Parameter and ParameteredObject classes.

#include <cstdlib>
#include <iostream>

#include <charon-core/ParameteredObject.hxx>
#include <charon-core/ExceptionHandler.h>

// make sure that assert() works (nothing is tested otherwise)
#undef NDEBUG
#include <cassert>

#define caseddummy_EXPORTS

#include "CasedDummy.h"
#include <charon-core/PluginManager.h>

/// test direct connection (without parameter file)
void standalone() {
	CasedDummy mod1, mod2;
	mod1.oUt.connect(mod2.iN);
	assert(!mod1.iN.connected());
	assert(mod1.oUt.connected());
	assert(mod2.iN.connected());
	assert(!mod2.oUt.connected());
}

void checkFixCase() {
	CasedDummy mod1;
	std::string res;
	res = mod1.fixCase("in");
	assert(res == "iN");
	res = mod1.fixCase("In");
	assert(res == "iN");
	res = mod1.fixCase("out");
	assert(res == "oUt");
	res = mod1.fixCase("ouT");
	assert(res == "oUt");
}

void checkPFile(const ParameterFile& pf) {
	PluginManager* man = new PluginManager(MODULE_DIR);
	man->loadParameterFile(pf);
	CasedDummy *mod1, *mod2;
	mod1 = dynamic_cast<CasedDummy*>(man->getInstance("mod1"));
	mod2 = dynamic_cast<CasedDummy*>(man->getInstance("mod2"));
	assert(mod1);
	assert(mod2);
	assert(!mod1->iN.connected());
	assert(mod1->oUt.connected());
	assert(mod2->iN.connected());
	assert(!mod2->oUt.connected());
	man->reset();
	sout << std::endl;
}

/// test connection with parameter file, check auto case correction
void paramFile() {
	ParameterFile pFile;

	// this is, how it should look like
	pFile.set<std::string>("mod1.type","CasedDummy");
	pFile.set<std::string>("mod2.type","CasedDummy");
	pFile.set<std::string>("mod1.oUt","mod2.iN");
	pFile.set<std::string>("mod2.iN","mod1.oUt");
	checkPFile(pFile);

	// misspell target cases
	pFile.set<std::string>("mod1.oUt","mod2.in");
	pFile.set<std::string>("mod2.iN","mod1.out");
	checkPFile(pFile);

	// misspell source cases
	pFile.erase("mod1.oUt");
	pFile.erase("mod2.iN");
	pFile.set<std::string>("mod1.out","mod2.iN");
	pFile.set<std::string>("mod2.in","mod1.oUt");
	checkPFile(pFile);
}

/// Main test application.
int main() {
	// output to standart out
	sout.assign(std::cout);

	int ret = EXIT_SUCCESS;
	ret += ExceptionHandler::run(standalone);
	ret += ExceptionHandler::run(checkFixCase);
	ret += ExceptionHandler::run(paramFile);
	return ret;
}
