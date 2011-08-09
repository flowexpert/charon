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
/// @file imgmaniptest.cpp
/// Some tests for imgmanip usage.
/// @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// @date 11.04.2009

#include <charon-core/PluginManager.h>
#include <charon-core/ParameteredObject.h>
#include <charon-core/FileTool.h>
#include <charon-core/ExceptionHandler.h>
#include <charon-utils/FileWriter.h>

#ifndef GLOBAL_PLUGIN_DIR
#error GLOBAL_PLUGIN_DIR not defined
#define GLOBAL_PLUGIN_DIR
#endif
#ifndef LOCAL_PLUGIN_DIR
#error LOCAL_PLUGIN_DIR not defined
#define LOCAL_PLUGIN_DIR
#endif

/// unit tests
int test() {
	// load plugins
	sout.assign(std::cout);

	try{
#ifdef CMAKE_INTDIR
	PluginManager man(GLOBAL_PLUGIN_DIR "/" CMAKE_INTDIR, LOCAL_PLUGIN_DIR "/" CMAKE_INTDIR);
#else
	PluginManager man(GLOBAL_PLUGIN_DIR,LOCAL_PLUGIN_DIR);
#endif

	man.loadPlugin("FileReader");
	man.loadPlugin("Normalize");
	man.loadPlugin("Threshold");
	man.loadPlugin("Crop");
	man.loadPlugin("Roi");
	man.loadPlugin("InterpolatorLinear");
	man.loadPlugin("InterpolatorCubic");
	man.loadPlugin("ImageBlur");
	man.loadPlugin("LinearFilter");
	man.loadPlugin("Mask1D");
	man.loadPlugin("SteeredDerivative");
	man.loadPlugin("ChannelConverter");
	man.loadPlugin("StructureTensor");

	// create test instances
	ParameteredObject* filereader = man.createInstance  ("FileReader");
	ParameteredObject* normalize  = man.createInstance  ("Normalize");
	ParameteredObject* threshold  = man.createInstance  ("Threshold");
	ParameteredObject* roi =
			man.createInstance  ("roi", ParameteredObject::TYPE_INT);
	ParameteredObject* crop       = man.createInstance  ("Crop");
	ParameteredObject* cubic      = man.createInstance  ("InterpolatorCubic");
	ParameteredObject* linear     = man.createInstance  ("InterpolatorLinear");
	ParameteredObject* blur       = man.createInstance  ("ImageBlur");
	ParameteredObject* linFil     = man.createInstance  ("LinearFilter");
	ParameteredObject* mask1d     = man.createInstance  ("Mask1D");
	ParameteredObject* steerDeriv = man.createInstance  ("SteeredDerivative");
	ParameteredObject* channConv  = man.createInstance  ("ChannelConverter");
	ParameteredObject* strucTens  = man.createInstance  ("StructureTensor");

	// cleanup
	man.destroyInstance(filereader);
	man.destroyInstance(normalize);
	man.destroyInstance(threshold);
	man.destroyInstance(roi);
	man.destroyInstance(crop);
	man.destroyInstance(cubic);
	man.destroyInstance(linear);
	man.destroyInstance(blur);
	man.destroyInstance(linFil);
	man.destroyInstance(mask1d);
	man.destroyInstance(steerDeriv);
	man.destroyInstance(channConv);
	man.destroyInstance(strucTens);

	std::string curDir = FileTool::getCurrentDir();
	FileTool::changeDir(TESTDIR);
	if (FileTool::exists("blur.wrp")) {
		man.loadParameterFile("blur.wrp");
		FileWriter<float>* writer = dynamic_cast<FileWriter<float>*>(
				man.getInstance("writer"));
		if (!writer) {
			std::cerr << "Failed to get pointer to writer instance!"
					<< std::endl;
			return EXIT_FAILURE;
		}
		writer->filename() = curDir + "/blurTestOut.pgm";
		man.executeWorkflow();
		if (!FileTool::exists(writer->filename())) {
			std::cerr << "Output File not written!" << std::endl;
			return EXIT_FAILURE;
		}

		// check result
		const cimg_library::CImgList<float>& curRes = writer->in();
		cimg_library::CImgList<float> res("PenguinBlurred.cimg");
		if (!res.is_sameNXYZC(1,60,60,1,1)) {
			std::cerr << "Result load error! (dim mismatch)" << std::endl;
			return EXIT_FAILURE;
		}
		if (!res.is_sameNXYZC(curRes)) {
			std::cerr << "Result dimension mismatch!" << std::endl;
			return EXIT_FAILURE;
		}
		int err = (res[0]-curRes[0]).abs().max();
		if (err > 0) {
			std::cerr << "Result content differs!" << std::endl;
			return EXIT_FAILURE;
		}
	}
	else {
		std::cerr << "test parameter file missing" << std::endl;
		return EXIT_FAILURE;
	}
	} catch (AbstractPluginLoader::PluginException e) {
		std::cout << e.what() << std::endl;
		return -1;
	}

	return 0;
}

/// main application
int main() {
	return ExceptionHandler::run(test);
}
