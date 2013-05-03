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
/// @file paramObjSample.cpp
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg-de">Cornelius Ratsch</a>
/// Sample usage of parametered objects

#include <iostream>
#include <cstdlib>

#include <charon-core/ParameteredObject.hxx>
#include <charon-core/SplitStream.h>

/// sample ParameteredObject class.
class Sample : public ParameteredObject {
public:
	Parameter<int>   par1;   ///< sample integer parameter
	Parameter<float> par2;   ///< sample float parameter

	OutputSlot<int>   out1;  ///< sample integer output slot
	OutputSlot<float> out2;  ///< sample float output slot

	InputSlot<int>   in1;    ///< sample integer input slot
	InputSlot<float> in2;    ///< sample float input slot

	/// create a new sample object
	/// \param name             Object name
	Sample(const std::string& name = "") :
			ParameteredObject("sample", name, "some sample class"),
			par1(20),          // initial values ...
			par2(1.5),         // ... for parameters ...
			out1(8),           // ... and for output ...
			out2(7.3f),        // ... slots
			in2(true, true)    // make in2 optional and multislot
	{
		// parameters
		ParameteredObject::_addParameter (par1, "par1", "sample integer parameter");
		ParameteredObject::_addParameter (par2, "par2", "sample float parameter");

		// slots
		ParameteredObject::_addInputSlot (in1,  "in1",  "sample integer input slot");
		ParameteredObject::_addInputSlot (in2,  "in2",  "sample float input slot");
		ParameteredObject::_addOutputSlot(out1, "out1", "sample integer output slot");
		ParameteredObject::_addOutputSlot(out2, "out2", "sample float output slot");
	}
};

/// sample ParameteredObject class.
class Outputgen : public ParameteredObject {
public:
	OutputSlot<int>    out1;   ///< sample integer output slot
	OutputSlot<float>  out2;   ///< sample float output slot

	/// create a new sample object
	/// \param name             Object name
	Outputgen(const std::string& name = "") :
			ParameteredObject("outputgen", name,
				"class to generate int and float output"),
			out1(10),   // set initial values
			out2(5.5f)  // ------- " --------
	{
		ParameteredObject::_addOutputSlot(out1, "out1", "integer output slot");
		ParameteredObject::_addOutputSlot(out2, "out2", "float output slot");
	}
};


/// Main test application.
int main() {
	ParameterFile testfile;

	Sample* sample = new Sample;
	// sample->par1 is 20
	sample->par1 = 5;
	// now, sample->par1 == 5
	sample->saveParameters(testfile);

	// set new value
	sample->par1 = 10;
	// load old one
	sample->loadParameters(testfile);
	sout << "par1: " << sample->par1 << std::endl; // value is now 5.

	// parametered object connections
	Outputgen* outgen  = new Outputgen;
	Outputgen* outgen2 = new Outputgen;
	outgen->out1.connect(sample->in1);
	// slot in2 needs not to be connected because it's optional

	// try to connect multiple sources to in2
	outgen->out2.connect(sample->in2);
	outgen2->out2.connect(sample->in2);

	// outgen and sample are now in a connected component of 3 objects
	// outgen only has one neighbour, sample2
	// sample has two neighbours, outputgen and outgen2

	// save object and connections
	testfile.clear();
	sample->save(testfile);
	outgen->save(testfile);
	outgen2->save(testfile);

	// delete connected object
	// connection should be removed (also from outgen)
	delete sample;
	assert(!outgen->out1.connected());
	assert(!outgen->out2.connected());

	// cleanup
	delete outgen;
	delete outgen2;

	testfile.save("example.wrp");

	return EXIT_SUCCESS;
}
