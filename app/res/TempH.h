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
/** @file @pluginName@.h
 * Declaration of the parameter class @pluginName@.
 * @author @Author@
 * @date 20.08.2009
 *

 */

#ifndef _@pluginName@_H_
#define _@pluginName@_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef @pluginName@_EXPORTS
///Visual C++ specific code
#define @pluginName@_DECLDIR __declspec(dllexport)
#else
#define @pluginName@_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define @pluginName@_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <CImg.h>


template <typename T>
class @pluginName@_DECLDIR @pluginName@ : public TemplatedParameteredObject<T> {
public:

@Parameter@


@In/Out@


    /// create a new sample object
    /// @param name             Object name
    @pluginName@(const std::string& name);

    /// Update object.
    /// Reload image and put new data into the output slot.
    virtual void execute();
};

#endif // _@pluginName@_H_
