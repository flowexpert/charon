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
/// @file paramObjSample.cpp
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg-de">Cornelius Ratsch</a>
/// Sample usage of parametered objects

#include <iostream>
#include <cstdlib>

#include "ParameteredObject.hxx"

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
