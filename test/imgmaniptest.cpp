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

#include "PluginManager.h"
#include "ParameteredObject.h"
#include "FileTool.h"

#ifndef TESTWRPFILE
#define TESTWRPFILE "cropsample.wrp"
#warning using "cropsample.wrp"
#endif


int main() {
	// load plugins

	try{
#ifdef WINDOWS
#ifdef _DEBUG
	PluginManager man("./Debug");
#else /* _DEBUG */
	PluginManager man("./Release");
#endif /* _DEBUG */
#else
	PluginManager man("../imgmanip");
#endif

	man.loadPlugin("filereader");
	man.loadPlugin("imagedisplay");
	man.loadPlugin("normalize");
	man.loadPlugin("threshold");
	man.loadPlugin("crop");
	man.loadPlugin("roi");
	man.loadPlugin("interpolatorlinear");
	man.loadPlugin("interpolatorcubic");

	// create test instances
	ParameteredObject*    filereader   = man.createInstance  ("filereader", 0, "filereader");
	ParameteredObject*    imagedisplay = man.createInstance  ("imagedisplay", 0, "imagedisplay");
	ParameteredObject*    normalize    = man.createInstance  ("normalize", 0, "normalize");
	ParameteredObject*    threshold    = man.createInstance  ("threshold", 0, "threshold");
	ParameteredObject*    roi          = man.createInstance  ("roi", 2, "roi");
	ParameteredObject*    crop         = man.createInstance  ("crop", 0, "crop");
	ParameteredObject*    cubic        = man.createInstance  ("interpolatorcubic", 0, "interpolatorcubic");
	ParameteredObject*    linear       = man.createInstance  ("interpolatorlinear", 0, "interpolatorlinear");

	// cleanup
	man.destroyInstance(filereader);
	man.destroyInstance(imagedisplay);
	man.destroyInstance(normalize);
	man.destroyInstance(threshold);
	man.destroyInstance(roi);
	man.destroyInstance(crop);
	man.destroyInstance(cubic);
	man.destroyInstance(linear);

	if (FileTool::exists(TESTWRPFILE)) {
		man.loadParameterFile(TESTWRPFILE);
		man.executeWorkflow();
	}
	else {
		std::cerr << TESTWRPFILE << " missing" << std::endl;
		return -1;
	}
	} catch (AbstractPluginLoader::PluginException e) {
		std::cout << e.what() << std::endl;
		return -1;
	}

	return 0;
}
