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
/** @file ExceptionHandler.cpp
 *  Implementation of class WorkflowExecutor
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date 2010-02-11
 */

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#ifdef __GNUG__
#include <cxxabi.h>
#endif // __GNUG__
#include "../include/charon-core/ExceptionHandler.h"
#include "../include/charon-core/TypeDetector.h"
#include "../include/charon-core/SplitStream.h"

int ExceptionHandler::run(int (&method)()) {
	int ret = EXIT_FAILURE;
	try {
		ret = method();
	}
	catch (const std::exception& e) {
		const char* name = typeid(e).name();
#ifdef __GNUG__
		name = abi::__cxa_demangle(name, 0, 0, 0);
#endif // __GNUG__
		std::cerr << "\n(EE) Caught exception of type " << name << ".\n";
		std::cerr << "(EE) Message:\n(EE) \t" << e.what() << std::endl;
	}
	catch (const std::string& e) {
		std::cerr << "\n(EE) Caught exception of type std::string.\n";
		std::cerr << "(EE) Message:\n(EE) \t" << e << std::endl;
	}
	catch (const char*& e) {
		std::cerr << "\n(EE) Caught exception of type const char*.\n";
		std::cerr << "(EE) Message:\n(EE) \t" << e << std::endl;
	}
	catch (...) {
		std::cerr << "\n(EE) Caught unknown exception." << std::endl;
	}
	TypeDetector::destroy();
	return ret;
}

int ExceptionHandler::run(void (&method)()) {
	bool excpt = false;
	try {
		method();
	}
	catch (const std::exception& e) {
		const char* name = typeid(e).name();
#ifdef __GNUG__
		name = abi::__cxa_demangle(name, 0, 0, 0);
#endif // __GNUG__
		std::cerr << "\n(EE) Caught exception of type " << name << ".\n";
		std::cerr << "(EE) Message:\n(EE) \t" << e.what() << std::endl;
		excpt = true;
	}
	catch (const std::string& e) {
		std::cerr << "\n(EE) Caught exception of type std::string.\n";
		std::cerr << "(EE) Message:\n(EE) \t" << e << std::endl;
		excpt = true;
	}
	catch (const char*& e) {
		std::cerr << "\n(EE) Caught exception of type const char*.\n";
		std::cerr << "(EE) Message:\n(EE) \t" << e << std::endl;
		excpt = true;
	}
	catch (...) {
		std::cerr << "\n(EE) Caught unknown exception." << std::endl;
		excpt = true;
	}
	TypeDetector::destroy();
	return excpt ? EXIT_FAILURE : EXIT_SUCCESS;
}

int ExceptionHandler::checkRaise(void (&method)()) {
	bool excpt = false;
	try {
		method();
	}
	catch (const std::exception& e) {
		const char* name = typeid(e).name();
#ifdef __GNUG__
		name = abi::__cxa_demangle(name, 0, 0, 0);
#endif // __GNUG__
		sout << "(II) Caught exception of type " << name << ".\n";
		sout << "(II) Message:\n(II) \t" << e.what() << std::endl;
		excpt = true;
	}
	catch (const std::string& e) {
		sout << "(II) Caught exception of type std::string.\n";
		sout << "(II) Message:\n(II) \t" << e << std::endl;
		excpt = true;
	}
	catch (const char*& e) {
		sout << "(II) Caught exception of type const char*.\n";
		sout << "(II) Message:\n(II) \t" << e << std::endl;
		excpt = true;
	}
	catch (...) {
		sout << "(II) Caught unknown exception." << std::endl;
		excpt = true;
	}
	TypeDetector::destroy();
	if (excpt) {
		return EXIT_SUCCESS;
	}
	sout << "\n(EE) Expected runtime error exception!" << std::endl;
	return EXIT_FAILURE;
}

