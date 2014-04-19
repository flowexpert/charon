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
/// @file Outputgen.h
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg-de">Cornelius Ratsch</a>
/// Sample plugin class
#ifndef OUTPUTGEN_H_
#define OUTPUTGEN_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef outputgen_EXPORTS
///Visual C++ specific code
#define outputgen_DECLDIR __declspec(dllexport)
#else
#define outputgen_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define outputgen_DECLDIR
#endif

#include <iostream>
#include <cstdlib>
#include "ParameteredObject.hxx"

/// sample ParameteredObject class.
class outputgen_DECLDIR Outputgen : public ParameteredObject {

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


#endif /* OUTPUTGEN_H_ */
