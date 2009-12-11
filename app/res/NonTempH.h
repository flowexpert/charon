/*  Copyright (C) 2009 @Author@

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
/// @file @pluginName@.h
/// @author @Author@
/// @date @date@
/// Sample plugin class
#ifndef @pluginNameUpper@_H_
#define @pluginNameUpper@_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef @pluginNameLower@_EXPORTS
///Visual C++ specific code
#define @pluginNameLower@_DECLDIR __declspec(dllexport)
#else
#define @pluginNameLower@_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define @pluginNameLower@_DECLDIR
#endif

#include <iostream>
#include <cstdlib>
#include "ParameteredObject.hxx"

/// sample ParameteredObject class.
class @pluginNameLower@_DECLDIR @pluginName@ : public ParameteredObject {

public:

    @Parameter@
    


    @In/Out@
    

    /// create a new sample object
    /// @param name             Object name
    @pluginName@(const std::string& name = "");

	/// Update object.
    virtual void execute();
};

#endif /* @pluginNameUpper@_H_ */
