/*  Copyright (C) 2011 Michael Baron

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
/** \file EnergyBCC.cpp
 *  This file is needed for class EnergyBCC to work as a charon plugin.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 30.08.2011
 */

/// Class name of the plugin
#define TYPE EnergyBCC
#define energystencil_EXPORTS
#include <charon/EnergyStencils/EnergyBCC.hxx>
#include <charon-utils/DataManagerCImgList.hxx>

/// Creates an instance of the plugin
extern "C" energybcc_DECLDIR ParameteredObject*
                create(const std::string & name, template_type t) {
        switch(t) {
        case ParameteredObject::TYPE_DOUBLE:
                return new TYPE<double>(name);
                break;
        case ParameteredObject::TYPE_FLOAT:
                return new TYPE<float>(name);
                break;
        case ParameteredObject::TYPE_INT:
                return new TYPE<int>(name);
                break;
        default:
                return new TYPE<int>(name);
                break;
        }
}

/// Deletes an instance of the plugin
extern "C" energybcc_DECLDIR void destroy(ParameteredObject * b) {
        delete b;
}
