/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** @file topoSortTest.cpp
 *  Testing PluginManager::determineExecutionOrder
 *  \date 15.09.2011
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include <cstdlib>
#include <iostream>

#include "../include/charon-core/ParameteredObject.hxx"
#include "../include/charon-core/ExceptionHandler.h"
#include "../include/charon-core/PluginManager.h"
#include "../include/charon-core/AbstractPluginLoader.h"

/// Main test application.
void testValid() {
	std::cout << "-- check valid plugin" << std::endl;
	std::ostringstream log;
	sout.assign(log);
	PluginManager man(MODULE_DIR,"",
			DEFAULT_DEBUG_SUFFIX, false,
			PluginManagerInterface::PluginVersionDiscard);
	man.loadPlugin("DummyModule");
	sout.assign(std::cout);
	if (man.getLoadedPluginsCount() != 1) {
		throw std::runtime_error("plugin load failed");
	}
	if (man.getInstancesCount() != 0) {
		throw std::runtime_error("too much instances");
	}
	if (log.str().find("ELF check successful") == std::string::npos) {
		throw std::runtime_error("ELF check success message missing");
	}
	std::cout << log.str();
}

void testInValid1() {
	std::cout << "-- check invalid plugin: missing version info" << std::endl;
	PluginManager man(MODULE_DIR,"",
			DEFAULT_DEBUG_SUFFIX, false,
			PluginManagerInterface::PluginVersionDiscard);
	try {
		man.loadPlugin("DummyModule2");
	}
	catch (const AbstractPluginLoader::PluginException& exp) {
		if (exp.getErrorCode() == AbstractPluginLoader::PluginException::
				VERSION_INFORMATION_MISSING) {
			std::cout << "(WW) " << exp.what() << std::endl;
		}
		else {
			throw;
		}
	}
}

void testInValid2() {
	std::cout << "-- check invalid plugin: wrong version info" << std::endl;
	PluginManager man(MODULE_DIR,"",
			DEFAULT_DEBUG_SUFFIX, false,
			PluginManagerInterface::PluginVersionDiscard);
	try {
		man.loadPlugin("DummyModule3");
	}
	catch (const AbstractPluginLoader::PluginException& exp) {
		if (exp.getErrorCode() == AbstractPluginLoader::PluginException::
				VERSION_MISSMATCH) {
			std::cout << "(WW) " << exp.what() << std::endl;
		}
		else {
			throw;
		}
	}
}

void testInValid3() {
	std::cout << "-- check invalid plugin: invalid version info" << std::endl;
	PluginManager man(MODULE_DIR,"",
			DEFAULT_DEBUG_SUFFIX, false,
			PluginManagerInterface::PluginVersionDiscard);
	try {
		man.loadPlugin("DummyModule4");
	}
	catch (const AbstractPluginLoader::PluginException& exp) {
		if (exp.getErrorCode() == AbstractPluginLoader::PluginException::
				VERSION_MISSMATCH) {
			std::cout << "(WW) " << exp.what() << std::endl;
		}
		else {
			throw;
		}
	}
}

void testInValid4() {
	std::cout << "-- check invalid plugin: warn-only mode" << std::endl;
	PluginManager man(MODULE_DIR,"",
			DEFAULT_DEBUG_SUFFIX, false,
			PluginManagerInterface::PluginVersionWarn);
	std::ostringstream log;
	sout.assign(log);
	man.loadPlugin("DummyModule3");
	sout.assign(std::cout);
	std::string logStr = log.str();
	size_t wpos = logStr.find("(WW)");
	if (wpos == std::string::npos) {
		throw std::runtime_error("no warning emitted");
	}
	if (logStr.find("version mismatch") == std::string::npos) {
		throw std::runtime_error("version mismatch not detected");
	}
	if (logStr.find("loaded successfully") == std::string::npos) {
		throw std::runtime_error("success message missing");
	}
	std::cout << logStr;
}

int main() {
	// output to standart out
	sout.assign(std::cout);
	int ret = EXIT_SUCCESS;
	ret |= ExceptionHandler::run(testValid);
	ret |= ExceptionHandler::run(testInValid1);
	ret |= ExceptionHandler::run(testInValid2);
	ret |= ExceptionHandler::run(testInValid3);
	ret |= ExceptionHandler::run(testInValid4);
	return ret;
}
