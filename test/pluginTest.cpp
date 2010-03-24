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

#include <charon-utils/FileReader.h>
#include <charon-utils/ImageDisplay.h>
#include <charon-utils/Roi.h>
#include <list>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>

int main() {
	std::ofstream log("pluginTestLog.txt", std::ios::trunc);
	assert(log.good());
	sout.assign(std::cout, log);

    // load plugins
#ifdef WINDOWS
#ifdef _DEBUG
    PluginManager man("../bin/debug");
#else /* _DEBUG */
    PluginManager man("../bin/release");
#endif /* _DEBUG */
#else
    PluginManager man("../src");
#endif

    // create test instances (automatically loads the plugin)
    FileReader<double>* fileReader = (FileReader<double>*)man.createInstance("filereader", 0, "filereader");
	ImageDisplay<double>* imgDisplay = (ImageDisplay<double>*)man.createInstance("imagedisplay", 0, "imagedisplay");
	
	fileReader->filename = PENGUINFILE;
	fileReader->out.connect(imgDisplay->in);
	imgDisplay->execute();
    man.destroyInstance(fileReader);
	man.destroyInstance(imgDisplay);

    Roi<int>* roi = (Roi<int>*)man.createInstance("roi", 2, "roi");
	roi->xBegin() = 10;
	roi->assign(10, 20, 10, 20);
	assert(roi->getWidth() == 10);
	assert(roi->getHeight() == 10);
	assert(roi->getDepth() == 1);
	roi->execute();
    man.destroyInstance(roi);

	// test creation of some plugins
	std::list<std::string> plugins;
	std::list<std::string>::const_iterator plugin;
	plugins.push_back("InterpolatorLinear");
	plugins.push_back("InterpolatorCubic");
	plugins.push_back("LinearFilter");
	plugins.push_back("Mask1D");
	plugins.push_back("SteeredDerivative");
	plugins.push_back("ChannelConverter");
	plugins.push_back("StructureTensor");

	ParameteredObject* inst = 0;	
	for(plugin=plugins.begin(); plugin!=plugins.end(); plugin++) {
		inst = man.createInstance(*plugin);
		assert(inst);
		man.destroyInstance(inst);
	}

	sout.assign();
	log.close();

    return 0;
}
