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

#include <FileReader.h>
#include <Roi.h>
//#include <ImageBlur.h>

#include <iostream>

int main() {
    // load plugins

#ifdef WINDOWS
#ifdef _DEBUG
    PluginManager man("./Debug");
#else /* _DEBUG */
    PluginManager man("./Release");
#endif /* _DEBUG */
#else
    PluginManager man("../src");
#endif

    // create test instances (automatically loads the plugin)
    FileReader<double>* fileReader =(FileReader<double>*)man.createInstance("filereader", 0, "filereader");
	fileReader->filename =PENGUINFILE;
	fileReader->execute();
	fileReader->out().display();
    man.destroyInstance(fileReader);

    Roi<int>* roi =(Roi<int>*)man.createInstance("roi", 2, "roi");
	roi->top =10;
	roi->assign(10, 10, 20, 20);
	roi->getWidth();
	roi->execute();
    man.destroyInstance(roi);

	//std::cin.get();
    return 0;
}
