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
/** @file charon.cpp
 *  Runner for charon workflows as parameter file.
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date 2010-02-15
 */

#ifdef STANDALONE_GETOPT
#include "getopt_standalone.h"
#else
#include <getopt.h>
#endif
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "../include/charon-core/configVersion.h"
#include "../include/charon-core/FileTool.h"
#include "../include/charon-core/SplitStream.h"
#include "../include/charon-core/ExceptionHandler.h"
#include "../include/charon-core/PluginManager.h"
#include "../include/charon-core/TypeDetector.h"

/// global configuration options
namespace Config {
	bool           verbose = false;     ///< more verbose output
	std::string    logfile;             ///< sout logfile
	std::string    paramFile;           ///< workflow parameter file
	std::string    globalPath;          ///< global plugin path
	std::string    localPath;           ///< local plugin path
	std::string    appName;             ///< application name
	std::string    workingDir;          ///< working directory
	std::string    oldDir;              ///< cwd at app start
	std::ofstream  log;                 ///< logging file stream
	PluginManager* man = 0;             ///< plugin manager
}

/// print usage info to stdout
void printInfo();
/// print version information
void printVersion();
/// parse command line and set up configuration
/// \param argc,argv     command line options
void init(int& argc, char**& argv);
/// execute workflow with given configuration using exception handler
int run();
/// close logfiles, reset plugin manager etc.
void cleanup();

/// main routine
/// \param argc,argv     command line options
int main(int argc, char* argv[]) {
	init(argc, argv);
	int ret = ExceptionHandler::run(run);
	return ret;
}

void printInfo() {
	std::cout << "\n === charon workflow executor ===\n\n"
		<< "Usage: " << Config::appName << " [options] parameterFile\n\n"
		<< "Options:\n\t"
		<< "-V  --version              Display version information\n\t"
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

void printVersion() {
	std::cout << "This is charon workflow executor "
		<< "from charon-core version " << CHARON_CORE_VERSION << "\n\n"
		<< "Copyright (C) 2009-2013 Heidelberg Collaboratory for Image Processing\n"
		<< "This is free software; see the source for copying conditions. "
		<< "There is NO\nwarranty; not even for MERCHANTABILITY or "
		<< "FITNESS FOR A PARTICULAR PURPOSE." << std::endl;
}

void init(int& argc, char**& argv) {
	// Parse command line options using GNU getopt.
	// See http://www.gnu.org/software/libc/manual/html_node/Getopt.html
	// for more information.
	//
	// Command line options:
	static struct option getoptLongOptions[] = {
		{"verbose",    no_argument,       NULL, 'v'},
		{"version",    no_argument,       NULL, 'V'},
		{"logfile",    required_argument, NULL, 'o'},
		{"help",       no_argument,       NULL, 'h'},
		{"global",     required_argument, NULL, 'g'},
		{"local",      required_argument, NULL, 'l'},
		{"workingdir", required_argument, NULL, 'w'},
		{NULL,         no_argument,       NULL,  0}
	};
	const char* getoptShortOptions = "vVo:hg:l:w:?";
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
		case 'V': printVersion(); exit(EXIT_SUCCESS);
		case 'o': Config::logfile = optarg;    break;
		case 'g': Config::globalPath = optarg; break;
		case 'l': Config::localPath = optarg;  break;
		case 'w': Config::workingDir = optarg; break;
		case '?':
		case 'h': printInfo(); exit(EXIT_SUCCESS);
		default : exit(EXIT_FAILURE);
		}
	}

	// remaining command line arguments
	if (optind < argc) Config::paramFile = argv[optind++];

	// check for still remaining arguments
	if (optind < argc) {
		printInfo();
		std::cerr << "ERROR: "
				<< "Too many additional arguments specified!\n";
		exit(EXIT_FAILURE);
	}

	// check for parameter file
	if (Config::paramFile.empty()) {
		printInfo();
		std::cerr << "ERROR: "
				<< "You have to specify some parameter file to execute!\n";
		exit(EXIT_FAILURE);
	}
	if (!FileTool::exists(Config::paramFile)){
		printInfo();
		std::cerr << "ERROR: "
				<< "Given parameter file (\"" << Config::paramFile
				<< "\") does not exist!\n";
		exit(EXIT_FAILURE);
	}

	// check for global plugin directory
	if (Config::globalPath.empty()) {
		printInfo();
		std::cerr << "ERROR: "
				<< "You have to specify a valid global plugin directory!\n";
		exit(EXIT_FAILURE);
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
}

int run() {
	std::ofstream log;
	if (!Config::logfile.empty()) {
		log.exceptions(std::ios_base::badbit | std::ios_base::failbit);
		log.open(Config::logfile.c_str(), std::ios::trunc | std::ios::out);
		sout.assign(log);
	}
	else {
		sout.assign(std::cout);
	}

	if (Config::verbose) {
		std::cout << "\nInitializing plugin manager." << std::endl;
	}

	Config::man = 0;
	Config::man = new PluginManager(Config::globalPath, Config::localPath);

	if (Config::verbose) {
		std::cout << "Loading workflow from \"" << Config::paramFile
				<< "\"." << std::endl;
	}

	Config::man->loadParameterFile(Config::paramFile);

	Config::oldDir = FileTool::getCurrentDir();
	if(!Config::workingDir.empty()) {
		if (Config::verbose)
			std::cout << "Entering directory " << Config::workingDir
					<< std::endl;
		FileTool::changeDir(Config::workingDir);
	}

	if (Config::verbose) {
		std::cout << "Starting workflow execution." << std::endl;
	}
	Config::man->runWorkflow();

	if (Config::verbose) {
		std::cout << "Finished workflow execution." << std::endl;
	}

	// reset man, reassign sout before log gets out of scope
	cleanup();

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
	sout.assign(std::cout);
	if (Config::log.is_open()) {
		Config::log.close();
	}
	if (Config::verbose) {
		std::cout << "Cleanup finished. Bye" << std::endl;
	}
}
