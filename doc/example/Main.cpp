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
/// @file Main.cpp
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg-de">Cornelius Ratsch</a>
/// Main function using sample plugins
#include "PluginManager.h"

/// Main test application.
int main() {
	PluginManager man;

    //Create sample instance
    ParameteredObject* sample = man.createInstance("sample", "sample");
    // sample->par1 is 20
    sample->setParameter<int>("par1", 5);
    // now, sample->par1 == 5

    std::cout << "par1: " << sample->getParameter("par1") << std::endl;

    // parametered object connections
    ParameteredObject* outgen  = man.createInstance("outputgen", "outputgen");
    ParameteredObject* outgen2  = man.createInstance("outputgen", "outgen2");
    man.connect(outgen->getSlot("out1"), sample->getSlot("in1"));
    // slot in2 needs not to be connected because it's optional

    // try to connect multiple sources to in2
    man.connect(outgen->getSlot("out2"), sample->getSlot("in2"));
    man.connect(outgen2->getSlot("out2"), sample->getSlot("in2"));

    // outgen and sample are now in a connected component of 3 objects
    // outgen only has one neighbour, sample2
    // sample has two neighbours, outputgen and outgen2

    // save object and connections
    man.saveParameterFile("example.wrp");

    // delete connected object
    // connection should be removed (also from outgen)
    man.destroyInstance(sample);
    assert(!outgen->getSlot("out1")->connected());
    assert(!outgen->getSlot("out2")->connected());

    // PluginManager does the cleanup for you.

    return EXIT_SUCCESS;
}
