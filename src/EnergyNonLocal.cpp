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
/** \file EnergyNonLocal.cpp
 *  This file is needed for class EnergyNonLocal to work as a charon plugin.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 13.12.2011
 */

/// Class name of the plugin
#define TYPE EnergyNonLocal

///export symbols of parent classes/plugins must be defined
#define energynonlocal_EXPORTS
#define stencil_EXPORTS
#define penaltyfunction_EXPORTS

#include <charon/EnergyStencils/EnergyNonLocal.hxx>
//#include <charon/EnergyStencil.h>

/// Creates an instance of the plugin
extern "C" energynonlocal_DECLDIR ParameteredObject*
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
extern "C" energynonlocal_DECLDIR void destroy(ParameteredObject * b) {
        delete b;
}
