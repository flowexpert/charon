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

#ifndef LOCAL_PLUGIN_DIR
#error LOCAL_PLUGIN_DIR not defined
#define LOCAL_PLUGIN_DIR
#endif

/// unit tests
int test() {
	// load plugins
	sout.assign(std::cout);

#ifdef CMAKE_INTDIR
	PluginManager man(LOCAL_PLUGIN_DIR "/" CMAKE_INTDIR);
#else
	PluginManager man(LOCAL_PLUGIN_DIR);
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

	return EXIT_SUCCESS;
}

/// main application
int main() {
	return ExceptionHandler::run(test);
}
