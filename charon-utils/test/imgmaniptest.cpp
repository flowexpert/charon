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
/// @file imgmaniptest.cpp
/// Some tests for imgmanip usage.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @date 11.04.2009

#include <charon-core/PluginManager.h>
#include <charon-core/ParameteredObject.h>
#include <charon-core/FileTool.h>
#include <charon-core/ExceptionHandler.h>
#include <charon-utils/FileWriter.h>

#ifndef LOCAL_PLUGIN_DIR
#error LOCAL_PLUGIN_DIR not defined
#define LOCAL_PLUGIN_DIR
#endif

/// unit tests
void test() {
	// load plugins
	sout.assign(std::cout);

#ifdef CMAKE_INTDIR
	PluginManager man(LOCAL_PLUGIN_DIR "/" CMAKE_INTDIR);
#else
	PluginManager man(LOCAL_PLUGIN_DIR);
#endif
	std::string curDir = FileTool::getCurrentDir();
	FileTool::changeDir(TESTDIR);
	if (!FileTool::exists("blur.wrp")) {
		throw "test parameter file missing";
	}
	man.loadParameterFile("blur.wrp");
	FileWriter<float>* writer = dynamic_cast<FileWriter<float>*>(
			man.getInstance("writer"));
	if (!writer) {
		throw "Failed to get pointer to writer instance!";
	}
	writer->filename() = curDir + "/blurTestOut.pgm";
	man.runWorkflow();
	if (!FileTool::exists(writer->filename())) {
		throw "Output File not written!";
	}

	// check result
	const cimg_library::CImgList<float>& curRes = writer->in();
	cimg_library::CImgList<float> res("PenguinBlurred.cimg");
	if (!res.is_sameNXYZC(1,60,60,1,1)) {
		throw "Result load error! (dim mismatch)";
	}
	if (!res.is_sameNXYZC(curRes)) {
		throw "Result dimension mismatch!";
	}
	int err = (res[0]-curRes[0]).abs().max();
	if (err > 0) {
		throw "Result content differs!";
	}
}

/// main application
int main() {
	return ExceptionHandler::run(test);
}
