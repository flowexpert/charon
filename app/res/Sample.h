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
/// @file Sample.h
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg-de">Cornelius Ratsch</a>
/// Sample plugin class
#ifndef SAMPLE_H_
#define SAMPLE_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef sample_EXPORTS
///Visual C++ specific code
#define sample_DECLDIR __declspec(dllexport)
#else
#define sample_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define sample_DECLDIR
#endif

#include <iostream>
#include <cstdlib>
#include "ParameteredObject.hxx"

/// sample ParameteredObject class.
class sample_DECLDIR Sample : public ParameteredObject {

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

#endif /* SAMPLE_H_ */
