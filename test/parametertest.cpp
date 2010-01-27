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
/// @file parametertest.cpp
/// Tests for Parameter and ParameteredObject classes.

#include <cstdlib>
#include <iostream>

#include "ParameteredObject.hxx"

// make sure that assert() works (nothing is tested otherwise)
#undef NDEBUG
#include <cassert>

/// sample ParameteredObject class.
class Sample : public ParameteredObject {

public:
    /// sample integer parameter
    Parameter<int>   par1;
    /// sample float parameter
    Parameter<float> par2;

    /// sample integer output slot
    OutputSlot<int>   out1;
    /// sample float output slot
    OutputSlot<float> out2;

    /// sample integer input slot
    InputSlot<int>   in1;
    /// sample float input slot
    InputSlot<float> in2;

    /// create a new sample object
    /// @param name             Object name
    Sample(const std::string& name = "") :
            ParameteredObject("sample", name, "some sample class"),
            par1(20),          // initial values ...
            par2(1.5),         // ... for parameters ...
            out1(8),           // ... and for output ...
            out2(7.3f),        // ... slots
            in2(true, true)    // make in2 optional and multislot
    {
        // parameters
        _addParameter (par1, "par1", "sample integer parameter");
        _addParameter (par2, "par2", "sample float parameter");

        // slots
        _addInputSlot (in1,  "in1",  "sample integer input slot");
        _addInputSlot (in2,  "in2",  "sample float input slot");
        _addOutputSlot(out1, "out1", "sample integer output slot");
        _addOutputSlot(out2, "out2", "sample float output slot");
    }
};

/// sample ParameteredObject class.
class Outputgen : public ParameteredObject {

public:
   /// sample integer output slot
    OutputSlot<int>    out1;
    /// sample float output slot
    OutputSlot<float>  out2;

    /// create a new sample object
    /// @param name             Object name
    Outputgen(const std::string& name = "") :
            ParameteredObject("outputgen", name,
                "class to generate int and float output"),
            out1(10),   // set initial values
            out2(5.5f)  // ------- " --------
    {
        // slots
        _addOutputSlot(out1, "out1", "integer output slot");
        _addOutputSlot(out2, "out2", "float output slot");
    }
};

/// Main test application.
int main() {
    ParameterFile testfile;
	ParameteredObject::setCreateMetadata(true);

    Sample* sample = new Sample(     "sample");
    assert(sample->par1 == 20);
    assert(sample->getName()      == "sample");
    assert(sample->getClassName() == "sample");
    sample->par1 = 5;
    assert(sample->par1 == 5);
    sample->saveParameters(testfile);

    // to view it by hand
    testfile.save("parametertest1.wrp");

    sample->par1 = 10;
    assert(sample->par1 == 10);
    sample->loadParameters(testfile);
    std::cout << "par1: " << sample->par1 << std::endl;
    assert(sample->par1 == 5);
    assert(sample->par1 != 20);

/*
    // check factory usage
    sample = dynamic_cast<Sample*>(
        ParameteredObject::getInstance("sample", "sample2"));
    assert(sample);
    assert(sample->par1 == 20);
*/

    // check connection settings
    assert(!sample->connected());
    Outputgen* outgen  = new Outputgen("outputgen");
    Outputgen* outgen2 = new Outputgen("outgen2");
    outgen->out1.connect(&(sample->in1));
    // slot in2 needs not to be connected because it's optional
    assert(sample->connected());

    // try to connect multiple sources to in2
    bool res = true;
    res = res && outgen->out2.connect(&(sample->in2));
    res = res && outgen2->out2.connect(&(sample->in2));
    assert(res);

    // save object and connections
    testfile.clear();
    sample->save(testfile);

    // to view it by hand
    testfile.save("parametertest2.wrp");

/*
    // outgen and sample should now be in a connected component of 3 objects
    std::set<std::string> nb = outgen->getNeighbours();
    // outgen only has one neighbour, sample2
    assert(nb.size() == 1);
    assert(nb.find("sample2") != nb.end());
    assert(outgen->getConnected().size() == 3);

    nb = sample->getNeighbours();
    // sample has two neighbours, outputgen and outgen2
    assert(nb.size() == 2);
    assert(nb.find("outputgen") != nb.end());
    assert(nb.find("outgen2") != nb.end());
    assert(sample->getConnected().size() == 3);

    // delete connected object
    // connection should be removed (also from outgen)
    delete ParameteredObject::getInstance("sample"   , "sample2");
    assert(outgen->getConnected().size() == 1);

    // reload connection (also checks object creation)
    outgen->load(testfile);
    assert(outgen->getConnected().size() == 3);
    // this should be the object created above
    sample = dynamic_cast<Sample*>(
        ParameteredObject::getInstance("sample", "sample2"));
    assert(sample->getConnected().size() == 3);

    // cleanup
    delete ParameteredObject::getInstance("sample"   , "sample2");
    delete ParameteredObject::getInstance("sample"   , "sample" );
    delete ParameteredObject::getInstance("outputgen", "outputgen");
    delete ParameteredObject::getInstance("outputgen", "outgen2");
*/
	delete sample;
	delete outgen;
	delete outgen2;
	TypeDetector::destroy();

	// check metadata generation
#ifdef MSVC
	assert(FileTool::exists("sample.wrp"));
	assert(FileTool::exists("outputgen.wrp"));
	testfile.load("sample.wrp");
#else
	assert(FileTool::exists("libsample.wrp"));
	assert(FileTool::exists("liboutputgen.wrp"));
	testfile.load("libsample.wrp");
#endif
	// check if input slot flags are correctly written
	assert(testfile.get<bool>("sample.in2.multi"));
	assert(testfile.get<bool>("sample.in2.optional"));

    return EXIT_SUCCESS;
}
