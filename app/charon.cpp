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
/** @file charon.cpp
 *  Runner for charon workflows as parameter file.
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date 2010-02-15
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#ifdef STANDALONE_GETOPT
#include "getopt_standalone.h"
#else
#include <getopt.h>
#endif
#include <FileTool.h>
#include <SplitStream.h>
#include <ExceptionHandler.h>
#include <PluginManager.h>
#include <TypeDetector.h>

namespace Config {
	bool           verbose     = false;
	std::string    logfile;
	std::string    paramFile;
	std::string    globalPath;
	std::string    localPath;
	std::string    appName;
	std::string    workingDir;
	std::string    oldDir;
	std::ofstream  log;
	PluginManager* man;
}

void printInfo();
int run();
void cleanup();

/// print usage info to stdout
void printInfo() {
	std::cout << "\n === charon workflow executor ===\n\n"
		<< "Usage: " << Config::appName << " [options] parameterFile\n\n"
		<< "Options:\n\t"
		<< "-v  --verbose              More verbose output\n\t"
		<< "-?\n\t"
		<< "-h  --help                 This message\n\t"
		<< "-o  --logfile      file    log to given text file too\n\t"
		<< "-g  --global       path    path of global plugin dir\n\t"
		<< "-l  --local        path    path of local plugin dir\n\t"
		<< "-w  --workingdir   path    path of working directory\n\t"
		<< "\n"
		<< "parameterFile:\n\t"
		<< "Parameter file describing the workflow to execute.\n\t"
		<< "This may be set up e.g. by Tuchulcha.\n\t"
		<< std::endl;
}

/// main routine
int main(int argc, char* argv[]) {
	// Parse command line options using GNU getopt.
	// See http://www.gnu.org/software/libc/manual/html_node/Getopt.html
	// for more information.
	//
	// Command line options:
	static struct option getoptLongOptions[] = {
		{"verbose",    no_argument,       NULL, 'v'},
		{"logfile",    required_argument, NULL, 'o'},
		{"help",       no_argument,       NULL, 'h'},
		{"global",     required_argument, NULL, 'g'},
		{"local",      required_argument, NULL, 'l'},
		{"workingdir", required_argument, NULL, 'w'},
		{NULL,         no_argument,       NULL,  0}
	};
	const char* getoptShortOptions = "vo:hg:l:w:?";
	assert(no_argument == 0);

	while (true) {
		int optionIndex = 0;
		int curOpt = getopt_long (
				argc, argv,
				getoptShortOptions, getoptLongOptions, &optionIndex);

		/* Detect the end of the options. */
		if (curOpt == -1)
			break;

		switch (curOpt) {
			case 'v': Config::verbose = true;      break;
			case 'o': Config::logfile = optarg;    break;
			case 'g': Config::globalPath = optarg; break;
			case 'l': Config::localPath = optarg;  break;
			case 'w': Config::workingDir = optarg; break;
			case '?':
			case 'h': printInfo(); return EXIT_SUCCESS;
			default : return EXIT_FAILURE;
		}
	}

	// remaining command line arguments
	if (optind < argc) Config::paramFile = argv[optind++];

	// check for still remaining arguments
	if (optind < argc) {
		printInfo();
		std::cerr << "ERROR: "
				<< "Too many additional arguments specified!\n";
		abort();
	}

	// check for parameter file
	if (Config::paramFile.empty()) {
		printInfo();
		std::cerr << "ERROR: "
				<< "You have to specify some parameter file to execute!\n";
		abort();
	}
	if (!FileTool::exists(Config::paramFile)){
		printInfo();
		std::cerr << "ERROR: "
				<< "Given parameter file (\"" << Config::paramFile
				<< "\") does not exist!\n";
		abort();
	}

	// check for global plugin directory
	if (Config::globalPath.empty() || !FileTool::exists(Config::globalPath)) {
		printInfo();
		std::cerr << "ERROR: "
				<< "You have to specify a valid global plugin directory!\n";
		abort();
	}

	// print configuration
	if (Config::verbose) {
		std::cout << "Configuration:\n\t";
		std::cout << "logfile           : "
				<< (!Config::logfile.empty() ? Config::logfile :
					"none (stdout only)")
				<< "\n\t";
		std::cout << "pluginPath        : " << Config::globalPath << "\n\t";
		std::cout << "pluginPath (local): "
				<< (!Config::localPath.empty() ? Config::localPath :
					"none (optional)")
				<< "\n\t";
		std::cout << "parameter file    : " << Config::paramFile << "\n\t";
		std::cout << "working directory : "
				<< (!Config::workingDir.empty() ? Config::workingDir :
					FileTool::getCurrentDir()) << "\n\t";
	}

	// start application with given configuration using exception handler
	int ret = ExceptionHandler::run(run);
	cleanup();
	return ret;
}

/// main routine with exception handling
int run() {
	std::ofstream log;
	if (!Config::logfile.empty()) {
		log.exceptions(std::ios_base::badbit | std::ios_base::failbit);
		log.open(Config::logfile.c_str(), std::ios::trunc | std::ios::out);
		sout.assign(log);
	}

	if (Config::verbose)
		std::cout << "\nInitializing plugin manager." << std::endl;

	Config::man = 0;
	Config::man = new PluginManager(Config::globalPath, Config::localPath);

	if (Config::verbose)
		std::cout << "Loading workflow from \"" << Config::paramFile
				<< "\"." << std::endl;
	Config::man->loadParameterFile(Config::paramFile);

	Config::oldDir = FileTool::getCurrentDir();
	if(!Config::workingDir.empty()) {
		if (Config::verbose)
			std::cout << "Entering directory " << Config::workingDir
					<< std::endl;
		FileTool::changeDir(Config::workingDir);
	}

	if (Config::verbose)
		std::cout << "Starting workflow execution." << std::endl;
	Config::man->executeWorkflow();

	if (Config::verbose)
		std::cout << "Finished workflow execution." << std::endl;

	return EXIT_SUCCESS;
}

void cleanup() {
	if(!Config::workingDir.empty()) {
		if (Config::verbose)
			std::cout << "Leaving directory " << Config::workingDir
					<< std::endl;
		FileTool::changeDir(Config::oldDir);
	}
	if (Config::man) {
		Config::man->reset();
		delete Config::man;
		Config::man = 0;
	}
	if(Config::log.is_open()) {
		sout.assign(std::cout);
		Config::log.close();
	}
	TypeDetector::destroy();
	if (Config::verbose)
		std::cout << "Cleanup finished. Bye" << std::endl;
}
